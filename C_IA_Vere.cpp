#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define layers 3
#define N 2
#define P 2
#define Q 4
//#define R 5

using namespace std;

int state = 3;
void readInputs(double y0[])
{
	//for (int i = 0; i < N; ++i)	{		y0[i] = rand() % 2;	}
	switch (state)
	{
	case 0:
		y0[0] = 0;
		y0[1] = 0;
		state = 1;
		break;
	case 1:
		y0[0] = 0;
		y0[1] = 1;
		state = 2;
		break;
	case 2:
		y0[0] = 1;
		y0[1] = 0;
		state = 3;
		break;
	default:
		y0[0] = 1;
		y0[1] = 1;
		state = 0;
		break;
	}
	//printf("y --  %1.1lf %1.1lf  -- state %d\n", y0[0], y0[1], state);
}
void readOutputs(double y[], double y0[])
{
	for (int i = 0; i < Q; ++i)
	{
		y[i] = 0;
	}
	if (y0[0] == 0 && y0[1] == 0)
	{
		y[0] = 1;
	}
	if (y0[0] == 0 && y0[1] == 1)
	{
		y[1] = 1;
	}
	if (y0[0] == 1 && y0[1] == 0)
	{
		y[2] = 1;
	}
	if (y0[0] == 1 && y0[1] == 1)
	{
		y[3] = 1;
	}
	//printf("%lf xor %lf = %lf \n", y0[0], y0[1], y[0]);
}
double sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

int main(void)
{
	unsigned int nr_noduri[layers] = { N,P,Q };
	double alfa = 0.1;
	double weights01[N][P] = { 0.5,0.9,0.4,1.0 }, weights12[P][Q] = { -1.2,1.1 };
	double deltaWeights01[N][P], deltaWeights12[P][Q];
	double bias01[P] = { 0.8,-0.1 }, bias12[Q] = { 0.3 };
	double deltaBias01[P], deltaBias12[Q];
	double y0[N], y1[P], y2[Q], y[Q];
	double erori[Q];
	double gradient01[P], gradient12[Q];
	double sigma = 1;
	unsigned short int count = 0;
	unsigned long int generatii = 0;

	//randomize wights and biases
	//works: 5
	//not work: 14
	//srand(5);
	srand(time(0));
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < P; ++j)
		{
			weights01[i][j] = double(rand() % 1000) / 500 * 2.4 / nr_noduri[0] - 2.4 / nr_noduri[0];
			//printf("%+0.6lf ", weights01[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");

	for (int i = 0; i < P; ++i)
	{
		for (int j = 0; j < Q; ++j)
		{
			weights12[i][j] = double(rand() % 1000) / 500 * 2.4 / nr_noduri[0] - 2.4 / nr_noduri[0];
			//printf("%+0.6lf ", weights12[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");

	for (int i = 0; i < P; ++i)
	{
		bias01[i] = double(rand() % 1000) / 500 * 2.4 / nr_noduri[0] - 2.4 / nr_noduri[0];
		//printf("%+0.6lf ", bias01[i]);
	}
	//printf("\n\n");

	for (int i = 0; i < Q; ++i)
	{
		bias12[i] = double(rand() % 1000) / 500 * 2.4 / nr_noduri[0] - 2.4 / nr_noduri[0];
		//printf("%+0.6lf ", bias12[i]);
	}
	//printf("\n");
	//printf("1\n");
	while (sigma > 0.0001)
	{
		//printf("2\n");
		sigma = 0;

		for (int aa = 0; aa < 4; ++aa)
		{
			//printf("Da?\n");
			//printf("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\t\taa = %d   wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n", aa);
			//read inputs -> expected outputs
			readInputs(y0);
			readOutputs(y, y0);

			//calculate y1
			//printf("\n");
			for (int i = 0; i < P; ++i)
			{
				y1[i] = -bias01[i];
				//printf("%y1[%d]=%lf \n", i, y1[i]);
				for (int j = 0; j < N; ++j)
				{
					y1[i] = y1[i] + y0[j] * weights01[j][i];
					//printf("   %0.4lf %0.4lf %0.4lf \n", y1[i], y0[j], weights01[j][i]);
				}
				//printf("%y1[%d]=%.10lf \n", i, y1[i]);
				y1[i] = sigmoid(y1[i]);
				//printf("%y1[%d]=%.10lf \n", i, y1[i]);
			}
			//printf("\n");
			//printf("\n");

			//calculate y2
			for (int j = 0; j < Q; ++j)
			{
				y2[j] = -bias12[j];
				//printf("y2[%d]=%lf \n", j, y2[j]);
				for (int k = 0; k < P; ++k)
				{
					y2[j] = y2[j] + y1[k] * weights12[k][j];
					//printf("    %0.4lf %0.4lf %0.4lf \n", y2[j], y1[k], weights12[j][k]);
				}
				//printf("%y2[%d]=%.10lf \n", j, y2[j]);
				y2[j] = sigmoid(y2[j]);
				//printf("%y2[%d]=%.10lf \n", j, y2[j]);
			}
			//printf("\n");

			//printf("--%lf %lf %.10lf --\n", y0[0], y0[1], y2[0]);


			//printf("-----------------------------------------------\n");
			for (int k = 0; k < Q; ++k)
			{
				//calculam erorile pentru iesire
				erori[k] = y[k] - y2[k];
				//calculam gradientul pentru nuronul/neuronii respectivi
				gradient12[k] = y2[k] * (1 - y2[k]) * erori[k];
				//printf("gradient12 -----> %.10lf\n", gradient12[k]);
				//cout << k << " " << erori[k] << " OOOO gradient12" << gradient12[k] << endl;
			}

			//caculam delta pentru stratul de iesire
			for (int j = 0; j < P; ++j)
			{
				for (int k = 0; k < Q; ++k)
				{
					deltaWeights12[j][k] = alfa * y1[j] * gradient12[k];
					//printf("dw[%d][%d]=%.10lf   ", j, k, deltaWeights12[j][k]);
					//cout << deltaWeights12[j][k] << " ---" << endl;
				}
				//printf("\n");
			}
			//calculam delta pentru bias
			//printf("___ deltaBias12\n");
			for (int k = 0; k < Q; ++k)
			{
				deltaBias12[k] = -alfa * gradient12[k];
				//printf("%.10lf ", deltaBias12[k]);
			}

			//printf("\n-----------------------------------------------\n");

			for (int j = 0; j < P; ++j)
			{
				double suma = 0;
				//calculam suma/sumele 
				for (int k = 0; k < Q; ++k)
				{
					suma = suma + gradient12[k] * weights12[j][k];
					//printf("\tgr12[%d] = %.10lf   ---   we[%d][%d] = %.10lf\n",k, gradient12[k],j,k,weights12[j][k]);
				}
				//calculam gradientul pentru nuronul/neuronii respectivi
				gradient01[j] = y1[j] * (1 - y1[j]) * suma;
				//cout << j << "  " << suma << " " << gradient01[j] << endl;
				//printf("gr01[%d] = %.10lf * (1 - %.10lf) * %.10lf = %.10lf \n", j, y1[j], y1[j], suma, gradient01[j]);
			}

			//calculam delta pentru neuronii ascunsi
			for (int i = 0; i < N; ++i)
			{
				//cout << "y0[" << i << "]=" << y0[i] << endl;
				for (int j = 0; j < P; ++j)
				{
					deltaWeights01[i][j] = alfa * y0[i] * gradient01[j];
					//printf("dw[%d][%d]=%.10lf   ", i, j, deltaWeights01[i][j]);
					//cout << deltaWeights01[i][j] << " __ ";
				}
				//printf("\n");
			}


			//printf("___ deltaBias01\n");
			for (int j = 0; j < P; ++j)
			{
				deltaBias01[j] = -alfa * gradient01[j];
				//printf("%.10lf ", deltaBias01[j]);
			}
			//printf("\n");

			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < P; ++j)
				{
					//printf("%.10lf_%.10lf ", weights01[i][j], deltaWeights01[i][j]);
				}
				//printf("\n");
			}

			//printf("\n weights01 \n");
			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < P; ++j)
				{
					weights01[i][j] = weights01[i][j] + deltaWeights01[i][j];
				}
				//printf("\n");
			}

			//printf("\n weights12 \n");
			for (int j = 0; j < P; ++j)
			{
				for (int k = 0; k < Q; ++k)
				{
					weights12[j][k] = weights12[j][k] + deltaWeights12[j][k];
				}
				//printf("\n");
			}

			//noul bias01
			//printf("%.10lf %.10lf --------\n", deltaBias01[0], deltaBias01[1]);
			for (int j = 0; j < P; ++j)
			{
				//printf("bias01[%d]= %.10lf + %.10lf =  ", j, bias01[j], deltaBias01[j]);
				bias01[j] = bias01[j] + deltaBias01[j];
				//printf("%.10lf \n", bias01[j]);
			}
			//printf("%.10lf %.10lf --------\n", deltaBias01[0], deltaBias01[1]);
			//printf("%.10lf %.10lf --------\n", bias01[0], bias01[1]);
			//printf("\n");
			//noul bias12
			for (int k = 0; k < Q; ++k)
			{
				//printf("bias12[%d]= %.10lf + %.10lf =  ", k, bias12[k], deltaBias12[k]);
				bias12[k] = bias12[k] + deltaBias12[k];
				//printf(" %.10lf", bias12[k]);
			}


			//printf("\n weights01 \n");
			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < P; ++j)
				{
					//printf("%.10lf ", weights01[i][j]);
				}
				//printf("\n");
			}

			//printf("\n weights12 \n");
			for (int j = 0; j < P; ++j)
			{
				for (int k = 0; k < Q; ++k)
				{
					//printf("%.10lf ", weights12[j][k]);
				}
				//printf("\n");
			}
			sigma = sigma + (y[0] - y2[0]) * (y[0] - y2[0]);
		}
		for (int p = 0; p < 4; ++p)
		{
			for (int k = 0; k < Q; ++k)
			{
				//sigma = sigma + (y[0] - y2[0]) * (y[0] - y2[0]);
			}
		}
		if (count == 0)
		{
			printf("sigma %.8lf  %d\n", sigma, generatii);
		}
		++count;
		++generatii;
		if (generatii > 3000000)
		{
			break;
		}
	}
	printf("n\n");
	// ------------------------------------------------------------------------------------------------

	//noul bias01
	for (int j = 0; j < P; ++j)
	{
		printf("bias01[%d]= %.10lf \n", j, bias01[j]);
	}
	printf("\n");
	//noul bias12
	for (int k = 0; k < Q; ++k)
	{
		printf("bias12[%d]= %.10lf \n", k, bias12[k]);
	}


	printf("\n weights01 \n");
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < P; ++j)
		{
			printf("%.10lf ", weights01[i][j]);
		}
		printf("\n");
	}

	printf("\n weights12 \n");
	for (int j = 0; j < P; ++j)
	{
		for (int k = 0; k < Q; ++k)
		{
			printf("%.10lf ", weights12[j][k]);
		}
		printf("\n");
	}


	for (int qq = 0; qq < 4; ++qq)
	{
		readInputs(y0);
		readOutputs(y, y0);
		for (int i = 0; i < P; ++i)
		{
			y1[i] = -bias01[i];
			//printf("%y1[%d]=%lf \n", i, y1[i]);
			for (int j = 0; j < N; ++j)
			{
				y1[i] = y1[i] + y0[j] * weights01[j][i];
				//printf("   %0.4lf %0.4lf %0.4lf \n", y1[i], y0[j], weights01[i][j]);
			}
			//printf("%y1[%d]=%lf \n", i, y1[i]);
			y1[i] = sigmoid(y1[i]);
			//printf("%y1[%d]=%lf \n", i, y1[i]);
		}
		//printf("\n");
		//printf("\n");

		//calculate y2
		for (int j = 0; j < Q; ++j)
		{
			y2[j] = -bias12[j];
			//printf("y2[%d]=%lf \n", j, y2[j]);
			for (int k = 0; k < P; ++k)
			{
				y2[j] = y2[j] + y1[k] * weights12[k][j];
				//printf("    %0.4lf %0.4lf %0.4lf \n", y2[j], y1[k], weights12[j][k]);
			}
			//printf("%y2[%d]=%lf \n", j, y2[j]);
			y2[j] = sigmoid(y2[j]);
			//printf("%y2[%d]=%lf \n", j, y2[j]);
		}

		printf("--%lf %lf %lf %lf %lf %lf --\n", y0[0], y0[1], y2[0], y2[1], y2[2], y2[3]);
	}

	printf("err = %.6lf \n", sigma);
	printf("nr_generatii = %u\n", generatii);
	return 0;
}
