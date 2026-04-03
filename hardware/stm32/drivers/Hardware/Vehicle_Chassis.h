#ifndef _VEHICLE_CHASSIS_H_
#define _VEHICLE_CHASSIS_H_

void Vehicle_Chassis_Init(void);

void Arc_Servo_Init(void);
void Arc_Servo_Reset(void);
void Arc_ServoPwm_Set(uint16_t value);
void Arc_ServoPwm_TurnLeft(uint8_t unit_pwm);
void Arc_ServoPwm_TurnRight(uint8_t unit_pwm);

void Vehicle_Chassis_Forward(uint16_t Speed);
void Vehicle_Chassis_Backward(uint16_t Speed);
void Vehicle_Chassis_TurnLeft(uint16_t Speed);
void Vehicle_Chassis_TurnRight(uint16_t Speed);
void Vehicle_Chassis_TransLeft(uint16_t Speed);
void Vehicle_Chassis_TransRight(uint16_t Speed);

#endif

