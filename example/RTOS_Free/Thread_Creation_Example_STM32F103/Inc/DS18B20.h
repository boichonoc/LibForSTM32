/*
 * DS18B20.h
 *
 *  Created on: Jan 6, 2019
 *      Author: pi
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>      

#define DS18B20_DATA_PIN RPI_GPIO_P1_07

//MODE
#define    SKIP_MODE	0
#define NO_SKIP_MODE    1

//Resolution Configuration
#define  RES_9_BIT 0x1F
#define RES_10_BIT 0x3F
#define RES_11_BIT 0x5F
#define RES_12_BIT 0x7F

// ROM Commands
#define   SEARCH_ROM 0xF0 //Search Rom
#define     READ_ROM 0x33 //Read   Rom
#define    MATCH_ROM 0x55 //Search Rom
#define     SKIP_ROM 0xCC //Skip   Rom
#define ALARM_SEARCH 0xEC //Alarm Search

// DS18B20 Function Commands

// Temperature conversion commands Table 3 in datasheets
#define 	   CONVERT_T 0x44

// Memory comands
#define	WRITE_SCRATCHPAD 0x4E
#define  READ_SCRATCHPAD 0xBE
#define  COPY_SCRATCHPAD 0x48
#define       RECALL_E_2 0xB8
#define  READ_POWER_SUPP 0xB4

uint8_t ds18b20_init(uint8_t mode);

void ds18b20_MeasureTemperCmd(uint8_t mode);
void ds18b20_ReadStratcpad(uint8_t mode, uint8_t *Data);

uint8_t ds18b20_GetSign(uint16_t dt);
float ds18b20_Convert(uint16_t dt);

#endif /* DS18B20_H_ */
