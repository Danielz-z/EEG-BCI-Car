#include "ps2.h"
#include "stm32f10x.h"                  // Device header
/*********************************************************     
**********************************************************/	 
unsigned int Handkey;	// 按键值读取，零时存储。
//uint8_t Key_num;
uint8_t ps2_mode;
uint8_t Comd[9]={0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	//开始命令。请求数据
uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
uint8_t key_search[12] = {0x00,0x00,0x00,0x00,
						  0x00,0x00,0x00,0x00,
						  0x00,0x00,0x00,0x00};

unsigned int MASK[16][2]={
    {PSB_SELECT,0},
    {PSB_L3,0},
    {PSB_R3,0},
    {PSB_START,0},
    {PSB_PAD_UP,0},
    {PSB_PAD_RIGHT,0},
    {PSB_PAD_DOWN,0},
    {PSB_PAD_LEFT,0},
    {PSB_L2,0},
    {PSB_R2,0},
    {PSB_L1,0},
    {PSB_R1 ,0},
    {PSB_GREEN,0},
    {PSB_RED,0},
    {PSB_BLUE,0},
    {PSB_PINK,0}
	};	//按键值与按键明

void PS2_Init(void)
{ 	
	PS2_GPIO_Init();
	
	PS2_SCK_H();
	PS2_CS_H();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//进入配置模式
	PS2_TurnOnAnalogMode();	//“红绿灯”配置模式，并选择是否保存
	//PS2_VibrationMode();	//开启震动模式
	PS2_ExitConfing();		//完成并保存配置 
	ps2_mode_get();
}

//向手柄发送命令
unsigned char PS2_Cmd(unsigned char CMD)
{    
	unsigned char i,j=1;
	unsigned char res=0; 
	for(i=0;i<=7;i++)     //逐位接收     
	{
		 if(CMD&0x01)
			PS2_DO_H();  
		 else
			PS2_DO_L();  
		 
		 CMD=CMD>>1;
		 
		 PS2_Delay_US(100);
		 PS2_SCK_L();   	
		 PS2_Delay_US(100);
		 
		 if(PS2_DI()) 
		 {
			res=res+j;
		 }
		 j=j<<1;
		
		 PS2_SCK_H();   
		 PS2_Delay_US(1);		 
	}

	return res;	
}

//读取手柄数据
void PS2_ReadData(void)
{
	unsigned char byte=0;
	PS2_CS_L();
    PS2_Delay_US(10);

    for(byte = 0; byte < 9; byte++)
    {
        if(byte < 2)
        {
            Data[byte] = PS2_Cmd(Comd[byte]);
        }
        else
        {
            Data[byte] = PS2_Cmd(0x00);
        }
    }
	PS2_CS_H();
	PS2_Delay_US(10);
}

//判断是否为红灯模式,0x41=模拟绿灯，0x73=模拟红灯
//返回值；0，红灯模式
//		  其他，其他模式
unsigned char ps2_mode_get(void)
{	
	if( Data[1] == 0X73)  
	{
		ps2_mode = PSB_REDLIGHT_MODE;
	}
	else if ( Data[1] == 0X41)
	{
		ps2_mode = PSB_GREENLIGHT_MODE;
	}
	else
	{
		ps2_mode = PSB_LOSE;
	}
	return ps2_mode;
}

//对读出来的PS2的数据进行处理,只处理按键部分  
//只有一个按键按下时按下为0， 未按下为1
unsigned char ps2_key_serch()
{
	unsigned char index;
	unsigned char key_num = 0;
	//PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=4;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index][0]-1)))==0)
		{
			MASK[index][1] = 1; 
			key_num++;
		}
		else
		{
			MASK[index][1] = 0; 
		}
	}
	return key_num;
}

unsigned char ps2_get_key_state(unsigned char key_id)
{
	if (key_id < PSB_SELECT)
		return 0;
	else
		return MASK[key_id - 1][1];
}

//得到一个摇杆的模拟量	 范围0~256
unsigned char ps2_get_anolog_data(unsigned char button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	unsigned char a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

/******************************************************
Function:    void PS2_Vibration(unsigned char motor1, unsigned char motor2)
Description: 手柄震动函数，
Calls:		 void PS2_Cmd(unsigned char CMD);
Input: motor1:右侧小震动电机 0x00关，其他开
	   motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
******************************************************/
void PS2_Vibration(unsigned char motor1, unsigned char motor2)
{
	PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  //开始命令
	PS2_Cmd(0x42);  //请求数据
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_CS_H();
	PS2_Delay_US(100);
}

//short poll
void PS2_ShortPoll(void)
{
	PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	PS2_CS_H();
	PS2_Delay_US(100);	
}

//进入配置
void PS2_EnterConfing(void)
{
    PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_CS_H();
	PS2_Delay_US(100);
}

//发送模式设置
void PS2_TurnOnAnalogMode(void)
{
	PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  软件设置发送模式
	PS2_Cmd(0xEE); //Ox03锁存设置，即不可通过按键“MODE”设置模式。
				   //0xEE不锁存软件设置，可通过按键“MODE”设置模式。
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_CS_H();
	PS2_Delay_US(100);
}

//振动设置
void PS2_VibrationMode(void)
{
	PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	PS2_CS_H();
	PS2_Delay_US(100);	
}

//完成并保存配置
void PS2_ExitConfing(void)
{
    PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_CS_H();
	PS2_Delay_US(100);
}

void PS2_GPIO_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void PS2_CS_H()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

void PS2_CS_L()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

void PS2_SCK_H()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

void PS2_SCK_L()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}

void PS2_DO_H()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
}

void PS2_DO_L()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}

uint8_t PS2_DI()
{
	uint8_t di = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
	return di;
}

void PS2_Delay_US(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
