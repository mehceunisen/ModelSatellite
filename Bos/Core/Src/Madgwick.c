/*
 * Madgwick.c
 *
 *  Created on: Jun 8, 2021
 *      Author: merty
 */

#include "Madgwick.h"

#define GAIN 0.5
#define INTEGRAL_COFF 0.004

void Madgwick_GyrotoQuaternion(Quaternion *gyroQ)
{
	 gyroQ->w = 0;
	 gyroQ->v[0] = myGyroMean.x;
	 gyroQ->v[1] = myGyroMean.y;
	 gyroQ->v[2] = myGyroMean.z;
}

void Madgwick_QuaternionMulti(Quaternion *gyroQ, Quaternion *preQ, Quaternion *outQ)
{
	Quaternion_multiply(gyroQ, preQ, outQ);
	outQ->v[0] *= GAIN;
	outQ->v[1] *= GAIN;
	outQ->v[2] *= GAIN;
}

void Madgwick_Integral(Quaternion *outQ)
{
	outQ->w    *= INTEGRAL_COFF;
	outQ->v[0] *= INTEGRAL_COFF;
	outQ->v[1] *= INTEGRAL_COFF;
	outQ->v[2] *= INTEGRAL_COFF;
}

void Madgwick_Sum(Quaternion *outQ, Quaternion *preQ)
{
	outQ->w += preQ->w;
	outQ->v[0] += preQ->v[0];
	outQ->v[1] += preQ->v[1];
	outQ->v[2] += preQ->v[2];
}

//void Madgwick_QuaternionNormalize(Quaternion *outQ)
//{
//	Quaternion_normalize(outQ, outQ);
//}

void Madgwick_QuaternionEqualize(Quaternion *preQ, Quaternion *outQ)
{
	preQ->w += outQ->w;
	preQ->v[0] += outQ->v[0];
	preQ->v[1] += outQ->v[1];
	preQ->v[2] += outQ->v[2];
}

void Madgwick_QuaterniontoEuler(Quaternion *outQ)
{
//	Quaternion_toEulerZYX(outQ, outQ);
}

void Madgwick_Update()
{
	Madgwick_GyrotoQuaternion(mad.gyroQ);
	Madgwick_QuaternionMulti(mad.gyroQ, mad.preQ, mad.outQ);


	Madgwick_Integral(mad.outQ);
	Madgwick_Sum(mad.outQ, mad.preQ);
//	Madgwick_QuaternionNormalize(mad.outQ);
	Madgwick_QuaternionEqualize(mad.preQ, mad.outQ);
	Madgwick_QuaterniontoEuler(mad.outQ);//BURADA SIÇTIK
}
