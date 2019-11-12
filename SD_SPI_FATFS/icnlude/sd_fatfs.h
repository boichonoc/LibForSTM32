#ifndef SD_FATFS__H_

#define SD_FATFS_H_

//------------------------------------------------------------------------------
//------ Includes --------------------------------------------------------------
#include "fatfs.h"
//------ Private Defenitions ---------------------------------------------------

//------------------------------------------------------------------------------


//------ Private Structures ----------------------------------------------------
typedef enum {
  SD_DISK_INIT,
  MOUNT_ERROR,
  FILE_OPEN_ERROR,
  FILE_READ_DONE,
  FILE_READ_ERROR,
  FILE_WRITE_DONE,
  FILE_WRITE_ERROR
} SD_FatFs_StatusTypeDef;

typedef struct __SD_FATFS_HandleTypeDef{
  FATFS SDFatFs;
  FIL MyFile;
  FRESULT result;
  uint8_t Buffer[512];
}SD_FatFs_HandleTypeDef;
//------------------------------------------------------------------------------


//------ Extern varibales ------------------------------------------------------

//------------------------------------------------------------------------------


//------ Global Function prototype ---------------------------------------------
uint32_t GetReadFileSize( SD_FatFs_HandleTypeDef * SdFatFs, char * ReadFileName );
SD_FatFs_StatusTypeDef SdFatFsReadFile( SD_FatFs_HandleTypeDef * SdFatFs, DWORD ofs, char * ReadFileName ); 
SD_FatFs_StatusTypeDef SdFatFsWriteFile( SD_FatFs_HandleTypeDef * SdFatFs, DWORD ofs, char * WriteFileName );
//------------------------------------------------------------------------------

#endif 