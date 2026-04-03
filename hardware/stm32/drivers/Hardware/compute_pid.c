#include "compute_pid.h"
#include "math.h"
/**
  * @brief  PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
int Increment_PID_CloseLoop(PID *pid, int actual_val, int target_val)
{
	/*计算目标值与实际值的误差*/
	pid->err = target_val - actual_val;
	
	/*积分项*/
	pid->integral += pid->err;
	pid->integral = pid->integral > 1000? 1000 : pid->integral;
	pid->integral = pid->integral < -1000? -1000 : pid->integral;	
	
	/*PID算法实现*/

	pid->output_val = pid->Kp * pid->err + 
				     pid->Ki * pid->integral + 
				     pid->Kd * (pid->err - pid->err_last);
	
	/*误差传递*/
	pid->err_last = pid->err;

	pid->output_val = pid->output_val > 1000? 1000 : pid->output_val;
	pid->output_val = pid->output_val < -1000? -1000 : pid->output_val;
	/*返回当前实际值*/
	return pid->output_val;
}
