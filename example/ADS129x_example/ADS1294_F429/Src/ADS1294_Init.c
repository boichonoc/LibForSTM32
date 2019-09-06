#include "ADS1294.h"
#include "main.h"

//#include "stm32f1xx_hal_spi.h"    
    void Init_ADS1294 (SPI_HandleTypeDef * hspi1, DAC_HandleTypeDef * hdac) {
      volatile static uint8_t id = 0x00; // ��� ������ ID ��������
      
      PowerDOWN();      // ������ � ���� �������� ����� ���������� ��������
      HAL_Delay(1000); 
      PowerUP(hspi1);   // �������� ���������� � ��������� ���� ��������� (��. �������).
      
      SendComand(hspi1, STANDBY);  // ����� �������� ���������� ����� ����� � � ����� STANDBY, SDATAC � ���������� 
      SendComand(hspi1, SDATAC);   // �������� ������, ��� ��������� ������ ������������ ����������
      SendComand(hspi1, STOP);     //
      
      id = RREG(hspi1, ID_ADDRESS);                             // �������� ����������� ������� ���������� ��������� ����� ���������� 
      if((id == ID_DATA_4) || (id == ID_DATA_6) || (id == ID_DATA_8)){         // ID ��������. ��� ���������� �������, ������� ������������, ������������ 
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);  // �� ��� ���������� ����� ���� ������ �������� �������� ���������
      HAL_Delay(100);
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
      }    
      
      SetConfigurations (hspi1,hdac);  // ��������� ������������ ����������
      
      
      HAL_GPIO_WritePin(START_GPIO_Port, START_Pin,  GPIO_PIN_RESET); // ����� ��������� ������������, �������� ������� ���������� � ����� 
                      
      SendComand(hspi1, START);                                       // ����������� ����������� ����� �� ������ ���
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
    
    
 
 