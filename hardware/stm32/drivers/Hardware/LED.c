#include "stm32f10x.h"
#include "sys.h"

void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void LED1_ON()
{
	PCout(13) = 0;
}

void LED1_OFF()
{
	PCout(13) = 1;	
}

void LED1_Turn()
{
	if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13)==0)
	{
		PCout(13) = 1;
	}
	else
	{
		PCout(13) = 0;
	}
}

