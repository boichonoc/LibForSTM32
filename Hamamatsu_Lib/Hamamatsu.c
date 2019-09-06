#include "Hamamatsu.h"


uint8_t I2C_ReadREG(I2C_HandleTypeDef * hi2c, uint8_t Adress_REG)
{
  uint8_t getValue = 0x00;
  uint8_t g = 0x00;
  
  while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
  g = HAL_I2C_Master_Transmit(hi2c, Device_ID, &Adress_REG, 1, 1);
  if (g != HAL_OK)
  {
    return g;
  }
  
  while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
  g = HAL_I2C_Master_Receive(hi2c, Device_ID, &getValue, 1, 1);
  if ( g != HAL_OK )
  {
    return g;
  }
  
  return getValue;
}

uint8_t I2C_WriteREG(I2C_HandleTypeDef * hi2c, uint8_t Adress_REG, uint8_t Data_REG)
{
  uint8_t buf[] = {Adress_REG, Data_REG};
  uint8_t setValue;
  while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
  setValue = HAL_I2C_Master_Transmit(hi2c, Device_ID, buf, 2, 2);
  if ( setValue != HAL_OK) {
      return setValue;
  }
  return HAL_OK;
}


void I2C_ReadData(I2C_HandleTypeDef * hi2c, UART_HandleTypeDef * huart)
{
  uint8_t call = 0x05;
  uint8_t dataGet[2] = {0x00,0x00};
   
  //dataGet[0] = I2C_ReadREG(hi2c, Output_DATA_H);
  //dataGet[1] = I2C_ReadREG(hi2c, Output_DATA_L);
  
  //HAL_I2C_Master_Receive(hi2c, Device_ID, dataGet, 2, 1);
  HAL_I2C_Mem_Read(hi2c, Device_ID, Output_DATA_H, 1, &dataGet[0], 1, 2);
  HAL_I2C_Mem_Read(hi2c, Device_ID, Output_DATA_L, 1, &dataGet[1], 1, 2);
  //HAL_I2C_Mem_Read(hi2c, Device_ID, call, 1, dataGet, 2, 1);
  char out[8];
  uint16_t var;
  var = dataGet[0] << 8 | dataGet[1];
  sprintf(out, "$%d;", var);
  //sprintf(out, "0x%02X%02X\r\n", dataGet[0], dataGet[1]);
  //HAL_UART_Transmit(huart, (uint8_t*)dataGet, 2, 1);
  HAL_UART_Transmit(huart, out, 8, 1);
}


void Registers_STAT (I2C_HandleTypeDef * hi2c)
{
  I2C_ReadREG(hi2c, 0x00);
  HAL_Delay(100);
  I2C_ReadREG(hi2c, 0x01);
  HAL_Delay(100);
  I2C_ReadREG(hi2c, 0x02);
  HAL_Delay(100);
  I2C_ReadREG(hi2c, 0x03);
  HAL_Delay(100);
  I2C_ReadREG(hi2c, 0x04);
  HAL_Delay(100);
  I2C_ReadREG(hi2c, 0x05);
  HAL_Delay(100);
  I2C_ReadREG(hi2c, 0x0E);
  HAL_Delay(100);
  I2C_ReadREG(hi2c, 0x0F);
  HAL_Delay(100);
  I2C_ReadREG(hi2c, 0x10);
  
}

void Hamamatsu_INIT (I2C_HandleTypeDef * hi2c, UART_HandleTypeDef * huart)
{
  while(1) {
  //for (int i=0; i<6; i++){
  LED_drive(hi2c);
  I2C_WriteREG(hi2c, RGB_sensor_control, 0x84);
  I2C_WriteREG(hi2c, RGB_sensor_control, 0x04);
  HAL_Delay(400);
  I2C_ReadData(hi2c, huart);
  
/*I2C_ReadREG(hi2c, RGB_sensor_control);
  I2C_ReadREG(hi2c, monitor_STNBY);*/
  }
}

void Fixed_teme_mode (I2C_HandleTypeDef * hi2c, UART_HandleTypeDef * huart, TIM_HandleTypeDef * htim)
{
  LED_drive(hi2c);
  //I2C_WriteREG(hi2c, RGB_sensor_control, 0xF1);
  
  
   
  I2C_WriteREG(hi2c, RGB_sensor_control, 0x81);
  I2C_WriteREG(hi2c, RGB_sensor_control, 0x01);
  
  while(1) {
    //for( int i=0; i<250; i++){
    HAL_Delay(4);
    //HAL_Delay(190);
    I2C_WriteREG(hi2c, RGB_sensor_control, 0x61);
    //HAL_Delay(10);
    HAL_Delay(1);
    I2C_ReadData(hi2c, huart);
    I2C_WriteREG(hi2c, RGB_sensor_control, 0x01);
  }
  I2C_ReadREG(hi2c, RGB_sensor_control);
  I2C_ReadREG(hi2c, monitor_STNBY);
  
  /*HAL_TIM_Base_Start(htim);
  HAL_TIM_Base_Start_IT(htim);
  HAL_TIM_Base_Stop(htim);*/
  
/*for(int i = 0; i <60; i++){
  
  if( (I2C_ReadREG(hi2c, monitor_STNBY) & 0x40) != 0x40)
   { 
     
     HAL_Delay(5);
     I2C_ReadData(hi2c,huart);
   } 
  } */ 
 
}

void Manual_setting_mode (I2C_HandleTypeDef * hi2c, UART_HandleTypeDef * huart)
{
  
}

void LED_drive (I2C_HandleTypeDef * hi2c)
{
  I2C_WriteREG(hi2c, LED_drive_CTRL_1, 0xA0);
  I2C_WriteREG(hi2c, LED_drive_CTRL_2, 0x80);
  I2C_WriteREG(hi2c, LED_drive_CTRL_1, 0x20);
}

