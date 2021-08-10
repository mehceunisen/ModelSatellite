/*
 * Madgwick.h
 *
 *  Created on: Jun 8, 2021
 *      Author: merty
 */

#ifndef INC_MADGWICK_H_
#define INC_MADGWICK_H_

#include "main.h"
#include "MPU6050.h"
#include "Quaternion.h"

typedef struct
{
	Quaternion *gyroQ;
	Quaternion *preQ;
	Quaternion *outQ;

}Madgwick;

Madgwick mad;

#endif /* INC_MADGWICK_H_ */
