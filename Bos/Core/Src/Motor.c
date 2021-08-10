/*
 * MotorPWMDeneme.c
 *
 *  Created on: 6 May 2021
 *      Author: merty
 */

#include <PID.h>
#include <Motor.h>
#include <MPU6050.h>

#define MIN_DUTY  1000
#define MAX_DUTY  2000
#define ANGLE_TO_DUTY 0.0555555555555556

static TIM_HandleTypeDef timHandler;
typedef_PID pPid, rPid;
float HzCofficent;

void InitMotorLib(TIM_HandleTypeDef * TIMhnd)
{
	memcpy(&timHandler, TIMhnd, sizeof(*TIMhnd));
}

void InitESC()
{
	timHandler.Instance->CCR1 = MAX_DUTY;
	HAL_Delay(2000);
	timHandler.Instance->CCR1 = MIN_DUTY;
}

void ServoAngleCompute()
{
	float set_value = 0;
	float pitch_duty = PID(set_value, mpu6050.pitch, pPid.Kp, pPid.Kd, pPid.Ki, pPid.max_limit, pPid.dt, &pPid.integral, &pPid.pre_error);
	float roll_duty  = PID(set_value, mpu6050.roll , rPid.Kp, rPid.Kd, rPid.Ki, rPid.max_limit, rPid.dt, &rPid.integral, &rPid.pre_error);
	MOTOR_DUTY_CYCLE(&timHandler, SERVO_PITCH, pitch_duty);
	MOTOR_DUTY_CYCLE(&timHandler, SERVO_ROLL, roll_duty);
}

void ServoRotate()
{
	float roll_duty = (ANGLE_TO_DUTY * mpu6050.roll) + 7.5;
	MOTOR_DUTY_CYCLE(&timHandler, SERVO_ROLL , roll_duty);
	float pitch_duty = (ANGLE_TO_DUTY * mpu6050.pitch) + 7.5;
	MOTOR_DUTY_CYCLE(&timHandler, SERVO_PITCH , pitch_duty);
}

float DutytoAngle(float duty)
{
	float angle = ((float)duty - 7.5) / ANGLE_TO_DUTY;
	return angle;
}
