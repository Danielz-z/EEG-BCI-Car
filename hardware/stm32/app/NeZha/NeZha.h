#ifndef _NeZha_H_
#define _NeZha_H_

//NeZha÷∏¡Ó
#define NEZHA_ADDR				0x80

#define NEZHA_CMD_MOTOR_WAIT		0x00  
#define NEZHA_CMD_MOTOR_INIT		0x01  
#define NEZHA_CMD_MOTOR1_SET		0x05 
#define NEZHA_CMD_MOTOR2_SET		0x09  
#define NEZHA_CMD_MOTOR3_SET		0x0D 
#define NEZHA_CMD_MOTOR4_SET		0x11  
#define NEZHA_CMD_ENCODER1_INIT		0x15  
#define NEZHA_CMD_ENCODER1_READ		0x16  
#define NEZHA_CMD_ENCODER2_INIT		0x18  
#define NEZHA_CMD_ENCODER2_READ		0x19  
#define NEZHA_CMD_ENCODER3_INIT		0x1B  
#define NEZHA_CMD_ENCODER3_READ		0x1C  
#define NEZHA_CMD_ENCODER4_INIT		0x1E  
#define NEZHA_CMD_ENCODER4_READ		0x1F  
#define NEZHA_CMD_SERVO1_INIT		0x21  
#define NEZHA_CMD_SERVO1_SET		0x22 
#define NEZHA_CMD_SERVO2_INIT		0x24  
#define NEZHA_CMD_SERVO2_SET		0x25  
#define NEZHA_CMD_SERVO3_INIT		0x27  
#define NEZHA_CMD_SERVO3_SET		0x28  
#define NEZHA_CMD_SERVO4_INIT		0x2A 
#define NEZHA_CMD_SERVO4_SET		0x2B 
#define NEZHA_CMD_HEADLED_ON		0x2D
#define NEZHA_CMD_HEADLED_OFF		0x2E
#define NEZHA_CMD_HEADLED_TURN		0x2F
#define NEZHA_CMD_TAILLEFTLED_ON	0x30
#define NEZHA_CMD_TAILLEFTLED_OFF	0x31
#define NEZHA_CMD_TAILLEFTLED_TURN	0x32
#define NEZHA_CMD_TAILRIGHTLED_ON	0x33
#define NEZHA_CMD_TAILRIGHTLED_OFF	0x34
#define NEZHA_CMD_TAILRIGHTLED_TURN	0x35
#define NEZHA_CMD_AMBIENTLED_ON		0x36
#define NEZHA_CMD_AMBIENTLED_OFF	0x37
#define NEZHA_CMD_AMBIENTLED_TURN   0X38
#define NEZHA_CMD_RESET				0XFF

void NeZha_Init(void);
void NeZha_Reset(void);
void NeZha_Motor_Init(void);
void NeZha_Forward(uint16_t Speed);
void NeZha_Backward(uint16_t Speed);
void NeZha_TurnLeft(uint16_t Speed);
void NeZha_TurnRight(uint16_t Speed);
void NeZha_TransLeft(uint16_t Speed);
void NeZha_TransRight(uint16_t Speed);
void NeZha_Motor1_SetPwm(uint16_t motor_a,uint16_t motor_b);
void NeZha_Motor2_SetPwm(uint16_t motor_a,uint16_t motor_b);
void NeZha_Motor3_SetPwm(uint16_t motor_a,uint16_t motor_b);
void NeZha_Motor4_SetPwm(uint16_t motor_a,uint16_t motor_b);

void NeZha_Encoder1_Init(void);
void NeZha_Encoder2_Init(void);
void NeZha_Encoder3_Init(void);
void NeZha_Encoder4_Init(void);
int16_t NeZha_Encoder1_Read(void);
int16_t NeZha_Encoder2_Read(void);
int16_t NeZha_Encoder3_Read(void);
int16_t NeZha_Encoder4_Read(void);

void NeZha_Servo1_Init(void);
void NeZha_Servo2_Init(void);
void NeZha_Servo3_Init(void);
void NeZha_Servo4_Init(void);
void NeZha_Servo1_SetPwm(uint16_t pwm);
void NeZha_Servo2_SetPwm(uint16_t pwm);
void NeZha_Servo3_SetPwm(uint16_t pwm);
void NeZha_Servo4_SetPwm(uint16_t pwm);

void NeZha_HeadLed_TurnOn(void);
void NeZha_HeadLed_TurnOff(void);
void NeZha_HeadLed_Turn(void);
void NeZha_AmbientLed_TurnOn(void);
void NeZha_AmbientLed_TurnOff(void);
void NeZha_AmbientLed_Turn(void);
void NeZha_TailLeftLed_TurnOn(void);
void NeZha_TailLeftLed_TurnOff(void);
void NeZha_TailLeftLed_Turn(void);
void NeZha_TailRightLed_TurnOn(void);
void NeZha_TailRightLed_TurnOff(void);
void NeZha_TailRightLed_Turn(void);

void NeZha_WriteCommand(uint8_t Command);

#endif

