#ifndef ADS1294
#define ADS1294


//Opcode Command Definitions
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"


#include "stm32f1xx_hal.h"
//#include "stm32f4xx_hal.h"


//SYSTEM COMMANDS  
 #define WAKEUP  0x02 // WAKE UP from standly mode
 #define STANDBY 0x04 // STANDBY - Enter standly mode
 #define RESET_ADS1294   0x06 // RESET the device
 #define START   0x08 // START/RESTART (synchronize) conversions
 #define STOP    0x0A // STOP conversion

//DATA READ COMANDS
 #define RDATAC 0x10 // Enable Read Data Continuous mode(defaulte mode at power up)
 #define SDATAC 0x11 // Stop read Data Continuously mode
 #define RDATA  0x12 // Read data by command; supports multiple read back
  
 #define R_REG   0x20 // Read from regist
 #define W_REG   0x40 // Write to regist
  


//Adress registers
//Device Settings (Read-only Registers

  #define ID_ADDRESS 0x00 // ID

//Global Settings Across Channels

  #define CONFIG1_ADRESS 0x01 // CONFIG1
  #define CONFIG2_ADRESS 0x02 // CONFIG2
  #define CONFIG3_ADRESS 0x03 // CONFIG3
  #define LOFF_ADRESS    0x04 // LOFF

//CHanel-Specific Settings

  #define CH1SET_ADRESS  0x05 // CH1SET
  #define CH2SET_ADRESS  0x06 // CH2SET
  #define CH3SET_ADRESS  0x07 // CH3SET
  #define CH4SET_ADRESS  0x08 // CH4SET
  #define CH5SET_ADRESS  0x09 // CH5SET
  #define CH6SET_ADRESS  0x0A // CH6SET
  #define CH7SET_ADRESS  0x0B // CH7SET
  #define CH8SET_ADRESS  0x0C // CH8SET

  #define RLD_SENSP_ADRESS  0x0D // RLD_SENSP
  #define RLD_SENSN_ADRESS  0x0E // RLD_SENSN
  
  #define LOFF_SENSP_ADRESS 0x0F // LOFF_SENSP
  #define LOFF_SENSN_ADRESS 0x10 // LOFF_SENSN
  #define LOFF_FLIP_ADRESS  0x11 // LOFF_FLIP

//Lead-Off Status Registers (Read-Only Registers)

  #define LOFF_STATP_ADRESS 0x12 // LOFF_STATP
  #define LOFF_STATN_ADRESS 0x13 // LOFF_STATN

//GPIO and Others Registers
  
  #define GPIO_ADRESS    0x14 // GPIO
  #define PACE_ADRESS    0x15 // PACE
  #define RESP_ADRESS    0x16  // RESP
  #define CONFIG4_ADRESS 0x17  // CONFIG4
  #define WCT1_ADRESS    0x18  // WCT1
  #define WCT2_ADRESS    0x19  // WCT2

//----------------------------------------------------------
//Reset DATA value (HEX)

//Global Settings Across Channels

  #define CONFIG1_DATA 0x06 // CONFIG1
  #define CONFIG2_DATA 0x40 // CONFIG2
  #define CONFIG3_DATA 0xC0 // CONFIG3
  #define LOFF_DATA    0x00 // LOFF

//CHanel-Specific Settings

  #define CH1SET_DATA  0x00 // CH1SET
  #define CH2SET_DATA  0x00 // CH2SET
  #define CH3SET_DATA  0x00 // CH3SET
  #define CH4SET_DATA  0x00 // CH4SET
  #define CH5SET_DATA  0x00 // CH5SET
  #define CH6SET_DATA  0x00 // CH6SET
  #define CH7SET_DATA  0x00 // CH7SET
  #define CH8SET_DATA  0x00 // CH8SET

  #define RLD_SENSP_DATA  0x00 // RLD_SENSP
  #define RLD_SENSN_DATA  0x00 // RLD_SENSN
  
  #define LOFF_SENSP_DATA 0x00 // LOFF_SENSP
  #define LOFF_SENSN_DATA 0x00 // LOFF_SENSN
  #define LOFF_FLIP_DATA  0x00 // LOFF_FLIP

//Lead-Off Status Registers (Read-Only Registers)

  #define LOFF_STATP_DATA 0x00 // LOFF_STATP
  #define LOFF_STATN_DARA 0x00 // LOFF_STATN

//GPIO and Others Registers
  
  #define GPIO_DATA    0x0F // GPIO
  #define PACE_DATA    0x00 // PACE
  #define RESP_DATA    0x00 // RESP
  #define CONFIG4_DATA 0x00 // CONFIG4
  #define WCT1_DATA    0x00 // WCT1
  #define WCT2_DATA    0x00 // WCT2

//===================================================================================
  //#define TCLK       1      // Tclk = 420 ns
  //#define TPOR       32768  // 110 // Tpor = 2^18 * Tclk   
 
  #define TCSSC   1 // 6 ns
  #define TSCCS   2 // 4 TCLKs
  #define TPOR    110 // 2^16 TCLKs
  #define TRST    2 // 2 TCLKs 
  #define TRST2   16 // 18 TCLKs
  #define TINTREF 150000 // 150ms
  #define TCMD    2 // 4 TCLKs
  #define TCSH    1 // 2 TCLKs

//-------- Value of registers --------------------------------------------------

//-------- ID Register ---------------------------------------------------------

  #define ID_DATA_4 0x90 // 4-channel ADS129x
  #define ID_DATA_6 0x91 // 6-channel ADS129x
  #define ID_DATA_8 0x92 // 8-channel ADS129x

  #define ID_DATA_4R 0xD0 // 4-channel ADS129xR
  #define ID_DATA_6R 0xD1 // 6-channel ADS129xR
  #define ID_DATA_8R 0xD2 // 8-channel ADS129xR

//-------- CONFIG 1 ------------------------------------------------------------

  #define CONFIG1_const 0x00

  #define LP 0x00
  #define HR 0x80

  #define Deisy_chain 0x00
  #define Multiple_readback 0x40

  #define OSC_output_disable 0x00
  #define OSC_output_enable 0x20

  #define SPS_32K_HR_16K_LP  0x00
  #define SPS_16K_HR_8K_LP   0x01
  #define SPS_8K_HR_4K_LP    0x02
  #define SPS_4K_HR_2K_LP    0x03
  #define SPS_2K_HR_1K_LP    0x04
  #define SPS_1K_HR_500_LP   0x05
  #define SPS_500_HR_250_LP  0x06

//------------------------------------------------------------------------------

//-------- CONFIG 2 ------------------------------------------------------------

  #define CONFIG2_const     0x00

  #define WCT_CHOP_FrVaries 0x00
  #define WCT_CHOP_FrCONST  0x20

  #define INT_TEST_EXT      0x00 
  #define INT_TEST_INT      0x10

  #define TEST_SIGNAL_AMP_1 0x00
  #define TEST_SIGNAL_AMP_2 0x04

  #define TEST_FREQ_21      0x00
  #define TEST_FREQ_20      0x01
  #define TEST_FREQ_NOT_USE 0x02
  #define TEST_FREQ_AT_DC   0x03

//------------------------------------------------------------------------------

//-------- CONFIG 3 ------------------------------------------------------------

  #define CONFIG3_const 0x40

  #define PD_REFBUF_PWDN_INT 0x40
  #define PD_REFBUF_ENAB_INT 0xC0

  #define VREF_2V 0x40
  #define VREF_4V 0x60

  #define RLD_MEAS_OPEN 0x40
  #define RLD_MEAS_FOR_MUX_010 0x50

  #define RLDREF_EXT 0x40
  #define RLDREF_INT 0x48

  #define RLDBUF_DISABLE 0x40
  #define RLDBUF_ENABLE 0x44

  #define RLD_LOFF_SENS_DISABLE 0x40
  #define RLD_LOFF_SENS_ENABLE 0x42

//------------------------------------------------------------------------------

//-------- LEAD-OFF Control ----------------------------------------------------

  #define LEAD_OFF_Const 0x00

  #define COMP_TH_95_5    0x00
  #define COMP_TH_925_75  0x20
  #define COMP_TH_90_10   0x40
  #define COMP_TH_875_125 0x60
  #define COMP_TH_85_15   0x80
  #define COMP_TH_80_20   0xA0
  #define COMP_TH_75_25   0xC0
  #define COMP_TH_70_30   0xE0

  #define VLEAD_OFF_EN_CS          0x00
  #define VLEAD_OFF_EN_Pull_UP_DWN 0x10

  #define I_LEAD_OFF_6nA  0x00
  #define I_LEAD_OFF_12nA 0x40
  #define I_LEAD_OFF_18nA 0x80
  #define I_LEAD_OFF_24nA 0xC0

  #define F_LEAD_OFF         0x00
  #define F_LEAD_OFF_AC      0x01
  #define F_LEAD_OFF_NOT_USE 0x02
  #define F_LEAD_OFF_DC      0x03



//======== ADS1294_init.c ======================================================

      void Init_ADS1294 (SPI_HandleTypeDef * hspi1);
      void SetConfigurations (SPI_HandleTypeDef * hspi1);
      void PowerUP(SPI_HandleTypeDef * hspi1); 
      void PowerDOWN();
      
//========= ADS1294_RW_SendComand ==============================================
      
      uint8_t RREG(SPI_HandleTypeDef * hspi1,uint8_t  address);
      void WREG(SPI_HandleTypeDef * hspi1,uint8_t address, uint8_t data);
      void SendComand (SPI_HandleTypeDef * hspi1, uint8_t comand );
      
//========= ADS1294_ReadData_FormatAndRecive.c =================================   
      void ReadData (SPI_HandleTypeDef * hspi1, UART_HandleTypeDef * huart2);  

   
#endif