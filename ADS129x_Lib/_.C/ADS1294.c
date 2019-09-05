#include "ADS1294.h"
#include "main.h"
//------------------ Local functions prototypes --------------------------------//
static void PowerUP(SPI_HandleTypeDef * hspi1); 
static void PowerDOWN();
static uint8_t RREG(SPI_HandleTypeDef * hspi1,uint8_t  address);
static void WREG(SPI_HandleTypeDef * hspi1,uint8_t address, uint8_t data);
static void SendComand (SPI_HandleTypeDef * hspi1, uint8_t comand);
static void SetConfigurations (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac);
static void ExternalREFvoltage (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac);
//------------------------------------------------------------------------------//
//------------------ Global functions ------------------------------------------//
void Init_ADS1294 (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac) {
  volatile static uint8_t id = 0x00;                                            // Для чтения ID регистра
  
  PowerDOWN();                                                                  // Ставим в ноль цифровые входы управления запуском
  HAL_Delay(1000); 
  PowerUP(hspi1);                                                               // Подымаем микросхему с видержкой всех тайменгов (см. Даташит).
  
  SendComand(hspi1, STANDBY);                                                   // После поднятия микросхемы нужно войти в в режим STANDBY, SDATAC и остановить 
  SendComand(hspi1, SDATAC);                                                    // передачу данных, для установки нужных конфигураций микросхемы
  SendComand(hspi1, STOP);                                                      //
  
  id = RREG(hspi1, ID_ADDRESS);                                                 // Проверку правильного запуска микросхемы выполнено через считывания 
  if((id == ID_DATA_4) || (id == ID_DATA_6) || (id == ID_DATA_8)){              // ID регистра. При правельном запуске, регистр считываеться, проверяеться 
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);                    // на тип микросхемы после чего должен замигать световой индикатор
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  }    
  
  SetConfigurations (hspi1,hdac);                                               // Установка Конфигураций микросхемы
  
  HAL_GPIO_WritePin(START_GPIO_Port, START_Pin,  GPIO_PIN_RESET);               // После установки конфигураций, загоняем обратно микросхему в режим 
  SendComand(hspi1, START);                                                     // непрерывной регестрации даных со входов АЦП
  SendComand(hspi1, RDATAC); 
  SendComand(hspi1, WAKEUP);
}

void ReadData (SPI_HandleTypeDef *  hspi1, UART_HandleTypeDef * huart1)
{
  volatile uint8_t DataOut[27] = {0x00};
  uint8_t packageCounter = 0;
  
  cs_Set();
  for( int i=0; i<6; i++)
  {
    HAL_SPI_Transmit(hspi1, (uint8_t*)0x23, 1, 1);
    DataOut[i] = SPI1->DR;
  }
  cs_Reset();
  // -1 - 1/1
  // 0 - 1/2
  // 1 - 1/3
  // 2 - 1/4
  if(packageCounter > 0)
  {
    // every 10 packages
    HAL_UART_Transmit(huart1, (uint8_t*)DataOut, 6, 1);
    packageCounter = 0;
  } 
  else 
  {
    packageCounter++;
  }
}
//------------------------------------------------------------------------------//
//---------------- Local Funtions ----------------------------------------------//
static void PowerUP (SPI_HandleTypeDef * hspi1){
  cs_Reset();
  HAL_GPIO_WritePin(START_GPIO_Port, START_Pin,  GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(PWDN_GPIO_Port, PWDN_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
  
  SendComand(hspi1, RESET_ADS1294);
  HAL_Delay(110);
  
  HAL_GPIO_WritePin(PWDN_GPIO_Port, PWDN_Pin,  GPIO_PIN_RESET);
  SendComand(hspi1, RESET_ADS1294);
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(PWDN_GPIO_Port, PWDN_Pin,  GPIO_PIN_SET); 
  SendComand(hspi1, RESET_ADS1294);
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(15);
    
  cs_Set();
  HAL_Delay(1);
  cs_Reset();    
}   

static void PowerDOWN () {
  HAL_GPIO_WritePin(START_GPIO_Port, START_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PWDN_GPIO_Port,  PWDN_Pin,  GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
}

static uint8_t RREG(SPI_HandleTypeDef * hspi1,uint8_t  address) 
{
  uint8_t result = 0; 
  uint8_t opcode1 = R_REG | address ;
  uint8_t opcode_x[2];
  opcode_x[0] = opcode1;
  opcode_x[1] = 0x00;
  uint8_t read = 0x00;
  
  cs_Set();
  HAL_Delay(1);
  
  HAL_SPI_Transmit(hspi1, &opcode_x[0], 1, 1);
  (void)(SPI1->DR);
  HAL_Delay(1);
  HAL_SPI_Transmit(hspi1, &opcode_x[1], 1, 1);
  (void)(SPI1->DR);
  HAL_Delay(1);
  HAL_SPI_Transmit(hspi1, &read, 1, 1);
  result = SPI1->DR;
  HAL_Delay(1);
  
  HAL_Delay(2);
  cs_Reset();
  
  HAL_Delay(1);
  return result;
}

static void WREG(SPI_HandleTypeDef * hspi1, uint8_t address, uint8_t data) 
{
  uint8_t opcode1 =  W_REG | address;
  uint8_t opcode_x[2];
  
  opcode_x[0] = opcode1;
  opcode_x[1] = 0x00;
  cs_Set();
  HAL_Delay(1);
  
  HAL_SPI_Transmit(hspi1, &opcode_x[0], 1, 1);
  (void)(SPI1->DR);
  HAL_Delay(1);        
  HAL_SPI_Transmit(hspi1, &opcode_x[1], 1, 1);
  (void)(SPI1->DR);
  HAL_Delay(1);        
  HAL_SPI_Transmit(hspi1, &data, 1, 1);
  (void)(SPI1->DR);
  HAL_Delay(1); 
  
  HAL_Delay(2);
  cs_Reset();
  
  HAL_Delay(1);          
}

static void SendComand (SPI_HandleTypeDef * hspi1, uint8_t comand)
{
  cs_Set();
  HAL_Delay(1);
  
  HAL_SPI_Transmit( hspi1, &comand, 1, 1);
  (void)(SPI1->DR);
  HAL_Delay(1);
  
  HAL_Delay(2);
  cs_Reset();
  
  HAL_Delay(1);
}

static void SetConfigurations (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac)
{      
//---- Set common CONFIGURATIONS ----------------------------------------------------------------------------------------------------------------
   
/* CONFIG 3 Register */   WREG(hspi1, CONFIG3_ADRESS, (CONFIG3_const | 
                                   PD_REFBUF_ENAB_INT | VREF_2V4 | RLD_MEAS_FOR_MUX_010 | RLDREF_INT | RLDBUF_ENABLE | RLD_LOFF_SENS_DISABLE));
                          ExternalREFvoltage(hspi1,hdac);
      
/* CONFIG 1 Register */   WREG(hspi1, CONFIG1_ADRESS, (CONFIG1_const | 
                                   HR | Multiple_readback | OSC_output_disable | SPS_500_HR_250_LP) );
      
/* CONFIG 2 Register */   WREG(hspi1, CONFIG2_ADRESS, (CONFIG2_const | 
                                   WCT_CHOP_FrVaries | INT_TEST_INT | TEST_SIGNAL_AMP_2 | TEST_FREQ_20));
      
/* LEAD-OFF Register */   WREG(hspi1, LOFF_ADRESS, (LEAD_OFF_Const |
                                   COMP_TH_95_5 | VLEAD_OFF_EN_CS | I_LEAD_OFF_6nA | F_LEAD_OFF_AC));
      
/* CONFIG 4 Register */   WREG(hspi1, CONFIG4_ADRESS,(CONFIG4_const | 
                                    RESP_FREQ_64kHZ | CONTINIUS_CONVERSETION_MODE_SH | WCT_TO_RLD_OFF | PD_LOFF_COMP_DISABLED));
            
//---- Set CHN Configurations --------------------------------------------------------------------------------------------------------------------      
                          
/*Chn_1*/      WREG(hspi1, CH1SET_ADRESS, (CN_N_SET_const |
                                    PWDN_CHN_NORMAL_OPERATION | PGA_6 | MUX_NORMAL_ELEC_INPUT));
/*Chn_2*/      WREG(hspi1, CH2SET_ADRESS, (CN_N_SET_const |
                                    PWDN_CHN_NORMAL_OPERATION | PGA_6 | MUX_NORMAL_ELEC_INPUT));
/*Chn_3*/      WREG(hspi1, CH3SET_ADRESS, (CN_N_SET_const |
                                    PWDN_CHN_NORMAL_OPERATION | PGA_6 | MUX_NORMAL_ELEC_INPUT));
/*Chn_4*/      WREG(hspi1, CH4SET_ADRESS, (CN_N_SET_const |
                                    PWDN_CHN_NORMAL_OPERATION | PGA_6 | MUX_USED_WITH_RLD_MEAS_bit));

/*For another ADS129x*/              
/*Chn_5*/      //WREG(hspi1, CH5SET_ADRESS, 0x60);  
/*Chn_6*/      //WREG(hspi1, CH6SET_ADRESS, 0x60);
/*Chn_7*/      //WREG(hspi1, CH7SET_ADRESS, 0x60);
/*Chn_8*/      //WREG(hspi1, CH8SET_ADRESS, 0x62);

}

static void ExternalREFvoltage (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac) 
{
  volatile static uint8_t CONFIG_3 = 0x00; 
  CONFIG_3 = RREG(hspi1, CONFIG3_ADRESS);
  if( (CONFIG_3 & 0x80) == 0x00 )
  {
    HAL_DAC_Start(hdac,DAC_CHANNEL_1);
    HAL_DAC_SetValue(hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,4095);
  }
}
//------------------------------------------------------------------------------//
    
 
 