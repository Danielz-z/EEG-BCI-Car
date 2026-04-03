#ifndef _ROBOTARM_H_
#define _ROBOTARM_H_


void RobotArm_Init(void);
void Robot_Arm_Reset(void);
void RobotArm_RaiseHand(uint8_t unit_pwm);
void RobotArm_DropHand(uint8_t unit_pwm);
void RobotArm_StretchHand(uint8_t unit_pwm);
void RobotArm_ShrinkHand(uint8_t unit_pwm);
void RobotArm_ShakeHand(uint8_t unit_pwm);
void RobotArm_LetHand(uint8_t unit_pwm);

#endif

