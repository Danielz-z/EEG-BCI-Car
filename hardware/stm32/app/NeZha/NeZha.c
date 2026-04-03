/******************************************************************
*  	有方机器人：STM32F103版-NeZha(哪吒)驱动板库V1.0
*	有关驱动板的详细使用请见使用手册
*	官方指定购买渠道：有方机器人（淘宝店铺）
*	官方淘宝地址：https://shop479988600.taobao.com/
*
*	NeZha(哪吒)驱动板库共4个文件：
*	1. NeZha.c   	&	NeZhe.h			
*		功能源代码文件：这两个文件内定义了驱动板的功能函数，
*						用户可调用这些函数控制驱动板驱动直流电机以及舵机
*	2. NeZha_I2C.c	&	NeZha_I2C.h 	
*		I2C接口硬件配置文件：这两个文件内是关于I2C总线硬件的配置
*
*	移植NeZha(哪吒)驱动板库步骤说明：
*   1. 修改NeZha_I2C.c 文件中关于I2C的引脚配置
*		1.1 修改 NeZha_I2C_Init()函数中的配置，修改SCL、SDA引脚为自定义的引脚
*		1.2 修改以下三个宏定义中的引脚为自定义引脚：
*				#define NeZha_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
*				#define NeZha_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))
*				#define NeZha_SDA_In		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
*		1.3 修改 NeZha_I2C_SdaDir()函数中关于SDA引脚的配置
******************************************************************/

#include "stm32f10x.h"
#include "NeZha.h"
#include "NeZha_I2c.h"

/**
  * @brief  NeZhen驱动板初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Init()
{
	NeZha_I2C_Init();
	NeZha_Reset();
}

/**
  * @brief  NeZhen驱动板复位
  * @param 	无
  * @retval 无
  */
void NeZha_Reset()
{
	NeZha_WriteCommand(NEZHA_CMD_RESET);
	NeZha_Delay_ms(100);	//上电延时,不可去掉!!!
}

/**
  * @brief  NeZha驱动板电机初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Motor_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_MOTOR_INIT);
}

/**
  * @brief  NeZha驱动板电机1的PWM设置
  * @param 	motor_a：电机反转，范围0-1000
			motor_b：电机正转，范围0-1000
  * @retval 无
  */
void NeZha_Motor1_SetPwm(uint16_t motor_a,uint16_t motor_b)
{
	uint8_t motor_ah,motor_al,motor_bh,motor_bl;

	motor_ah = (uint8_t)(motor_a >> 8);
	motor_al = (uint8_t)(motor_a & 0x00FF);
	motor_bh = (uint8_t)(motor_b >> 8);
	motor_bl = (uint8_t)(motor_b & 0x00FF);
	
	NeZha_WriteCommand(NEZHA_CMD_MOTOR1_SET);
	
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_MOTOR1_SET);
	NeZha_I2C_SendByte(motor_ah);
	NeZha_I2C_SendByte(motor_al);
	NeZha_I2C_SendByte(motor_bh);
	NeZha_I2C_SendByte(motor_bl);
	NeZha_I2C_Stop();
}

/**
  * @brief  NeZha驱动板电机2的PWM设置
  * @param 	motor_a：电机反转，范围0-1000
			motor_b：电机正转，范围0-1000
  * @retval 无
  */
void NeZha_Motor2_SetPwm(uint16_t motor_a,uint16_t motor_b)
{
	uint8_t motor_ah,motor_al,motor_bh,motor_bl;

	motor_ah = (uint8_t)(motor_a >> 8);
	motor_al = (uint8_t)(motor_a & 0x00FF);
	motor_bh = (uint8_t)(motor_b >> 8);
	motor_bl = (uint8_t)(motor_b & 0x00FF);
	
	NeZha_WriteCommand(NEZHA_CMD_MOTOR2_SET);
	
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_MOTOR2_SET);
	NeZha_I2C_SendByte(motor_ah);
	NeZha_I2C_SendByte(motor_al);
	NeZha_I2C_SendByte(motor_bh);
	NeZha_I2C_SendByte(motor_bl);
	NeZha_I2C_Stop();
}

/**
  * @brief  NeZha驱动板电机3的PWM设置
  * @param 	motor_a：电机反转，范围0-1000
			motor_b：电机正转，范围0-1000
  * @retval 无
  */
void NeZha_Motor3_SetPwm(uint16_t motor_a,uint16_t motor_b)
{
	uint8_t motor_ah,motor_al,motor_bh,motor_bl;

	motor_ah = (uint8_t)(motor_a >> 8);
	motor_al = (uint8_t)(motor_a & 0x00FF);
	motor_bh = (uint8_t)(motor_b >> 8);
	motor_bl = (uint8_t)(motor_b & 0x00FF);

	NeZha_WriteCommand(NEZHA_CMD_MOTOR3_SET);
	
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_MOTOR3_SET);
	NeZha_I2C_SendByte(motor_ah);
	NeZha_I2C_SendByte(motor_al);
	NeZha_I2C_SendByte(motor_bh);
	NeZha_I2C_SendByte(motor_bl);
	NeZha_I2C_Stop();
}

/**
  * @brief  NeZha驱动板电机4的PWM设置
  * @param 	motor_a：电机反转，范围0-1000
			motor_b：电机正转，范围0-1000
  * @retval 无
  */
void NeZha_Motor4_SetPwm(uint16_t motor_a,uint16_t motor_b)
{
	uint8_t motor_ah,motor_al,motor_bh,motor_bl;

	motor_ah = (uint8_t)(motor_a >> 8);
	motor_al = (uint8_t)(motor_a & 0x00FF);
	motor_bh = (uint8_t)(motor_b >> 8);
	motor_bl = (uint8_t)(motor_b & 0x00FF);
	
	NeZha_WriteCommand(NEZHA_CMD_MOTOR4_SET);
	
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_MOTOR4_SET);
	NeZha_I2C_SendByte(motor_ah);
	NeZha_I2C_SendByte(motor_al);
	NeZha_I2C_SendByte(motor_bh);
	NeZha_I2C_SendByte(motor_bl);
	NeZha_I2C_Stop();
}

/**
  * @brief  NeZha驱动板电机1编码器初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Encoder1_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_ENCODER1_INIT);	
}

/**
  * @brief  NeZha驱动板电机2编码器初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Encoder2_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_ENCODER2_INIT);	
}

/**
  * @brief  NeZha驱动板电机3编码器初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Encoder3_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_ENCODER3_INIT);	
}

/**
  * @brief  NeZha驱动板电机4编码器初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Encoder4_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_ENCODER4_INIT);	
}

/**
  * @brief  NeZha驱动板电机1编码器数据读取
  * @param 	无
  * @retval value：电机1的转速(编码器数据采集周期为20ms)
			value>=0,表示电机正转
			value<0,表示电机反转
  */
int16_t NeZha_Encoder1_Read()
{
	uint8_t value_h = 0, value_l = 0;
	int16_t value = 0;

	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_ENCODER1_READ);
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR|0x01);
	
	value_h = NeZha_I2C_ReadByte();
	NeZha_I2C_ACK();
	value_l = NeZha_I2C_ReadByte();
	NeZha_I2C_ACK();
	
	NeZha_I2C_Stop();
		
	value = (value_h << 8) |  value_l;
	return value;
}

/**
  * @brief  NeZha驱动板电机2编码器数据读取
  * @param 	无
  * @retval value：电机2的转速(编码器数据采集周期为20ms)
			value>=0,表示电机正转
			value<0,表示电机反转
  */
int16_t NeZha_Encoder2_Read()
{
	uint8_t value_h = 0, value_l = 0;
	int16_t value = 0;

	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_ENCODER2_READ);
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR|0x01);
	value_h = NeZha_I2C_ReadByte();
	NeZha_I2C_ACK();
	value_l = NeZha_I2C_ReadByte();
	NeZha_I2C_ACK();
	NeZha_I2C_Stop();
		
	value = (value_h << 8) |  value_l;

	return value;
}

/**
  * @brief  NeZha驱动板电机3编码器数据读取
  * @param 	无
  * @retval value：电机3的转速(编码器数据采集周期为20ms)
			value>=0,表示电机正转
			value<0,表示电机反转
  */
int16_t NeZha_Encoder3_Read()
{
	uint8_t value_h = 0, value_l = 0;
	int16_t value = 0;

	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_ENCODER3_READ);
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR|0x01);
	value_h = NeZha_I2C_ReadByte();
	NeZha_I2C_ACK();
	value_l = NeZha_I2C_ReadByte();
	NeZha_I2C_ACK();
	NeZha_I2C_Stop();
		
	value = (value_h << 8) |  value_l;

	return value;
}

/**
  * @brief  NeZha驱动板电机4编码器数据读取
  * @param 	无
  * @retval value：电机4的转速(编码器数据采集周期为20ms)
			value>=0,表示电机正转
			value<0,表示电机反转
  */
int16_t NeZha_Encoder4_Read()
{
	uint8_t value_h = 0, value_l = 0;
	int16_t value = 0;

	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_ENCODER4_READ);
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR|0x01);
	value_h = NeZha_I2C_ReadByte();
	NeZha_I2C_ACK();
	value_l = NeZha_I2C_ReadByte();
	NeZha_I2C_ACK();
	NeZha_I2C_Stop();
		
	value = (value_h << 8) |  value_l;

	return value;
}

/**
  * @brief  NeZha驱动板舵机1初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Servo1_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_SERVO1_INIT);	
}

/**
  * @brief  NeZha驱动板舵机2初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Servo2_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_SERVO2_INIT);	
}

/**
  * @brief  NeZha驱动板舵机3初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Servo3_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_SERVO3_INIT);	
}

/**
  * @brief  NeZha驱动板舵机4初始化
  * @param 	无
  * @retval 无
  */
void NeZha_Servo4_Init()
{
	NeZha_WriteCommand(NEZHA_CMD_SERVO4_INIT);	
}

/**
  * @brief  NeZha驱动板舵机1的PWM设置
  * @param 	pwm 范围：50-250(对应舵机角度0°- 180°)
  * @retval 无
  */
void NeZha_Servo1_SetPwm(uint16_t pwm)
{
	uint8_t servo1_h,servo1_l;

	servo1_h = (uint8_t)(pwm >> 8);
	servo1_l = (uint8_t)(pwm & 0x00FF);
	
	NeZha_WriteCommand(NEZHA_CMD_SERVO1_SET);
	
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_SERVO1_SET);
	NeZha_I2C_SendByte(servo1_h);
	NeZha_I2C_SendByte(servo1_l);
	NeZha_I2C_Stop();
}

/**
  * @brief  NeZha驱动板舵机2的PWM设置
  * @param 	pwm 范围：50-250(对应舵机角度0°- 180°)
  * @retval 无
  */
void NeZha_Servo2_SetPwm(uint16_t pwm)
{
	uint8_t servo2_h,servo2_l;

	servo2_h = (uint8_t)(pwm >> 8);
	servo2_l = (uint8_t)(pwm & 0x00FF);
	
	NeZha_WriteCommand(NEZHA_CMD_SERVO2_SET);
	
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_SERVO2_SET);
	NeZha_I2C_SendByte(servo2_h);
	NeZha_I2C_SendByte(servo2_l);
	NeZha_I2C_Stop();
}

/**
  * @brief  NeZha驱动板舵机3的PWM设置
  * @param 	pwm 范围：50-250(对应舵机角度0°- 180°)
  * @retval 无
  */
void NeZha_Servo3_SetPwm(uint16_t pwm)
{
	uint8_t servo3_h,servo3_l;

	servo3_h = (uint8_t)(pwm >> 8);
	servo3_l = (uint8_t)(pwm & 0x00FF);
	
	NeZha_WriteCommand(NEZHA_CMD_SERVO3_SET);
	
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_SERVO3_SET);
	NeZha_I2C_SendByte(servo3_h);
	NeZha_I2C_SendByte(servo3_l);
	NeZha_I2C_Stop();
}

/**
  * @brief  NeZha驱动板舵机4的PWM设置
  * @param 	pwm 范围：50-250(对应舵机角度0°- 180°)
  * @retval 无
  */
void NeZha_Servo4_SetPwm(uint16_t pwm)
{
	uint8_t servo4_h,servo4_l;

	servo4_h = (uint8_t)(pwm >> 8);
	servo4_l = (uint8_t)(pwm & 0x00FF);
	
	NeZha_WriteCommand(NEZHA_CMD_SERVO4_SET);
	
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(NEZHA_CMD_SERVO4_SET);
	NeZha_I2C_SendByte(servo4_h);
	NeZha_I2C_SendByte(servo4_l);
	NeZha_I2C_Stop();
}

/**
  * @brief  NeZha驱动板前灯打开
  * @param  无
  * @retval 无
  */
void NeZha_HeadLed_TurnOn()
{
	NeZha_WriteCommand(NEZHA_CMD_HEADLED_ON);
}

/**
  * @brief  NeZha驱动板前灯关闭
  * @param  无
  * @retval 无
  */
void NeZha_HeadLed_TurnOff()
{
	NeZha_WriteCommand(NEZHA_CMD_HEADLED_OFF);
}

/**
  * @brief  NeZha驱动板前灯翻转
  * @param  无
  * @retval 无
  */
void NeZha_HeadLed_Turn()
{
	NeZha_WriteCommand(NEZHA_CMD_HEADLED_TURN);
}

/**
  * @brief  NeZha驱动板氛围灯打开
  * @param  无
  * @retval 无
  */
void NeZha_AmbientLed_TurnOn()
{
	NeZha_WriteCommand(NEZHA_CMD_AMBIENTLED_ON);
}

/**
  * @brief  NeZha驱动板氛围灯关闭
  * @param  无
  * @retval 无
  */
void NeZha_AmbientLed_TurnOff()
{
	NeZha_WriteCommand(NEZHA_CMD_AMBIENTLED_OFF);
}

/**
  * @brief  NeZha驱动板氛围灯翻转
  * @param  无
  * @retval 无
  */
void NeZha_AmbientLed_Turn()
{
	NeZha_WriteCommand(NEZHA_CMD_AMBIENTLED_TURN);
}

/**
  * @brief  NeZha驱动板左尾灯打开
  * @param  无
  * @retval 无
  */
void NeZha_TailLeftLed_TurnOn()
{
	NeZha_WriteCommand(NEZHA_CMD_TAILLEFTLED_ON);
}

/**
  * @brief  NeZha驱动板左尾灯关闭
  * @param  无
  * @retval 无
  */
void NeZha_TailLeftLed_TurnOff()
{
	NeZha_WriteCommand(NEZHA_CMD_TAILLEFTLED_OFF);
}

/**
  * @brief  NeZha驱动板左尾灯翻转
  * @param  无
  * @retval 无
  */
void NeZha_TailLeftLed_Turn()
{
	NeZha_WriteCommand(NEZHA_CMD_TAILLEFTLED_TURN);
}

/**
  * @brief  NeZha驱动板右尾灯打开
  * @param  无
  * @retval 无
  */
void NeZha_TailRightLed_TurnOn()
{
	NeZha_WriteCommand(NEZHA_CMD_TAILRIGHTLED_ON);
}

/**
  * @brief  NeZha驱动板右尾灯关闭
  * @param  无
  * @retval 无
  */
void NeZha_TailRightLed_TurnOff()
{
	NeZha_WriteCommand(NEZHA_CMD_TAILRIGHTLED_OFF);
}

/**
  * @brief  NeZha驱动板右尾灯翻转
  * @param  无
  * @retval 无
  */
void NeZha_TailRightLed_Turn()
{
	NeZha_WriteCommand(NEZHA_CMD_TAILRIGHTLED_TURN);
}

/**
  * @brief  NeZha驱动板发送一个命令
  * @param 	Command 范围：NeZha指令
  * @retval 无
  */
void NeZha_WriteCommand(uint8_t Command)
{
	NeZha_I2C_Start();
	NeZha_I2C_SendByte(NEZHA_ADDR);
	NeZha_I2C_SendByte(0x00);
	NeZha_I2C_SendByte(Command);
	NeZha_I2C_Stop();	
}
