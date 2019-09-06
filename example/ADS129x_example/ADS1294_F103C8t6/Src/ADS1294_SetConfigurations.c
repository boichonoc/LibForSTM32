#include "ADS1294.h"

 void SetConfigurations (SPI_HandleTypeDef * hspi1){
      
      WREG(hspi1, CONFIG3_ADRESS, (CONFIG3_const | 
                                   PD_REFBUF_ENAB_INT | VREF_2V | RLD_MEAS_OPEN | RLDREF_INT | RLDBUF_ENABLE | RLD_LOFF_SENS_DISABLE));
      
      WREG(hspi1, CONFIG1_ADRESS, (CONFIG1_const | 
                                   HR | Multiple_readback | OSC_output_disable | SPS_500_HR_250_LP) );
      
      WREG(hspi1, CONFIG2_ADRESS, (CONFIG2_const | 
                                   WCT_CHOP_FrVaries | INT_TEST_INT | TEST_SIGNAL_AMP_2 | TEST_FREQ_20));
      
      WREG(hspi1, LOFF_ADRESS, (LEAD_OFF_Const |
                                   COMP_TH_95_5 | VLEAD_OFF_EN_CS | I_LEAD_OFF_6nA | F_LEAD_OFF_AC));
      
      WREG(hspi1, CONFIG4_ADRESS, 0x02);
            
      WREG(hspi1, CH1SET_ADRESS, 0x60);
      WREG(hspi1, CH2SET_ADRESS, 0x60);
      WREG(hspi1, CH3SET_ADRESS, 0x60);
      WREG(hspi1, CH4SET_ADRESS, 0x60);
      WREG(hspi1, CH5SET_ADRESS, 0x60);
      WREG(hspi1, CH6SET_ADRESS, 0x60);
      WREG(hspi1, CH7SET_ADRESS, 0x60);
      WREG(hspi1, CH8SET_ADRESS, 0x62);

    }
    
   