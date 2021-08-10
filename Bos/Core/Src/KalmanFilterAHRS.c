/*
 * KalmanFilterAHRS.c
 *
 *  Created on: Jun 30, 2021
 *      Author: merty
 */


#include <main.h>
#include <math.h>
#include <MPU6050.h>
#include <KalmanFilterAHRS.h>

//float Q[3][3] = {{((pow(dT,4) * pow(qAcc,2)) / 4), ((pow(dT,3) * pow(qAcc,2)) / 2), 0}, {((pow(dT,3) * pow(qAcc,2)) / 2), (pow(dT,2) * pow(qAcc,2)), 0}, {0, 0, pow(qAccB,2)}};

void MatrixMultiplication(float matrixOne[][N], float matrixTwo[][N], float matrixRes[][N])
{
	int i, j, k;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			matrixRes[i][j] = 0;
	        for (k = 0; k < N; k++)
	        	matrixRes[i][j] += matrixOne[i][k] * matrixTwo[k][j];
	        }
	    }
}

void MatrixPlus(float matrixOne[][N], float matrixTwo[][N], float matrixRes[][N])
{
	int i, j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			matrixRes[i][j] = matrixOne[i][j] + matrixTwo[i][j];
		}
	}
}

void UpdateAltitudeStateMatrix(StateMatrix *z, StateMatrix pre_z)
{
	z->ak = pre_z.ak + (dT * pre_z.vk) + (pow(dT,2) * (myAccelMean.z));
}

void UpdateVerVelStateMatrix(StateMatrix *z, StateMatrix pre_z)
{
	z->vk = pre_z.vk + (dT * myAccelMean.z);
}

void UpdateAccBiasStateMatrix(StateMatrix *z)
{
	z->bk = myAccelCal.z;
}

void UpdateStateMatrix(StateMatrix *z, StateMatrix pre_z)
{
	UpdateAltitudeStateMatrix(z, pre_z);
	UpdateVerVelStateMatrix(z, pre_z);
	UpdateAccBiasStateMatrix(z);
}

void UpdatePreStateMatrix(StateMatrix *z, StateMatrix *pre_z)
{
	pre_z->ak = z->ak;
	pre_z->vk = z->vk;
	pre_z->bk = z->bk;
}

void UpdateStateCovarianceMatrix(StateCovarianceMatrix *P)
{
//	static float matrixResOne[N][N];
//	static float matrixResTwo[N][N];
//	MatrixMultiplication(F, P->pre,matrixResOne);
//	MatrixMultiplication(matrixResOne, Ft, matrixResTwo);
//	MatrixPlus(matrixResTwo, Q, P->p);
}
