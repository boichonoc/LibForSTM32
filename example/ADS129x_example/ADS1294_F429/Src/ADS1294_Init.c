#include "ADS1294.h"
#include "main.h"

//#include "stm32f1xx_hal_spi.h"    
    void Init_ADS1294 (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac) {
      volatile static uint8_t id = 0x00; // Для чтения ID регистра
      
      PowerDOWN();      // Ставим в ноль цифровые входы управления запуском
      HAL_Delay(1000); 
      PowerUP(hspi1);   // Подымаем микросхему с видержкой всех тайменгов (см. Даташит).
      
      SendComand(hspi1, STANDBY);  // После поднятия микросхемы нужно войти в в режим STANDBY, SDATAC и остановить 
      SendComand(hspi1, SDATAC);   // передачу данных, для установки нужных конфигураций микросхемы
      SendComand(hspi1, STOP);     //
      
      id = RREG(hspi1, ID_ADDRESS);                             // Проверку правильного запуска микросхемы выполнено через считывания 
      if((id == ID_DATA_4) || (id == ID_DATA_6) || (id == ID_DATA_8)){         // ID регистра. При правельном запуске, регистр считываеться, проверяеться 
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);  // на тип микросхемы после чего должен замигать световой индикатор
      HAL_Delay(100);
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
      }    
      
      SetConfigurations (hspi1,hdac);  // Установка Конфигураций микросхемы
      
      
      HAL_GPIO_WritePin(START_GPIO_Port, START_Pin,  GPIO_PIN_RESET); // После установки конфигураций, загоняем обратно микросхему в режим 
                      
      SendComand(hspi1, START);                                       // непрерывной регестрации даных со входов АЦП
      SendComand(hspi1, RDATAC); 
      SendComand(hspi1, WAKEUP);
            
    }
    
          
    void PowerUP (SPI_HandleTypeDef * hspi1){
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
    
    void PowerDOWN () {
      
      HAL_GPIO_WritePin(START_GPIO_Port, START_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PWDN_GPIO_Port,  PWDN_Pin,  GPIO_PIN_RESET);
      HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
    }
    
    
 
 