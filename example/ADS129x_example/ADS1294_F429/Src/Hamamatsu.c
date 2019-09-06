#include "Hamamatsu.h"


void I2C_ReadREG(I2C_HandleTypeDef * hi2c1, uint8_t Adress_REG)
{
  uint8_t get = 0x00;
  while(HAL_I2C_Mem_Read(hi2c1, Device_ID, Adress_REG, 1, (uint8_t*)get, 1, 1)!= HAL_OK)
   {
    if (HAL_I2C_GetError(hi2c1) != HAL_I2C_ERROR_AF)
     {
     }
   }

}

void I2C_WriteREG(I2C_HandleTypeDef * hi2c1, uint8_t Adress_REG, uint8_t Data_REG)
{
  uint8_t send[1] = {Data_REG};
  while(HAL_I2C_Mem_Write(hi2c1, Device_ID, Adress_REG, 1, send, 1, 1)!= HAL_OK)
   {
    if (HAL_I2C_GetError(hi2c1) != HAL_I2C_ERROR_AF)
     {
     }
   }

}



void I2C_ReadData(I2C_HandleTypeDef * hi2c)
{
  
HAL_I2C_AddrCallback(hi2c,I2C_FIRST_FRAME,Device_ID);
    
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

void Hamamatsu_INIT (I2C_HandleTypeDef * hi2c1)
{
  I2C_WriteREG(hi2c1, RGB_sensor_control, RGB_Censor_Control_INIT);
  I2C_WriteREG(hi2c1, LED_drive_CTRL_1, LED_Drv_Cntrl_1_INIT);
  I2C_WriteREG(hi2c1, LED_drive_CTRL_1, LED_Drv_Cntrl_2_INIT);
  
  HAL_Delay(400); 
}

void Fixed_teme_mode (I2C_HandleTypeDef * hi2c)
{
  I2C_WriteREG(hi2c, RGB_sensor_control, 0x81);
  LED_drive(hi2c);
  I2C_WriteREG(hi2c, RGB_sensor_control, 0x01); 
  HAL_Delay(10);
  I2C_ReadData(hi2c);
/*for(int i = 0; i <250; i++){
  HAL_Delay(5);
  I2C_ReadData(hi2c);
  }  */

  I2C_WriteREG(hi2c, RGB_sensor_control, 0x41);
  I2C_WriteREG(hi2c, LED_drive_CTRL_1, 0xC0);
      
}

void Manual_setting_mode (I2C_HandleTypeDef * hi2c)
{
  uint8_t value[4] = {0x00,0x8D,0x01,0x65};
  HAL_I2C_Master_Transmit(hi2c, Device_ID, value, 4, 1);
  
  //I2C_WriteREG(hi2c, RGB_sensor_control, 0x8D);
  //I2C_WriteREG(hi2c, Manual_tim_H, 0x01);
  //I2C_WriteREG(hi2c, Manual_tim_L, 0x65);
  I2C_WriteREG(hi2c, RGB_sensor_control, 0x0D);
  LED_drive(hi2c);
  HAL_Delay(2);
  I2C_ReadData(hi2c);
    
  I2C_WriteREG(hi2c, LED_drive_CTRL_1, 0xC0);
}


void LED_drive (I2C_HandleTypeDef * hi2c)
{
  I2C_WriteREG(hi2c, LED_drive_CTRL_1, 0xF0);
  I2C_WriteREG(hi2c, LED_drive_CTRL_2, 0x60);
  I2C_WriteREG(hi2c, LED_drive_CTRL_1, 0x30);
}