#include "ADS1294.h"

//======== Arrays ==============================================================
   
volatile uint8_t DataOut[27];
uint8_t packageCounter = 0;
//==============================================================================

    
 void ReadData (SPI_HandleTypeDef *  hspi1, UART_HandleTypeDef * huart2){
      for(int i =0; i<27; i++){ DataOut[i] = 0x00;}
      cs_Set();
      for( int i=0; i<12; i++){
        HAL_SPI_Transmit(hspi1, (uint8_t*)0x23, 1, 1);
        DataOut[i] = SPI1->DR;
      }
      cs_Reset();
      // -1 - 1/1
      // 0 - 1/2
      // 1 - 1/3
      // 2 - 1/4
      if(packageCounter > 0) {
       // every 10 packages
        HAL_UART_Transmit(huart2, (uint8_t*)DataOut, 12, 1);
          packageCounter = 0;
       } else {
        packageCounter++;
      }
     
    }
