/*
 * main.c
 *
 *  Created on: 2013. 6. 6.
 *      Author: administrator
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct timeval lt;
static int first = 1;

void set_timer() {

    if (first) {

       gettimeofday(&lt, NULL);

       first = 0;

    }

}

double get_timer() {

    struct timeval tt;

    double lap_time;

    if (first) {

       set_timer();

       return 0.0;

    }

    gettimeofday(&tt, NULL);

    lap_time = (double)(tt.tv_sec - lt.tv_sec) + (double)(tt.tv_usec - lt.tv_usec)/1000000.0;

    return lap_time;

}

#define MATRIX(n, i, j)	*(matrix[n] +(matrix_size[n][1] * i + j))
#define FLOATLEN	20

int main(void)
{
	FILE *fp1, *fp2;
	unsigned int matrix_size[3][2];
	unsigned int i, j, k;
	float *matrix[3];
	float *temp;

	fp1 = fopen("matrix1", "r");
	fp2 = fopen("matrix2", "r");
	if(fp1 == NULL || fp2 == NULL)
	{
		printf("no input file!\n");
		return -1;
	}

	//Read First Matrix
	fscanf(fp1, "%d %d", &matrix_size[0][0], &matrix_size[0][1]);

	matrix[0] = (float*)malloc(sizeof(float) * matrix_size[0][0] * matrix_size[0][1]);
	for(i = 0; i < matrix_size[0][0] * matrix_size[0][1]; i++)
	{
		fscanf(fp1, "%f", matrix[0]+i);
	}
	fclose(fp1);

	//Read Second Matrix
	fscanf(fp2, "%d %d", &matrix_size[1][1], &matrix_size[1][0]);

	temp = (float*)malloc(sizeof(float) * matrix_size[1][0] * matrix_size[1][1]);
	for(i = 0; i < matrix_size[1][0] * matrix_size[1][1]; i++)
	{
		fscanf(fp2, "%f", temp+i);
	}
	fclose(fp2);
	matrix[1] = (float*)malloc(sizeof(float) * matrix_size[1][0] * matrix_size[1][1]);
	for(i = 0; i < matrix_size[1][0]; i++)
	{
		for(j = 0; j < matrix_size[1][1]; j++)
		{
			MATRIX(1, i, j) = *(temp + j * matrix_size[1][0] + i);
		}
	}
	free(temp);

	if(matrix_size[0][1] != matrix_size[1][1])
	{
		//Matrix Sizes do not match!
		printf("wrong input matrix\n");
		return -1;
	}

	matrix[2] = (float*)malloc(sizeof(float)*matrix_size[0][0] * matrix_size[1][1]);
	matrix_size[2][0] = matrix_size[0][0];
	matrix_size[2][1] = matrix_size[1][0];

	float acc;
	set_timer();
	for(i = 0; i < matrix_size[0][0]; i++)
	{
		for(j = 0; j < matrix_size[1][0]; j++)
		{
			acc = 0;
			for(k = 0; k < matrix_size[0][1]; k++)
			{
				acc = acc + MATRIX(0, i, k) * MATRIX(1, j, k);
			}
			MATRIX(2, i, j) = acc;
		}
	}
	printf ("time : %.6lf\n", get_timer());
	free(matrix[0]);
	free(matrix[1]);
	FILE *output = fopen("sepe", "w");
	for(i = 0; i < matrix_size[0][0]; i++)
	{
		for(j = 0; j < matrix_size[1][0]; j++)
		{
			fprintf(output, "%f ", MATRIX(2, i, j));
		}
		fprintf(output, "\n");
	}
	fclose(output);
	free(matrix[2]);
	return 0;
}
