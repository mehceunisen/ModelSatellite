#include <stdlib.h>
#include <string.h>
#include "PID.h"

float	PID(float Set_Value, float Actual_Value, float Kp, float Ki, float Kd, float Limit, double dt,float *integral,float *pre_error)
{
	const int freq = 1/dt;
	float error;
	float derivative;
	float output;

	error = Set_Value - Actual_Value;
	if(Kd !=0)
	{
		derivative = (error - *pre_error) / dt;// dt must be pre defined according to pid frequency
	}
	else{
		derivative=0;
	}

	// Anti-windup clamp method
	output = Kp*error + Ki*(*integral) + Kd*derivative;
	if(abs((int)output) >= Limit && (((error >= 0) && (*integral >= 0)) || ((error < 0) && (*integral < 0))))
	{
			*integral = *integral;
	}
	else
	{
		*integral = *integral + (dt * error);
	}


	if(abs((int)output) >= Limit)
	{
		if(output < 0)
		{
			output = -Limit;
		}
		else
		{
			output = Limit;
		}
  }
	*pre_error = error;
	return output;
}

float	PID_2(float Set_Value, float Actual_Value, typedef_PID* PID_struct)
{
  //static float freq = 1/PID_struct->dt;
	float error;
	float derivative;
	float output;

	error = Set_Value - Actual_Value;
	if(PID_struct->Kd !=0)
	{
		derivative = (error - PID_struct->pre_error) / PID_struct->dt;// dt must be pre defined according to pid frequency
	}
	else{
		derivative=0;
	}

	// Anti-windup clamp method
	output = PID_struct->Kp*error + PID_struct->Ki*(PID_struct->integral) + PID_struct->Kd*derivative;
	if((PID_struct->min_limit >= output || output >= PID_struct->max_limit) && (((error >= 0) && (PID_struct->integral >= 0)) || ((error < 0) && (PID_struct->integral < 0))))
	{
			PID_struct->integral = PID_struct->integral;
	}
	else
	{
		PID_struct->integral = PID_struct->integral + (PID_struct->dt * error);
	}

	output = output >= PID_struct->max_limit ? PID_struct->max_limit : output;
	output = output <= PID_struct->min_limit ? PID_struct->min_limit : output;
	PID_struct->pre_error = error;
	return output;
}

