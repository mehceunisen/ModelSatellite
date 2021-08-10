/*
 * KalmanFilterAHRS.h
 *
 *  Created on: Jun 30, 2021
 *      Author: merty
 */

#ifndef INC_KALMANFILTERAHRS_H_
#define INC_KALMANFILTERAHRS_H_

#define dT 0.004
#define qAcc 1
#define qAccB 1
#define N 3

typedef struct
{
	float ak; // Altitude
	float vk; // Vertical Velocity
	float bk; // Acceleration Bias
}StateMatrix;

typedef struct
{
	float pre[3][3];
	float p[3][3];
}StateCovarianceMatrix;

StateMatrix z, pre_z;

//float Q[3][3]; // Process Noise Covariance Matrix
float F[3][3]; // State Transition Matrix

void MatrixMultiplication(float matrixOne[][N], float matrixTwo[][N], float matrixRes[][N]);
void SetQMatrix(float *Q);
void UpdateStateMatrix(StateMatrix *z, StateMatrix pre_z);
void UpdateAltitudeStateMatrix(StateMatrix *z, StateMatrix pre_z);
void UpdateVerVelStateMatrix(StateMatrix *z, StateMatrix pre_z);
void UpdateAccBiasStateMatrix(StateMatrix *z);
void UpdatePreStateMatrix(StateMatrix *z, StateMatrix *pre_z);


#endif /* INC_KALMANFILTERAHRS_H_ */
