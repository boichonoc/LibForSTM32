#ifndef LCD
#define LCD

#define DATA_INPUT      HAL_GPIO_WritePin(RS_LCD_GPIO_Port,RS_LCD_Pin, GPIO_PIN_SET);    // RS_LCD_Pin is hight - select DATA mode
#define INSTRUCT_INPUT  HAL_GPIO_WritePin(RS_LCD_GPIO_Port,RS_LCD_Pin, GPIO_PIN_RESET);  // RS_LCD_Pin is low - select INSTRUCTIONs mode

#define RW_READ         HAL_GPIO_WritePin(RW_LCD_GPIO_Port,RW_LCD_Pin, GPIO_PIN_SET);    // RW_LCD_Pin is hight - select READ data\instruct mode
#define RW_WRITE        HAL_GPIO_WritePin(RW_LCD_GPIO_Port,RW_LCD_Pin, GPIO_PIN_RESET);  // RW_LCD_Pin is low - select WRITE data\instruct mode

#define ENABLE          HAL_GPIO_WritePin(E_LCD_GPIO_Port,E_LCD_Pin, GPIO_PIN_SET);      // Enable LCD
#define DISABLE         HAL_GPIO_WritePin(E_LCD_GPIO_Port,E_LCD_Pin, GPIO_PIN_RESET);    // Disable LCD

#define D0_0            HAL_GPIO_WritePin(D0_LCD_GPIO_Port,D0_LCD_Pin, GPIO_PIN_RESET);
#define D0_1            HAL_GPIO_WritePin(D0_LCD_GPIO_Port,D0_LCD_Pin, GPIO_PIN_SET);

#define D1_0            HAL_GPIO_WritePin(D1_LCD_GPIO_Port,D1_LCD_Pin, GPIO_PIN_RESET);
#define D1_1            HAL_GPIO_WritePin(D1_LCD_GPIO_Port,D1_LCD_Pin, GPIO_PIN_SET);

#define D2_0            HAL_GPIO_WritePin(D2_LCD_GPIO_Port,D2_LCD_Pin, GPIO_PIN_RESET);
#define D2_1            HAL_GPIO_WritePin(D2_LCD_GPIO_Port,D2_LCD_Pin, GPIO_PIN_SET);

#define D3_0            HAL_GPIO_WritePin(D3_LCD_GPIO_Port,D3_LCD_Pin, GPIO_PIN_RESET);
#define D3_1            HAL_GPIO_WritePin(D3_LCD_GPIO_Port,D3_LCD_Pin, GPIO_PIN_SET);

#define D4_0            HAL_GPIO_WritePin(D4_LCD_GPIO_Port,D4_LCD_Pin, GPIO_PIN_RESET);
#define D4_1            HAL_GPIO_WritePin(D4_LCD_GPIO_Port,D4_LCD_Pin, GPIO_PIN_SET);

#define D5_0            HAL_GPIO_WritePin(D5_LCD_GPIO_Port,D5_LCD_Pin, GPIO_PIN_RESET);
#define D5_1            HAL_GPIO_WritePin(D5_LCD_GPIO_Port,D5_LCD_Pin, GPIO_PIN_SET);

#define D5_0            HAL_GPIO_WritePin(D5_LCD_GPIO_Port,D5_LCD_Pin, GPIO_PIN_RESET);
#define D5_1            HAL_GPIO_WritePin(D5_LCD_GPIO_Port,D5_LCD_Pin, GPIO_PIN_SET);

#define D6_0            HAL_GPIO_WritePin(D6_LCD_GPIO_Port,D6_LCD_Pin, GPIO_PIN_RESET);
#define D6_1            HAL_GPIO_WritePin(D6_LCD_GPIO_Port,D6_LCD_Pin, GPIO_PIN_SET);

#define D7_0            HAL_GPIO_WritePin(D7_LCD_GPIO_Port,D7_LCD_Pin, GPIO_PIN_RESET);
#define D7_1            HAL_GPIO_WritePin(D7_LCD_GPIO_Port,D7_LCD_Pin, GPIO_PIN_SET);
//------- 8-bit Mode -----------------------------------------------------------
void LCD_Init();
void LCD_Clean();
void LCD_SendChar( char ch );
void LCD_String( char* st );
void LCD_SetPos(uint8_t c, uint8_t r);
//------- 4-bit Mode -----------------------------------------------------------
void LCD_Init_4bit();
void LCD_Clean_4bit();
void LCD_SendChar_4bit( char ch );
void LCD_String_4bit( char* st );
void LCD_SetPos_4bit(uint8_t c, uint8_t r);

void Send_Byte_4bit(uint8_t dt);
#endif
