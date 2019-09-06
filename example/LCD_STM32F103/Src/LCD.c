#include "stm32f1xx_hal.h"
#include "LCD.h"

void Send_Byte (uint8_t byte)
{
  if( (byte & 0x01) == 0x00 ) D0_0 else D0_1;
  if( (byte & 0x02) == 0x00 ) D1_0 else D1_1;
  if( (byte & 0x04) == 0x00 ) D2_0 else D2_1;
  if( (byte & 0x08) == 0x00 ) D3_0 else D3_1;
  
  if( (byte & 0x10) == 0x00 ) D4_0 else D4_1;
  if( (byte & 0x20) == 0x00 ) D5_0 else D5_1;
  if( (byte & 0x40) == 0x00 ) D6_0 else D6_1;
  if( (byte & 0x80) == 0x00 ) D7_0 else D7_1;
}
void LCD_Command (uint8_t cmd)
{  
  INSTRUCT_INPUT;
  RW_WRITE;
  Send_Byte(cmd);
  ENABLE;
  HAL_Delay(1);
  DISABLE;  
}

void LCD_Data (uint8_t data)
{  
  DATA_INPUT
  RW_WRITE;
  Send_Byte(data);
  ENABLE;
  HAL_Delay(1);
  DISABLE;  
}

void LCD_Init()
{ 
  HAL_Delay(40);
  INSTRUCT_INPUT;
  //**********
  Send_Byte(0x30);
  
  ENABLE;
  HAL_Delay(1);
  DISABLE;
  
  HAL_Delay(5);
  //***********
  Send_Byte(0x30);
  
  ENABLE;
  HAL_Delay(1);
  DISABLE;
  
  HAL_Delay(1);
  //**********
  Send_Byte(0x30);
  
  ENABLE;
  HAL_Delay(1);
  DISABLE;
  
  HAL_Delay(1);
  LCD_Command(0x38);
 
  HAL_Delay(1);
  LCD_Command(0x38);
 
  LCD_Command(0x0C);
  HAL_Delay(1);
  
  LCD_Command(0x01);
  HAL_Delay(2);
  
  LCD_Command(0x06);
  HAL_Delay(1);
  
  LCD_Command(0x02);
  HAL_Delay(2);
  
}

void LCD_Clean()
{
  LCD_Command(0x01);
  HAL_Delay(2);
}

void LCD_SendChar( char ch )
{
  LCD_Data((uint8_t)ch);
  HAL_Delay(1);
}

void LCD_String( char* st )
{
  
  uint8_t i = 0;
  
  while (st[i] != 0)
  {
    LCD_Data(st[i]);
    HAL_Delay(1);
    i++;
  }
}

void LCD_SetPos(uint8_t c, uint8_t r)
{
  switch(r)
  {
    case 0:
      LCD_Command(c|0x80);
      HAL_Delay(1);
      break;
    case 1:
      LCD_Command((0x40+c)|0x80);
      HAL_Delay(1);
      break;    
  }
}
//---------- 4bit mode ---------------------------------------------------------
void Send_Byte_4bit(uint8_t byte)
{ 
  if( ((byte>>0) & 0x10) == 0x00 ) D4_0 else D4_1;
  if( ((byte>>1) & 0x20) == 0x00 ) D5_0 else D5_1;
  if( ((byte>>2) & 0x40) == 0x00 ) D6_0 else D6_1;
  if( ((byte>>3) & 0x80) == 0x00 ) D7_0 else D7_1;
  
  /*
     if(((dt >> 3)&0x01)==1) {D7_1} else {D7_0}

        if(((dt >> 2)&0x01)==1) {D6_1} else {D6_0}

        if(((dt >> 1)&0x01)==1) {D5_1} else {D5_0}

        if((dt&0x01)==1) {D4_1} else {D4_0}
  */
}

void LCD_Command_4bit (uint8_t cmd)
{  
  INSTRUCT_INPUT;
  RW_WRITE;
  Send_Byte_4bit(cmd>>4);
  ENABLE;
  HAL_Delay(1);
  DISABLE; 
  Send_Byte_4bit(cmd);
  ENABLE;
  HAL_Delay(1);
  DISABLE;  
}

void LCD_Init_4bit()
{
  HAL_Delay(40);
  INSTRUCT_INPUT;
  RW_WRITE;
  
  Send_Byte_4bit(0x30);
  ENABLE;
  HAL_Delay(1);
  DISABLE;
  
  HAL_Delay(2);
  
  Send_Byte_4bit(0x30);
  ENABLE;
  HAL_Delay(1);
  DISABLE;
  
  HAL_Delay(2);
  
  Send_Byte_4bit(0x30);
  ENABLE;
  HAL_Delay(1);
  DISABLE;
  
  HAL_Delay(2);
  
  LCD_Command_4bit(0x28);
  HAL_Delay(1);
  
  LCD_Command_4bit(0x28);
  HAL_Delay(1);
  
  LCD_Command_4bit(0x0C);
  HAL_Delay(1);
  
  LCD_Command_4bit(0x01);
  HAL_Delay(2);
  
  LCD_Command_4bit(0x06);
  HAL_Delay(1);
  
  LCD_Command_4bit(0x02);
  HAL_Delay(2);
   
}

void LCD_Data_4bit(uint8_t data)
{
  DATA_INPUT
  RW_WRITE;
  Send_Byte(data>>4);
  ENABLE;
  HAL_Delay(1);
  DISABLE;  
  Send_Byte(data);
  ENABLE;
  HAL_Delay(1);
  DISABLE;  
}

void LCD_Clean_4bit()
{
  LCD_Command_4bit(0x01);
  HAL_Delay(2);
}

void LCD_SendChar_4bit( char ch )
{
  LCD_Data_4bit((uint8_t)ch);
  HAL_Delay(1);
}

void LCD_String_4bit( char* st )
{
  
  uint8_t i = 0;
  
  while (st[i] != 0)
  {
    LCD_Data_4bit(st[i]);
    HAL_Delay(1);
    i++;
  }
}

void LCD_SetPos_4bit(uint8_t c, uint8_t r)
{
  switch(r)
  {
    case 0:
      LCD_Command_4bit(c|0x80);
      HAL_Delay(1);
      break;
    case 1:
      LCD_Command_4bit((0x40+c)|0x80);
      HAL_Delay(1);
      break;    
  }
}
