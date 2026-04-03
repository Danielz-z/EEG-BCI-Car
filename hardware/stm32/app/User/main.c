#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "NeZha.h"
#include "sys.h"
#include "ps2.h"
#include "Usart.h"
#include "RobotArm.h"
#include "Board_Timer.h"
#include "HC05.h"
#include "string.h"

#define   UNIT_PWM	1				//Servo unit rotation value
#define   PS2_LSPEED    1000		//Pushing the left joystick all the way down corresponds to the maximum target value of the motor (the maximum speed of the motor is 1000)
#define   PS2_RSPEED    800			//Pushing the right joystick all the way down corresponds to the maximum target value of the motor (the maximum speed of the motor is 800)©
#define   RECIPROCAL	0.0078f     //The reciprocal of 128. This is for the convenience of calculating the mapping from the joystick value to the target value. It cannot be modified.

uint8_t RxSTA = 1;
char RxData[100] = "None";
char adva[100] = "adva";
char righ[100] = "righ";
char left[100] = "left";
char slow[100] = "slow";
char stop[100] = "stop";


//int16_t M1_Target, M2_Target, M3_Target, M4_Target;		//Four motor encoder target value variables
uint8_t Time;	//
uint8_t Led_State;  //Vehicle status flag bits; 1: Brake, 2: Left turn, 3: Right turn, 4: Stop, 5: Forward∆
uint8_t TD = 0;
int main(void)
{
 	__disable_irq(); 		//Disable all interrupts of 32 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//Interrupt priority setting
	
	LED_Init();	   			//Initialization of the 32 main control blue indicator light
	NeZha_Init(); 			
	NeZha_Motor_Init();		//Nezha driver board hardware initialization
	Board_Timer_Init();     //Development board timing timer initialization, with an interrupt period of 5ms
	
	HC05_Init();
	
	__enable_irq();			//Enable all interrupts of 32  
	while (1)
	{
		if(TD==0)
		{
		if (Board_Timer_Flag_Get())    //Timer with a 5ms timing period
		{
			Time++;
      HC05_GetData(RxData);
			if ((Time + 1)%2 == 0)		//Remote control cycle of 10ms
			{
						//Status judgment
						if(!strcmp(RxData,slow))    //Brake
						{
						  Led_State = 1;
						}
						else if (!strcmp(RxData,left))  //Left turn
						{
						  
							Led_State = 2;
						}
						 else if(!strcmp(RxData,righ)) //Right turn
						  {
							 Led_State = 3;
						  }
						else if(!strcmp(RxData,stop))  //Stop
						  {
							 Led_State = 4;
						  }
						else if(!strcmp(RxData,adva))  //Forward
						  {
							 Led_State = 5;
						  }
				}
											
					
										
			}		
			if ((Time + 1)%200 == 0)      //Motor control
			{
				if(Led_State == 5)
				{
					NeZha_Motor1_SetPwm(500,0);
					NeZha_Motor2_SetPwm(0,500);
					NeZha_Motor3_SetPwm(0,500);
					NeZha_Motor4_SetPwm(500,0);
				}
				else if(Led_State == 4)
				{
					NeZha_Motor1_SetPwm(0,0);
					NeZha_Motor2_SetPwm(0,0);
					NeZha_Motor3_SetPwm(0,0);
					NeZha_Motor4_SetPwm(0,0);
				}
				else if(Led_State == 1)
				{				
					TD=1;
					int i=0;
					if(i<4)
					{
						RxData[i] = adva[i];
						i++;
					}
					NeZha_Motor1_SetPwm(100,0);
					NeZha_Motor2_SetPwm(0,100);
					NeZha_Motor3_SetPwm(0,100);
					NeZha_Motor4_SetPwm(100,0);
					Delay_s(1);
					NeZha_Motor1_SetPwm(500,0);
					NeZha_Motor2_SetPwm(0,500);
					NeZha_Motor3_SetPwm(0,500);
					NeZha_Motor4_SetPwm(500,0);
					Led_State =5;
					TD=0;
				}
				else if(Led_State == 3)
				{
					TD=1;
					int i=0;
					if(i<4)
					{
						RxData[i] = adva[i];
						i++;
					}
					NeZha_Motor1_SetPwm(500,0);
					NeZha_Motor2_SetPwm(500,0);
					NeZha_Motor3_SetPwm(500,0);
					NeZha_Motor4_SetPwm(500,0);
					Delay_s(1);
					NeZha_Motor1_SetPwm(500,0);
					NeZha_Motor2_SetPwm(0,500);
					NeZha_Motor3_SetPwm(0,500);
					NeZha_Motor4_SetPwm(500,0);
					Delay_s(1);
					NeZha_Motor1_SetPwm(0,500);
					NeZha_Motor2_SetPwm(0,500);
					NeZha_Motor3_SetPwm(0,500);
					NeZha_Motor4_SetPwm(0,500);
					Delay_s(1);
					NeZha_Motor1_SetPwm(500,0);
					NeZha_Motor2_SetPwm(0,500);
					NeZha_Motor3_SetPwm(0,500);
					NeZha_Motor4_SetPwm(500,0);
					Led_State =5;
					TD=0;
				}
				else if(Led_State == 2)
				{
					TD=1;
					int i=0;
					if(i<4)
					{
						RxData[i] = adva[i];
						i++;
					}
					NeZha_Motor1_SetPwm(0,500);
					NeZha_Motor2_SetPwm(0,500);
					NeZha_Motor3_SetPwm(0,500);
					NeZha_Motor4_SetPwm(0,500);
					Delay_s(1);
					NeZha_Motor1_SetPwm(500,0);
					NeZha_Motor2_SetPwm(0,500);
					NeZha_Motor3_SetPwm(0,500);
					NeZha_Motor4_SetPwm(500,0);
					Delay_s(1);
					NeZha_Motor1_SetPwm(500,0);
					NeZha_Motor2_SetPwm(500,0);
					NeZha_Motor3_SetPwm(500,0);
					NeZha_Motor4_SetPwm(500,0);
					Delay_s(1);
					NeZha_Motor1_SetPwm(500,0);
					NeZha_Motor2_SetPwm(0,500);
					NeZha_Motor3_SetPwm(0,500);
					NeZha_Motor4_SetPwm(500,0);
					Led_State =5;
					TD=0;
				}
			}
			if ((Time + 1)%50 == 0)   //Tail light control  250ms
			{
				switch(Led_State)
				{
				  case 0:{
					NeZha_TailLeftLed_TurnOff();
					NeZha_TailRightLed_TurnOff();
				  }break;
				  case 1:{
					NeZha_TailLeftLed_TurnOn();
					NeZha_TailRightLed_TurnOn();
				  }break;
				  case 2:{
					NeZha_TailLeftLed_Turn();
					NeZha_TailRightLed_TurnOff();
				  }break;     
				  case 3:{
					NeZha_TailLeftLed_TurnOff();
					NeZha_TailRightLed_Turn();
				  }break;  
				  default:{
					NeZha_TailLeftLed_TurnOff();
					NeZha_TailRightLed_TurnOff();
				  }break;     
				}
			}			
			if((Time + 1)%200 == 0)		//indicator light  1s
			{
				LED1_Turn();
				Time = 0;
			}
		}	
	}
	}

	
