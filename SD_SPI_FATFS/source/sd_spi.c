#include "sd_spi.h"
//----------------------- Local Varibales --------------------------------------

//----------------------- Extern Varibales -------------------------------------
extern char uartBuffer[60];
//------------------- Static Functions Prototype -------------------------------
static void SdSelect(uint8_t state);
static uint8_t LedError(uint8_t State);
static uint8_t SdReadByte(void);
static void SdWriteByte(uint8_t Byte);
static void SdPowerOn(void);
static void InitSequence(void);
//------------------------------------------------------------------------------
//----------------------- Static Functions -------------------------------------
//------------------------- SPI Functions --------------------------------------
/** Select/DeSelect SD Card
    */
void SdSelect(uint8_t State){
  if( State == 0 )
    /** If state == 0 then SD Card is Selected
    */
    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
  else
    /** If state == 1 then SD Card is Deselected
    */
    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
}
/** Read Data from SD card by SPI interface
    */
uint8_t SdReadByte(void){
  volatile uint8_t ReceiveByte = 0;
  uint8_t tx[1] = {0xFF};
  /** If SPI Transmit != HAL_Ok then LedError(PC13) will be on
  */
  SdSelect(0);
  if( HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)tx, (uint8_t*)&ReceiveByte, 1, 1000) != HAL_OK )
  {
    SdSelect(1);
    return LedError(1);
  }
  else
  /** If SPI Transmit -= HAL_Ok then LedError(PC13) will be off, and function return recive byte
  */
  {
    SdSelect(1);
    return ReceiveByte;
  } 
}
/** Write Data to SD card by SPI interface
    */
void SdWriteByte(uint8_t Byte){
  volatile uint8_t ReceiveByte = 0;
  SdSelect(0);
  if( HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&Byte, (uint8_t*)&ReceiveByte, 1, 1000) != HAL_OK )
  {
    SdSelect(1);
    LedError(1);
  }
  else
  {
    SdSelect(1);
    LedError(0);
  }
}
//------------------------------------------------------------------------------
/** Status Led Error
    */
uint8_t LedError(uint8_t State){
  if( State == 1)
  {
    /** If state == 1 it's Error
    */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    return 1;
  }
  else
  {
    /** If state == 0 it's Ok 
    */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    return 0;
  }
}
/** Send Comand to SD Card
    */
uint8_t SdSendComand(uint8_t comand, uint32_t argument){
  uint8_t responseTimeout = 10;
  uint8_t crc = 0x01;
  volatile uint8_t result;
  
  if( comand & 0x80 )
  {
    comand &= 0x7F;
    result = SdSendComand(CMD55, 0);
    if( result > 1) return result; 
  }
 
  SdReadByte();
  SdReadByte();
  
  SdWriteByte(comand);
  SdWriteByte((uint8_t)(argument >> 24));
  SdWriteByte((uint8_t)(argument >> 16));
  SdWriteByte((uint8_t)(argument >> 8));
  SdWriteByte((uint8_t)argument);
  
  if (comand == CMD0)
    crc = 0x95;
  if (comand == CMD8)
    crc = 0x87; 
  SdWriteByte(crc);
  
  do {
    result = SdReadByte();
  } while ((result & 0x80) && --responseTimeout);
  
  return result;
}
/** Power On SD Card
  */
void SdPowerOn(void){
  HAL_Delay(20);
}
/** Initialization sequence
  */
void InitSequence(void){
  volatile uint8_t ReceiveByte = 0;
  uint8_t tx[1] = {0xFF};
  SdSelect(1);
  for( int i = 0; i < 10; i++)
  {
    HAL_SPI_TransmitReceive(&hspi2, tx, (uint8_t*)&ReceiveByte, 1, 0x1000);
  }
  SdSelect(0);
  //SdSelect(1);
}

//------------------------ Global Functions ------------------------------------
SD_HandleTypeDef SdInit(uint32_t SdBaudRate){
  uint8_t cmd = 0;
  uint16_t tmr = 0;
  SD_HandleTypeDef SdCard;
  SdCard.SdBaudRate = SdBaudRate;
  SdCard.SdError = NO_ERRORS;
  SdCard.SdStatus = NOT_REDY;
  
  hspi2.Init.BaudRatePrescaler = SdCard.SdBaudRate;
  HAL_SPI_Init(&hspi2);
  
  SdPowerOn();
  InitSequence();
  
  if( SdSendComand(CMD0, 0) == 1 )
  {
    SdReadByte();
    if (SdSendComand(CMD8, 0x01AA) == 1) // SDv2
    {
      SdCard.SdType = SD2;
      for( int i = 0; i < 4; i++ )
        SdCard.OCR[i] = SdReadByte();
      
      if (SdCard.OCR[2] == 0x01 && SdCard.OCR[3] == 0xAA) // The card can work at vdd range of 2.7-3.6V        
        for (tmr = 12000; tmr && SdSendComand(ACMD41, 1UL << 30); tmr--);
      
      if (tmr && SdSendComand(CMD58, 0) == 0) // Check CCS bit in the OCR
        for (int i = 0; i < 4; i++) 
          SdCard.OCR[i] = SdReadByte();
      
      SdCard.SdType = (SdCard.OCR[0] & 0x40) ? SD2HC : SD2; // SDv2 (HC or SC)
    }
    else //SDv1 or MMCv3 
    {
      if (SdSendComand(ACMD41, 0) <= 1)
      {
       SdCard.SdType = SD1; cmd = ACMD41; // SDv1
      }
      else
      {
       SdCard.SdType = MMC; cmd = CMD1; // MMCv3
      }
      for (tmr = 25000; tmr && SdSendComand(cmd, 0); tmr--);
      if (!tmr || SdSendComand(CMD16, 512) != 0) {
        SdCard.SdType   = UNDEFTYPE;
        SdCard.SdStatus = NOT_REDY;
      }
    }
  }
  else
  {
    SdCard.SdError  = WRONG_COMMAND;
    SdCard.SdStatus = NOT_REDY;
    return SdCard;
  }
  sprintf(uartBuffer,"Type SD: 0x%02X\r\n",SdCard.SdType);
  
  HAL_UART_Transmit(&huart1,(uint8_t*)uartBuffer,strlen(uartBuffer),0x1000);
  SdCard.SdStatus = REDY;
  return SdCard;
}

uint8_t SdCardStatus(SD_HandleTypeDef * SdCard)
{
  uint8_t res;
  uint16_t cnt;
  cnt=0;
  SdSelect(0);
  do { 
    res = SdReadByte();
    cnt++;
  } while ( (res != 0xFF)&&(cnt < 0xFFFF) );
  SdSelect(1);
  if (cnt >= 0xFFFF){
    SdCard->SdStatus = BUSY;
    return 1;
  }
  SdCard->SdStatus = REDY;
  return res;
}

uint8_t SdReadBlock(uint8_t *buff, uint16_t buffSize, uint32_t lba, uint8_t comand){
  uint8_t result = 0;
  uint16_t countBuffer = 0;
  
  result = SdSendComand(comand, lba);
  if( result != 0x00 )
    return READ_BLOCK_ERROR;
  
  SdReadByte();
  do{
    result = SdReadByte();   
    countBuffer++;
  } while( (result == 0xFF)&&(countBuffer < 0xFFFF) );
  if ( result != 0xFE )
    return READ_BLOCK_ERROR;
  
  do { /* Receive the data block into buffer */
    (*buff++) = SdReadByte();
    (*buff++) = SdReadByte();
  } while (buffSize -= 2);
  
  //for (countBuffer = 0; countBuffer < buffSize; countBuffer++) 
  //  buff[countBuffer] = SdReadByte();
  
  SdReadByte(); 
  SdReadByte();
  
  return NO_ERRORS;
}

uint8_t SdWriteBlock (uint8_t *buff, uint32_t lba, uint8_t comand)
{
  uint8_t result = 0; 
  uint16_t countBuffer;
    
  if( comand != CMD23 ){
    result = SdSendComand(comand,lba);
    if (result != 0x00) 
      return WRITE_BLOCK_ERROR; 
    SdReadByte();
    if( comand == CMD24 )
      SdWriteByte(0xFE);
    if( comand == CMD25 )
      SdWriteByte(0xFC);
  } else {
    SdSendComand(CMD55,0);
    result = SdSendComand(comand,lba);
    if (result != 0x00)
      return WRITE_BLOCK_ERROR;
  }
  /*countBuffer = 0;
  do{
    SdWriteByte(*buff++);
    SdWriteByte(*buff++);
  }while(--countBuffer);*/
  
  for (countBuffer = 0; countBuffer < BUFFER_SIZE; countBuffer++){
    SdWriteByte( buff[countBuffer] );
  }
  
  SdReadByte(); 
  SdReadByte();
  
  result = SdReadByte();
  
  if ((result & 0x05)!= 0x05) 
    return WRITE_BLOCK_ERROR; 
  
  countBuffer = 0;
  do { 
    result = SdReadByte();
    countBuffer++;
  } while ( (result != 0xFF)&&(countBuffer < 0xFFFF) );
  
  if (countBuffer >= 0xFFFF) 
    return WRITE_BLOCK_ERROR;
  
  return 0;
 
}
