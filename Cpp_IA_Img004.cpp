#pragma comment(linker, "/STACK:990000000")
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>

#define layers 3
#define N 784
#define P 70
#define Q 10
//#define R 5
#define BATCH_SIZE 100
#define Nr_Training 50000
#define Nr_Tests 10000
#define Sigma_Limit 0.01
#define populatie 3
#define nrBatches 160
double alfa = 0.3;
unsigned int successes = 0;

using namespace std;

ifstream fTrain;
ifstream fTest;

int maxV(double vector[])
{
	int poz = 0;
	double max = 0.0;
	for (int i = 0; i < Q; ++i)
	{
		if (vector[i] > max)
		{
			poz = i;
			max = vector[i];
		}
	}
	return poz;
}

void readTraining(double y[], double y0[])
{
	//printf("TRaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
	int temp;
	fTrain >> temp;
	//cout << temp << endl;
	for (int i = 0; i < Q; ++i)
	{
		y[i] = 0;
	}
	y[temp] = 1.0;
	//cout << y[temp] << endl;
	for (int i = 0; i < Q; ++i)
	{
		//printf("%1.4lf - ", y[i]);
	}
	//cout << endl;
	//cout << y[5] << endl;
	for (int i = 0; i < N; ++i)
	{
		fTrain >> y0[i];
		//printf("%.4lf ", y0[i]);
	}
	//fTrain >> temp;
}
void readTest(double y[], double y0[])
{
	//printf("TRbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
	int temp;
	fTest >> temp;
	//cout << temp << endl;
	for (int i = 0; i < Q; ++i)
	{
		y[i] = 0;
	}
	y[temp] = 1.0;
	//cout << y[temp] << endl;
	for (int i = 0; i < Q; ++i)
	{
		//printf("%1.4lf - ", y[i]);
	}
	//cout << endl;
	//cout << y[5] << endl;
	for (int i = 0; i < N; ++i)
	{
		fTest >> y0[i];
		//printf("%.4lf ", y0[i]);
	}
	//fTest >> temp;
}
double sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

int main(void)
{
	fTrain.open("train.txt");
	fTest.open("test.txt");

	unsigned int nr_noduri[layers] = { N,P,Q };
	double weights01[populatie][N][P], weights12[populatie][P][Q];
	double deltaWeights01[N][P], deltaWeights12[P][Q];

	double bias01[populatie][P], bias12[populatie][Q];
	double deltaBias01[P], deltaBias12[Q];

	double y0[N], y1[P], y2[Q], y[Q];
	double erori[Q];
	double gradient01[P], gradient12[Q];
	double sigma[populatie] = { 1 };
	unsigned short int count = 0;
	unsigned long int generatii = 0;

	//randomize wights and biases
	//works: 5
	//not work: 14
	//srand(5);
	srand(4);
	for (int pop = 0; pop < populatie; ++pop) {
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < P; ++j)
			{
				weights01[pop][i][j] = double(rand() % 1000) / 500 * 2.4 / nr_noduri[0] - 2.4 / nr_noduri[0];
				//printf("%+0.6lf ", weights01[i][j]);
			}
			//printf("\n");
		}
	}
	//printf("\n");
	for (int pop = 0; pop < populatie; ++pop) {
		for (int i = 0; i < P; ++i)
		{
			for (int j = 0; j < Q; ++j)
			{
				weights12[pop][i][j] = double(rand() % 1000) / 500 * 2.4 / nr_noduri[0] - 2.4 / nr_noduri[0];
				//printf("%+0.6lf ", weights12[i][j]);
			}
			//printf("\n");
		}
	}
	//printf("\n");
	for (int pop = 0; pop < populatie; ++pop) {
		for (int i = 0; i < P; ++i)
		{
			bias01[pop][i] = double(rand() % 1000) / 500 * 2.4 / nr_noduri[0] - 2.4 / nr_noduri[0];
			//printf("%+0.6lf ", bias01[i]);
		}
	}
	//printf("\n\n");
	for (int pop = 0; pop < populatie; ++pop) {
		for (int i = 0; i < Q; ++i)
		{
			bias12[pop][i] = double(rand() % 1000) / 500 * 2.4 / nr_noduri[0] - 2.4 / nr_noduri[0];
			//printf("%+0.6lf ", bias12[i]);
		}
	}
	//printf("\n");
	//printf("1\n");
	for (int pop = 0; pop < populatie; ++pop)
	{
		sigma[pop] = 1;
	}
	//while (generatii < (Nr_Training / BATCH_SIZE) && (sigma > Sigma_Limit))
	for(int batch=0;batch<nrBatches;++batch)
	{
		//printf("2\n");
		for (int pop = 0; pop < populatie; ++pop) {
			sigma[pop] = 0;
		}
		//--sigma;

		//calculam delta pentru neuronii ascunsi
		for (int i = 0; i < N; ++i)
		{
			//cout << "y0[" << i << "]=" << y0[i] << endl;
			for (int j = 0; j < P; ++j)
			{
				//deltaWeights01[i][j] = 0;
				//printf("dw[%d][%d]=%.10lf   ", i, j, deltaWeights01[i][j]);
				//cout << deltaWeights01[i][j] << " __ ";
			}
			//printf("\n");
		}
		//printf("___ deltaBias01\n");
		for (int j = 0; j < P; ++j)
		{
			//deltaBias01[j] = 0;
			//printf("%.10lf ", deltaBias01[j]);
		}

		//caculam delta pentru stratul de iesire
		for (int j = 0; j < P; ++j)
		{
			for (int k = 0; k < Q; ++k)
			{
				//deltaWeights12[j][k] = 0;
				//printf("dw[%d][%d]=%.10lf   ", j, k, deltaWeights12[j][k]);
				//cout << deltaWeights12[j][k] << " ---" << endl;
			}
			//printf("\n");
		}
		//calculam delta pentru bias
		//printf("___ deltaBias12\n");
		for (int k = 0; k < Q; ++k)
		{
			//deltaBias12[k] = 0;
			//printf("%.10lf ", deltaBias12[k]);
		}

		
		for (int pop = 0; pop < populatie; ++pop)
		{
			for (int aa = 0; aa < BATCH_SIZE; ++aa)
			{
				readTraining(y, y0);

				//printf("Da?\n");
				//printf("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\t\taa = %d   wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n", aa);
				//read inputs -> expected outputs

				//calculate y1
				//printf("\n");
				for (int i = 0; i < P; ++i)
				{
					y1[i] = -bias01[pop][i];
					//printf("%y1[%d]=%lf \n", i, y1[i]);
					for (int j = 0; j < N; ++j)
					{
						y1[i] = y1[i] + y0[j] * weights01[pop][j][i];
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
					y2[j] = -bias12[pop][j];
					//printf("y2[%d]=%lf \n", j, y2[j]);
					for (int k = 0; k < P; ++k)
					{
						y2[j] = y2[j] + y1[k] * weights12[pop][k][j];
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
						suma = suma + gradient12[k] * weights12[pop][j][k];
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
				//double sum = 0;
				for (int a1 = 0; a1 < Q; ++a1)
				{
					sigma[pop] = sigma[pop] + (y[a1] - y2[a1]) * (y[a1] - y2[a1]);
				}
				//printf("s=%lf\n", sigma);
				//printf("\n weights01 \n");
				for (int i = 0; i < N; ++i)
				{
					for (int j = 0; j < P; ++j)
					{
						weights01[pop][i][j] = weights01[pop][i][j] + deltaWeights01[i][j];
					}
					//printf("\n");
				}

				//printf("\n weights12 \n");
				for (int j = 0; j < P; ++j)
				{
					for (int k = 0; k < Q; ++k)
					{
						weights12[pop][j][k] = weights12[pop][j][k] + deltaWeights12[j][k];
					}
					//printf("\n");
				}

				//noul bias01
				//printf("%.10lf %.10lf --------\n", deltaBias01[0], deltaBias01[1]);
				for (int j = 0; j < P; ++j)
				{
					//printf("bias01[%d]= %.10lf + %.10lf =  ", j, bias01[j], deltaBias01[j]);
					bias01[pop][j] = bias01[pop][j] + deltaBias01[j];
					//printf("%.10lf \n", bias01[j]);
				}
				//printf("%.10lf %.10lf --------\n", deltaBias01[0], deltaBias01[1]);
				//printf("%.10lf %.10lf --------\n", bias01[0], bias01[1]);
				//printf("\n");
				//noul bias12
				for (int k = 0; k < Q; ++k)
				{
					//printf("bias12[%d]= %.10lf + %.10lf =  ", k, bias12[k], deltaBias12[k]);
					bias12[pop][k] = bias12[pop][k] + deltaBias12[k] / 1;
					//printf(" %.10lf", bias12[k]);
				}
			}
		}
		for (int pop = 0; pop < populatie; ++pop)
		{
			sigma[pop] = sigma[pop] / BATCH_SIZE;
			printf("sigma %.8lf  %d\n", sigma[pop], generatii);
		}




		for (int p = 0; p < 4; ++p)
		{
			for (int k = 0; k < Q; ++k)
			{
				//sigma = sigma + (y[0] - y2[0]) * (y[0] - y2[0]);
			}
		}


		++generatii;
	}
	//printf("n\n");
	// ------------------------------------------------------------------------------------------------

	//noul bias01
	for (int j = 0; j < P; ++j)
	{
		//printf("bias01[%d]= %.10lf \n", j, bias01[j]);
	}
	//printf("\n");
	//noul bias12
	for (int k = 0; k < Q; ++k)
	{
		//printf("bias12[%d]= %.10lf \n", k, bias12[k]);
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



	int min = 0;

	for (int qq = 0; qq < Nr_Tests; ++qq)
	{
		//readInputs(y0);
		//readOutputs(y, y0);
		readTest(y, y0);
		for (int i = 0; i < P; ++i)
		{
			y1[i] = -bias01[min][i];
			//printf("%y1[%d]=%lf \n", i, y1[i]);
			for (int j = 0; j < N; ++j)
			{
				y1[i] = y1[i] + y0[j] * weights01[min][j][i];
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
			y2[j] = -bias12[min][j];
			//printf("y2[%d]=%lf \n", j, y2[j]);
			for (int k = 0; k < P; ++k)
			{
				y2[j] = y2[j] + y1[k] * weights12[min][k][j];
				//printf("    %0.4lf %0.4lf %0.4lf \n", y2[j], y1[k], weights12[j][k]);
			}
			//printf("%y2[%d]=%lf \n", j, y2[j]);
			y2[j] = sigmoid(y2[j]);
			//printf("%y2[%d]=%lf \n", j, y2[j]);
		}

		//printf("--%lf %lf %lf %lf %lf %lf --\n", y0[0], y0[1], y2[0], y2[1], y2[2], y2[3]);

		for (int hh = 0; hh < Q; ++hh)
		{
			//printf("%.4lf ", y2[hh]);
		}
		//printf("\n");
		for (int hh = 0; hh < Q; ++hh)
		{
			//printf("%.4lf ", y[hh]);
		}
		//printf("\n----------------------------------------------\n");

		int c1, c2;
		c1 = maxV(y);
		c2 = maxV(y2);
		if (c1 == c2)
		{
			successes++;
		}
		else
		{
			cout << qq << " " << c1 << " " << c2 << " " << y2[c2] << endl;
		}
	}

	printf("err = %.6lf \n", sigma[min]);
	printf("nr_generatii = %u\n", generatii);
	printf("rata_succes = %.4lf\n", double(successes) / Nr_Tests);

	fTrain.close();
	fTest.close();
	return 0;
}
