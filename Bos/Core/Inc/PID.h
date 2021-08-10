
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdint.h>

typedef struct{

	float Kp;
	float Ki;
	float Kd;
	float max_limit;
	float min_limit;
	float dt;
	float integral;
	float pre_error;

}typedef_PID;
float	PID_2(float Set_Value, float Actual_Value, typedef_PID* PID_struct);
float   PID(float Set_Value, float Actual_Value, float Kp, float Ki, float Kd, float Limit, double dt,float *integral,float *pre_error);
#endif
