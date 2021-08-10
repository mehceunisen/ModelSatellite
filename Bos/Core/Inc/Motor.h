/*
 * MotorPWMDeneme.h
 *
 *  Created on: 6 May 2021
 *      Author: merty
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

#define MOTOR_DUTY_CYCLE(__HANDLE__, __MOTORID__, __DUTY__) \
	(((__MOTORID__)== DC_MOTOR)    ? ((__HANDLE__)->Instance->CCR1 = ((__DUTY__) * (TIM1->ARR + 1) / 100)) :\
	((__MOTORID__) == SERVO_PITCH) ? ((__HANDLE__)->Instance->CCR2 = ((__DUTY__) * (TIM1->ARR + 1) / 100)) :\
    ((__MOTORID__) == SERVO_ROLL)  ? ((__HANDLE__)->Instance->CCR3 = ((__DUTY__) * (TIM1->ARR + 1) / 100)) :\
    ((__HANDLE__)->Instance->CCR4 = ((__DUTY__) * TIM1->ARR / 100)))

typedef enum
{
	DC_MOTOR,
	SERVO_PITCH,
	SERVO_ROLL,
	SERVO_LID,
}MotorID;


void InitMotorLib(TIM_HandleTypeDef * TIMhnd);
void InitESC();
void ServoAngleCompute();
void MotorPowerCompute();
void ServoRotate();
float DutytoAngle(float duty);

#endif /* INC_MOTOR_H_ */
