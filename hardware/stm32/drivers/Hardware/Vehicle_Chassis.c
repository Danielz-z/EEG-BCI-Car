#include "stm32f10x.h"                  // Device header
#include "Vehicle_chassis.h"
#include "NeZha.h"
#include "type_servo.h"

/*****************************************************************
舵机结构体：
*	1.舵机基本参数，从左到右对应舵机当前pwm值，左极限值，中值，右极限值
*	2.该值代表当前PWM高电平时间，例  150 = 1.5ms，对应舵机角度90°
******************************************************************/
struct SERVO_PARAM arc_servo = {150, 120, 150, 190};

void Vehicle_Chassis_Init()
{
	NeZha_Motor_Init();
}

void Arc_Servo_Init()
{
	NeZha_Servo1_Init();
	Arc_Servo_Reset();
}
/**
  * @brief  底盘阿克曼舵机复位
  * @param 	无
  * @retval 无
  */
void Arc_Servo_Reset()
{
	NeZha_Servo1_SetPwm(arc_servo.middle);
}

/**
  * @brief  底盘阿克曼舵机角度设置
  * @param 	value：舵机目标角度的pwm高电平持续时间，范围见结构体参数
  * @retval 无
  */
void Arc_ServoPwm_Set(uint16_t value)
{
	//限幅
	if (value < arc_servo.left_limit)
	{
		value = arc_servo.left_limit;
	}
	else if (value > arc_servo.right_limit)
	{
		value = arc_servo.right_limit;
	}
	
	arc_servo.pwm = value;
	NeZha_Servo1_SetPwm(arc_servo.pwm);
}

/**
  * @brief  底盘阿克曼舵机左打角
  * @param 	unit_pwm，舵机单位转动角度
  * @retval 无
  */
void Arc_ServoPwm_TurnLeft(uint8_t unit_pwm)
{
	unit_pwm = arc_servo.pwm - unit_pwm;
	arc_servo.pwm = unit_pwm;
	NeZha_Servo1_SetPwm(unit_pwm);
}

/**
  * @brief  底盘阿克曼舵机右打角
  * @param 	unit_pwm，舵机单位转动角度
  * @retval 无
  */
void Arc_ServoPwm_TurnRight(uint8_t unit_pwm)
{
	unit_pwm = arc_servo.pwm + unit_pwm;
	arc_servo.pwm = unit_pwm;
	NeZha_Servo1_SetPwm(unit_pwm);
}

void Vehicle_Chassis_Forward(uint16_t Speed)
{
	NeZha_Motor1_SetPwm(Speed,0);
	NeZha_Motor2_SetPwm(0,Speed);
	NeZha_Motor3_SetPwm(0,Speed);
	NeZha_Motor4_SetPwm(Speed,0);
}

void Vehicle_Chassis_Backward(uint16_t Speed)
{
	NeZha_Motor1_SetPwm(0,Speed);
	NeZha_Motor2_SetPwm(Speed,0);
	NeZha_Motor3_SetPwm(Speed,0);
	NeZha_Motor4_SetPwm(0,Speed);
}

void Vehicle_Chassis_TurnLeft(uint16_t Speed)
{
	NeZha_Motor1_SetPwm(0,Speed);
	NeZha_Motor2_SetPwm(0,Speed);
	NeZha_Motor3_SetPwm(0,Speed);
	NeZha_Motor4_SetPwm(0,Speed);
}

void Vehicle_Chassis_TurnRight(uint16_t Speed)
{
	NeZha_Motor1_SetPwm(Speed,0);
	NeZha_Motor2_SetPwm(Speed,0);
	NeZha_Motor3_SetPwm(Speed,0);
	NeZha_Motor4_SetPwm(Speed,0);
}

void Vehicle_Chassis_TransLeft(uint16_t Speed)
{
	NeZha_Motor1_SetPwm(Speed,0);
	NeZha_Motor2_SetPwm(Speed,0);
	NeZha_Motor3_SetPwm(0,Speed);
	NeZha_Motor4_SetPwm(0,Speed);
}

void Vehicle_Chassis_TransRight(uint16_t Speed)
{
	NeZha_Motor1_SetPwm(0,Speed);
	NeZha_Motor2_SetPwm(0,Speed);
	NeZha_Motor3_SetPwm(Speed,0);
	NeZha_Motor4_SetPwm(Speed,0);
}
