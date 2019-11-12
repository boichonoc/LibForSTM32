#ifndef SD_SPI__H_

#define SD_SPI_H_

//------------------------------------------------------------------------------
//------ Includes --------------------------------------------------------------
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
//------ Private Defenitions ---------------------------------------------------
#define BUFFER_SIZE 512
/*
#define MMC 0x01                        // MMC ver 3 
#define SD1 0x02                        // SD ver 1 
#define SD2 0x04                        // SD ver 2 
#define SDC (CT_SD1|CT_SD2)             // SD 
#define BLOCK 0x08                      // Block addressing 

//--------------------------------------------------
// Definitions for MMC/SDC command
#define CMD0 (0x40+0) // GO_IDLE_STATE
#define CMD1 (0x40+1) // SEND_OP_COND (MMC)
#define ACMD41 (0xC0+41) // SEND_OP_COND (SDC)
#define CMD8 (0x40+8)   // SEND_IF_COND
#define CMD9 (0x40+9)   // SEND_CSD
#define CMD16 (0x40+16) // SET_BLOCKLEN
#define CMD17 (0x40+17) // READ_SINGLE_BLOCK
#define CMD24 (0x40+24) // WRITE_BLOCK
#define CMD55 (0x40+55) // APP_CMD
#define CMD58 (0x40+58) // READ_OCR
*/
//------------------------------------------------------------------------------
//------ Private Structures ----------------------------------------------------
typedef enum {
  UNDEFTYPE = 0x00,
  MMC = 0x01,
  SD1 = 0x02,
  SD2 = 0x04,
  SDC = (SD1|SD2),
  BLOCK = 0x08,
  SD2HC = (SD2|BLOCK)
} SD_KindTypeDef;

typedef enum {
  CMD0   = 0x40+0,
  CMD1   = 0x40+1,
  ACMD41 = 0xC0+41,
  CMD8   = 0x40+8,
  CMD9   = 0x40+9,
  CMD12  = 0x40+12,
  CMD16  = 0x40+16,
  CMD17  = 0x40+17,
  CMD18  = 0x40+18,
  CMD23  = 0x40+23,
  CMD24  = 0x40+24,
  CMD25  = 0x40+25,
  CMD55  = 0x40+55,
  CMD58  = 0x40+58
} SD_ComandTypeDef;

typedef enum {
  NO_ERRORS,
  WRONG_COMMAND,
  READ_BLOCK_ERROR,
  WRITE_BLOCK_ERROR
} SD_ErrorTypeDef;

typedef enum {
  BUSY,
  REDY,
  NOT_REDY
} SD_StatusTypeDef;

typedef struct __SD_HandleTypeDef{
  SD_KindTypeDef        SdType;
  uint32_t              SdBaudRate;
  uint8_t               OCR[4];
  SD_ErrorTypeDef       SdError;
  SD_StatusTypeDef      SdStatus;
}SD_HandleTypeDef;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------ Extern varibales ------------------------------------------------------
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
//------------------------------------------------------------------------------
//------ Global Function prototype ---------------------------------------------
SD_HandleTypeDef SdInit(uint32_t SdBaudRate);
uint8_t SdCardStatus(SD_HandleTypeDef * SdCard);
uint8_t SdSendComand(uint8_t comand, uint32_t argument);
uint8_t SdReadBlock(uint8_t *buff, uint16_t buffSize, uint32_t lba, uint8_t comand);
uint8_t SdWriteBlock (uint8_t *buff, uint32_t lba, uint8_t comand);
//------------------------------------------------------------------------------
#endif /* SD_H_ */