#ifndef __SSD1602
#define __SSD1602

#include "stm32f1xx_hal.h"

#define OLED_adress 0x78
#define OLED_SETCONTRAST 0x81
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF
#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10
#define OLED_SETSTARTLINE 0x40
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8
#define OLED_SEGREMAP 0xA0
#define OLED_CHARGEPUMP 0x8D
#define OLED_SWITCHCAPVCC 0x2
#define OLED_NOP 0xE3

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_BUFFERSIZE (OLED_WIDTH*OLED_HEIGHT)/8
#define OLED_DEFAULT_SPACE 5

#define COMAND 0x00
#define DATA 0x40

void sendCommand(I2C_HandleTypeDef *hi2c, uint8_t command_s);
void sendData(I2C_HandleTypeDef *hi2c, uint8_t data_s);
void LCD_Goto(I2C_HandleTypeDef *hi2c, unsigned char x, unsigned char y);
void LCD_Clear(I2C_HandleTypeDef *hi2c);
void LCD_Char(I2C_HandleTypeDef *hi2c, unsigned int c);
void OLED_string(I2C_HandleTypeDef *hi2c, char *string);
void OLED_init(I2C_HandleTypeDef *hi2c);


#endif