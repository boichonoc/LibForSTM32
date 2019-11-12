#include "sd_fatfs.h"
//----------------------- Local Varibales --------------------------------------

//------------------------------------------------------------------------------


//----------------------- Extern Varibales -------------------------------------
extern UART_HandleTypeDef huart1;
//------------------------------------------------------------------------------


//------------------- Static Functions Prototype -------------------------------

//------------------------------------------------------------------------------


//----------------------- Static Functions -------------------------------------

//------------------------------------------------------------------------------

//------------------------ Global Functions ------------------------------------
uint32_t GetReadFileSize( SD_FatFs_HandleTypeDef * SdFatFs, char * ReadFileName )
{
  uint32_t fSize = 0;
  if(f_mount(&SdFatFs->SDFatFs,(TCHAR const*)USERPath,0)!=FR_OK){
    return MOUNT_ERROR;
  } else {
    if(f_open(&SdFatFs->MyFile,(TCHAR const*)ReadFileName,FA_READ)!=FR_OK){
      return FILE_OPEN_ERROR;
    } else {
      fSize = SdFatFs->MyFile.fsize;
    }
    f_close(&SdFatFs->MyFile);
  } 
  return fSize;
}
SD_FatFs_StatusTypeDef SdFatFsReadFile( SD_FatFs_HandleTypeDef * SdFatFs, DWORD ofs, char * ReadFileName )
{
  uint32_t bytesReaded;
  if(f_mount(&SdFatFs->SDFatFs,(TCHAR const*)USERPath,0)!=FR_OK){
    HAL_UART_Transmit(&huart1, "MOUNT_ERROR \r\n", 14, 1);
    return MOUNT_ERROR;
  } else {
    HAL_UART_Transmit(&huart1, "MOUNT\r\n", 7, 1);
    if(f_open(&SdFatFs->MyFile,(TCHAR const*)ReadFileName,FA_READ)!=FR_OK){
      HAL_UART_Transmit(&huart1, "FILE_OPEN_ERROR \r\n", 18, 1);
      return FILE_OPEN_ERROR;
    } else {
      HAL_UART_Transmit(&huart1, "FILE_OPENED \r\n", 14, 1);
      f_lseek(&SdFatFs->MyFile,ofs);
      SdFatFs->result = f_read(&SdFatFs->MyFile,&SdFatFs->Buffer, 512,(UINT*)&bytesReaded);
      if( SdFatFs->result == FR_OK )
        f_close(&SdFatFs->MyFile);
      else
        return FILE_READ_ERROR;
    }
  }
  return FILE_READ_DONE;
}

SD_FatFs_StatusTypeDef SdFatFsWriteFile( SD_FatFs_HandleTypeDef * SdFatFs, DWORD ofs, char * WriteFileName )
{
  uint32_t bytesWritten;
  if(f_mount(&SdFatFs->SDFatFs,(TCHAR const*)USERPath,0)!=FR_OK){
    HAL_UART_Transmit(&huart1, "MOUNT_ERROR \r\n", 14, 1);
    return MOUNT_ERROR;
  } else {
    HAL_UART_Transmit(&huart1, "MOUNT\r\n", 7, 1);
    if(f_open(&SdFatFs->MyFile,(TCHAR const*)WriteFileName,FA_CREATE_ALWAYS|FA_WRITE)!=FR_OK){
      HAL_UART_Transmit(&huart1, "FILE_OPEN_ERROR \r\n", 18, 1);
      return FILE_OPEN_ERROR;
    } else {
      HAL_UART_Transmit(&huart1, "FILE_OPENED \r\n", 14, 1);
      f_lseek(&SdFatFs->MyFile,ofs);
      SdFatFs->result = f_write(&SdFatFs->MyFile,&SdFatFs->Buffer,512,(void*)&bytesWritten);
      if( (bytesWritten == 0)||(SdFatFs->result != FR_OK) )
        return FILE_WRITE_ERROR;
      else
        f_close(&SdFatFs->MyFile);
    }
  }
  return FILE_READ_DONE;
}
//------------------------------------------------------------------------------

