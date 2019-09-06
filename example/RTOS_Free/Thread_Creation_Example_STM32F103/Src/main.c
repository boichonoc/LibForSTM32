/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "SSD1602.h"
#include "DS18B20.h"
#include "stdio.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
osThreadId ledTask, printTask, tempTask, rtcTask;
char count[20];
char heapSizeFree[20];

RTC_TimeTypeDef Time;
RTC_DateTypeDef Date;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void LedTaskToggle();
void PrintOLED();
void LedTaskToggle2();
void TempMesure(void const * argument);
void RTC_Task();
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();

  /* USER CODE BEGIN 2 */
  HAL_Delay(100);
  OLED_init(&hi2c1);
  LCD_Clear(&hi2c1);
  ds18b20_init(SKIP_MODE);
  

  
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  
  osThreadDef(toggleLedTask, LedTaskToggle, osPriorityIdle, 0, configMINIMAL_STACK_SIZE);
  ledTask = osThreadCreate(osThread(toggleLedTask),NULL);
    
  osThreadDef(printOLED, PrintOLED, osPriorityIdle, 0, configMINIMAL_STACK_SIZE);
  printTask = osThreadCreate(osThread(printOLED),NULL);
  
  osThreadDef(temperTask, TempMesure, osPriorityIdle, 0, configMINIMAL_STACK_SIZE);
  tempTask = osThreadCreate(osThread(temperTask),NULL);
  
  osThreadDef(rtcTask, RTC_Task, osPriorityIdle, 0, configMINIMAL_STACK_SIZE);
  rtcTask = osThreadCreate(osThread(rtcTask),NULL);

  
  
  
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef DateToUpdate;

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initialize RTC and set the Time and Date 
    */
  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2){
  sTime.Hours = 0x4;
  sTime.Minutes = 0x55;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  DateToUpdate.WeekDay = RTC_WEEKDAY_TUESDAY;
  DateToUpdate.Month = RTC_MONTH_MARCH;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0x32F2);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void PrintOLED(void const * argument)
{
  for(int i = 0; i < 20; i++)
    {
      HAL_Delay(500);
      sprintf(count,"%d ", i);
      LCD_Goto(&hi2c1, 0,4);
      OLED_string(&hi2c1," Task 2: ");
      LCD_Goto(&hi2c1, 50,4);
      OLED_string(&hi2c1,count);    
    }
  osThreadDef(toggleLedTask, LedTaskToggle2, osPriorityIdle, 0, configMINIMAL_STACK_SIZE);
  ledTask = osThreadCreate(osThread(toggleLedTask),NULL);
  
  sprintf(heapSizeFree,"%d", xPortGetFreeHeapSize());
  LCD_Goto(&hi2c1, 70,5);
  OLED_string(&hi2c1,heapSizeFree);
  
  LCD_Goto(&hi2c1, 0,5);
  OLED_string(&hi2c1," Free Size:");
  osThreadSetPriority(ledTask, osPriorityIdle);
  
  osThreadTerminate(printTask);
}
void LedTaskToggle(void const * argument)
{  
  for(int i = 0; i < 25; i++)
    {
      HAL_Delay(200);
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      osDelay(10);
      LCD_Goto(&hi2c1, 0, 3);
      OLED_string(&hi2c1, " Task 1:");
      sprintf(count, "%d", i);
      LCD_Goto(&hi2c1, 50, 3);
      OLED_string(&hi2c1, count);
    }
  sprintf(heapSizeFree,"%d", xPortGetFreeHeapSize());
  LCD_Goto(&hi2c1, 70,5);
  OLED_string(&hi2c1,heapSizeFree);
    
  LCD_Goto(&hi2c1, 0,5);
  OLED_string(&hi2c1," Free Size:");
  
  LCD_Goto(&hi2c1, 50, 3);
  OLED_string(&hi2c1, "  ");
  
  osThreadTerminate(ledTask);
  //vTaskDelete(ledTask);
}
void LedTaskToggle2(void const * argument)
{    
  for(int i = 0; i < 12; i++)
    {
      HAL_Delay(417);
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      osDelay(10);
      LCD_Goto(&hi2c1, 0, 3);
      OLED_string(&hi2c1, " Task 1:");
      sprintf(count, "%d", i);
      LCD_Goto(&hi2c1, 50, 3);
      OLED_string(&hi2c1, count);
    }
  osThreadDef(printOLED, PrintOLED, osPriorityIdle, 0, configMINIMAL_STACK_SIZE);
  printTask = osThreadCreate(osThread(printOLED),NULL);
  
  sprintf(heapSizeFree,"%d", xPortGetFreeHeapSize());
  LCD_Goto(&hi2c1, 70,5);
  OLED_string(&hi2c1,heapSizeFree);
  
  LCD_Goto(&hi2c1, 0,5);
  OLED_string(&hi2c1," Free Size:");
  
  LCD_Goto(&hi2c1, 50, 3);
  OLED_string(&hi2c1, "  ");
    
  osThreadTerminate(ledTask);
}

void TempMesure(void const * argument)
{       
  uint8_t dt[8];
  uint16_t raw_temper;
  float temper;
  char signe;
  char printTemp[8];
  
  for(;;)
  {
    ds18b20_MeasureTemperCmd(SKIP_MODE);
    HAL_Delay(800);
    ds18b20_ReadStratcpad(SKIP_MODE, dt);
    raw_temper = ((uint16_t)dt[1]<<8)|dt[0];
    if(ds18b20_GetSign(raw_temper)) signe = '+';
    else signe = '-';
    temper = ds18b20_Convert(raw_temper);
   
    sprintf(printTemp, " Temp:%c%.1fC", signe, temper);
    LCD_Goto(&hi2c1, 0,7);
    OLED_string(&hi2c1, printTemp);
    osDelay(500);
   }
  
}

void RTC_Task()
{
  char clock[8];
  char date[10];

  Time.Hours = 0x13;
  Time.Minutes = 0xC;
  Time.Seconds = 0x00;
  HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
  
  Date.Year = 0x13;
  Date.Month = RTC_MONTH_MARCH;
  Date.Date = 0x05;
  HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN);
  
  for(;;)
  {
    osDelay(125);
    /**/
    HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    sprintf(clock, "Time %.2d:%.2d:%.2d", Time.Hours, Time.Minutes, Time.Seconds); 
    LCD_Goto(&hi2c1, 49,0);
    OLED_string(&hi2c1, clock);    
    osDelay(250);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);
    sprintf(date, "Date %.2d:%.2d:%.2d", Date.Date, Date.Month, Date.Year);
    LCD_Goto(&hi2c1, 50,1);
    OLED_string(&hi2c1, date); 
  }  
}
/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */

    
  /* Infinite loop */
  for(;;)
  {

  }
  /* USER CODE END 5 */ 
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
