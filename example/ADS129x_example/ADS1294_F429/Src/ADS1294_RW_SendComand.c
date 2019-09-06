#include "ADS1294.h"

   uint8_t RREG(SPI_HandleTypeDef * hspi1,uint8_t  address) {
    
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
    
   void WREG(SPI_HandleTypeDef * hspi1, uint8_t address, uint8_t data) {
       
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

   void SendComand (SPI_HandleTypeDef * hspi1, uint8_t comand){
      
      cs_Set();
      HAL_Delay(1);
      
      HAL_SPI_Transmit( hspi1, &comand, 1, 1);
      (void)(SPI1->DR);
      HAL_Delay(1);
      
      HAL_Delay(2);
      cs_Reset();
      
      HAL_Delay(1);
    }
