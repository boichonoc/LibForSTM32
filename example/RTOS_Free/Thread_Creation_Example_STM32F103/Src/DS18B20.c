#include "DS18B20.h"


__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
micros *= (SystemCoreClock / 1000000) / 9;
/* Wait till done */
while (micros--) ;
}

uint8_t ds18b20_Reset(void)
{
	uint16_t status;

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	DelayMicro(485);
        
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	DelayMicro(65);
         
        status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
        DelayMicro(500);
        
	return (status ? 1 : 0);

}

void ds18b20_WriteBit(uint8_t bit)
{       
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        DelayMicro( bit ? 3 : 65 );
  


        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        DelayMicro( bit ? 65 : 3 );
        
        
	
}

void ds18b20_WriteByte(uint8_t data)
{
	for( uint8_t i = 0; i < 8; i++)
	{
		ds18b20_WriteBit( data >> i & 1);
		DelayMicro(5);
                
	}
}

uint8_t ds18b20_ReadBit(void)
{
	uint8_t bit = 0;
        
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        DelayMicro(2);
        
	
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        DelayMicro(13);
        

        bit = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) ? 1 : 0);
        DelayMicro(45);
        
	
	return bit;

}

uint8_t ds18b20_ReadByte(void)
{
	uint8_t data = 0;

	for( uint8_t i = 0; i <= 7; i++ )
        data += ds18b20_ReadBit() << i;
	return data;
}


uint8_t ds18b20_init(uint8_t mode)
{
	
	if(ds18b20_Reset()) return 1;

	
	if(mode == SKIP_MODE)
	{
		ds18b20_WriteByte(0xCC);
		ds18b20_WriteByte(0x4E);
		ds18b20_WriteByte(0x64);
		ds18b20_WriteByte(0x9E);
		ds18b20_WriteByte(0x7F);
	}

	return 0;
}

void ds18b20_MeasureTemperCmd(uint8_t mode)
{
	ds18b20_Reset();
	if(mode == SKIP_MODE)
	{
		ds18b20_WriteByte(0xCC);
	}
	ds18b20_WriteByte(0x44);
}

void ds18b20_ReadStratcpad(uint8_t mode, uint8_t *Data)
{	uint8_t i;
	ds18b20_Reset();
	if(mode == SKIP_MODE)
	{
		ds18b20_WriteByte(0xCC);
	}
	ds18b20_WriteByte(0xBE);

	for( i = 0; i < 8; i++)
        {
          Data[i] = ds18b20_ReadByte();
        }
}

uint8_t ds18b20_GetSign(uint16_t dt)

{

  //�������� 11-� ���

  if (dt&(1<<11)) return 1;

  else return 0;

}

float ds18b20_Convert(uint16_t dt)

{

  float t;

  t = (float) ((dt&0x07FF)>>4); //��������� �������� � ������� ����

  //�������� ������� �����

  t += (float)(dt&0x000F) / 16.0f;

  return t;

}