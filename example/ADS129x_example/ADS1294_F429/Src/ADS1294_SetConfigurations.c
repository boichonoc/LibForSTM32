#include "ADS1294.h"

 void SetConfigurations (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac){
      
//---- Set common CONFIGURATIONS ----------------------------------------------------------------------------------------------------------------
   
/* CONFIG 3 Register */   WREG(hspi1, CONFIG3_ADRESS, (CONFIG3_const | 
                                   PD_REFBUF_PWDN_INT | VREF_2V4 | RLD_MEAS_FOR_MUX_010 | RLDREF_INT | RLDBUF_ENABLE | RLD_LOFF_SENS_DISABLE));
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
    
   void ExternalREFvoltage (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac) {
     volatile static uint8_t CONFIG_3 = 0x00; 
     CONFIG_3 = RREG(hspi1, CONFIG3_ADRESS);
     if( (CONFIG_3 & 0x80) == 0x00 )
     {
      HAL_DAC_Start(hdac,DAC_CHANNEL_1);
      HAL_DAC_SetValue(hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,4095);
     }
    }