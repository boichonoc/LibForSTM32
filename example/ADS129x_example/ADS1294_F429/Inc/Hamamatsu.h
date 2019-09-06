#ifndef Hamamatsu
#define Hamamatsu

#include "stm32f4xx_hal.h"

//------------------------- Adres Registers ------------------------------------
#define Device_ID          0x54           // Device ID Hamamatsu


#define RGB_sensor_control   0x00           // RGB sensor control register adrs.

#define Manual_tim_H         0x01           // Manual timing (Hight byte)
#define Manual_tim_L         0x02           // Manual timing (Low byte)

#define Output_DATA_H        0x03           // Output data (Hight byte)
#define Output_DATA_L        0x04           // Output data (Low byte)

//-- 0x05 - 0x0D: NOT USED -----------------------------------------------------

#define LED_drive_CTRL_1     0x0E           // LED driver control 1
#define LED_drive_CTRL_2     0x0F           // LED driver control 2

//-------- NOTE: Recomend that the LEDs be used in DC mode ---------------------

//------------------------- Value of Registers----------------------------------

//---------------------- RGB sensor control register -------------------------//
#define RGB_SC_Const             0x00 // MASK of register value (RGB sens)           
//----- Reset ------------------------//
#define Operation_res            0x00 // 
#define Reset_res                0x80 //          
//----- Standby ----------------------//
#define Operation_stb            0x00 //          
#define Standby                  0x40 //  
//----- RegisterReset ----------------//
#define Reset_release            0x00 //
#define Address_03_0A_Data_reset 0x10 //
//----- Gain -------------------------//
#define High                     0x00 //
#define Low                      0x08 //
//----- Integration mode -------------//
#define Fixed_Time               0x00 //
#define Manual_setting           0x04 //
//----- Integration time -------------//
#define T_32us                   0x00 //
#define T_1ms                    0x01 //
#define T_16ms4                  0x02 //
#define T_131ms                  0x03 //
//----------------------------------------------------------------------------//

//------------------------- LED drive conrol 1 -------------------------------//
#define LED_DC_1_Const           0x00 //
//----- LED Reset --------------------//
#define Operation_LedRS          0x00 //
#define Reset_LedRS              0x80 //
//----- LED Standby ------------------//
#define Operation_LedSTB         0x00 //
#define Standby_LedSTB           0x40 //
//----- DC mode ----------------------//
#define Pulse_mode               0x00 //
#define DC_mode                  0x20 //
//----- 1/10 mode --------------------//
#define Normal_mode              0x00 //
#define mode_1_10                0x10 //
//----------------------------------------------------------------------------//

//------------------------- LED drive control 2 ------------------------------//
#define LED_DC_2_Const           0x00 //
//----- Current ----------------------//
#define mA_0                     0x00 //
#define mA_64                    0x80 //
#define mA_32                    0x40 //
#define mA_16                    0x20 //
#define mA_8                     0x10 //
//----------------------------------------------------------------------------//

//------------------------- Initial setings registers ------------------------//

#define RGB_Censor_Control_INIT  0xD4
#define LED_Drv_Cntrl_1_INIT     0xC0
#define LED_Drv_Cntrl_2_INIT     0x00

//----------------------------------------------------------------------------//

void Hamamatsu_INIT (I2C_HandleTypeDef * hi2c1);
void I2C_ReadREG(I2C_HandleTypeDef * hi2c1, uint8_t Adress_REG);
void I2C_WriteREG(I2C_HandleTypeDef * hi2c1, uint8_t Adress_REG, uint8_t Data_REG);
void I2C_ReadData(I2C_HandleTypeDef * hi2c);
void Fixed_teme_mode (I2C_HandleTypeDef * hi2c);
void Manual_setting_mode (I2C_HandleTypeDef * hi2c);
void LED_drive (I2C_HandleTypeDef * hi2c);
void Registers_STAT (I2C_HandleTypeDef * hi2c);


#endif