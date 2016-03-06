#include <stdio.h>
int acc = 0;
char DSD_ConvertSamples(short int input)
{//1 order Delta_Sigma Modulator
	int feedback;
	if(acc > 0)
		feedback = 32768;
	else
		feedback = -32768;
	acc += input - feedback;
	if(acc > 0)
		return 100;
	else
		return -100;
}
int acc_1 = 0;//Integrator1
int acc_2 = 0;//Integrator 2
char DSD_ConvertSamples_2(short int input)
{//2 order Delta_Sigma Modulator
	int feedback;//which is 1-bit DAC
	if(input > 32767)
		input = 32767;
	if(input < -32767)
		input = -32767;
	if(acc_2 > 0)
		feedback = 32767;
	else
		feedback = -32767;
	acc_1 += input - feedback;
	acc_2 += acc_1 - feedback;
	if(acc_2 > 0)
		return 100;
	else
		return -100;	
}

float acc_4_1 = 0.133544;/* Magic number */
float acc_4_2 = -0.040005;
float acc_4_3 = -0.015113;
float acc_4_4 = -0.018597;
int dithering = 0;
char DSD_ConvertSamples_4(float input)
{//a1=1/3, a2=3/25, a3=1/10 a4=1/10 b1=6/5 b6=1 c1=1/6
	float feedback1, feedback2, feedback3;
	if(input > 1.0)
		input = 1.0;
	if(input < -1.0)
		input = -1.0;
	
	input *= 0.8;
	/*if(input < 1E-6 && input > -1E-6)
	{
		if(dithering)
		{
			input = 1E-5;
			dithering = 0;
		}
		else
		{
			input = -1E-5;
			dithering = 1;
		}
	}*/
	
	if(acc_4_4 > 0.0)
		feedback1 = 1.0;
	else
		feedback1 = -1.0;
	
	acc_4_1 += (1.0/3.0) * (input - feedback1);
	feedback2 = (6.0/5.0) * acc_4_1;
	
	acc_4_2 += (3.0/25.0) * acc_4_1;
	feedback3 = 1.0 * acc_4_2;

	acc_4_3 += (1.0/10.0) * (acc_4_2 - (1.0/6.0) * acc_4_4);
	acc_4_4 += (1.0/10.0) * (acc_4_3 + feedback2 + feedback3 - feedback1);
	
	if(acc_4_4 > 0.0)
		return 100;
	return -100;
}

int main()
{
	FILE *fp1, *fp2;
	short int out;
	char dsd;

	fp1 = fopen("sine.wav", "rb");
	fp2 = fopen("sine.dsd", "wb");
	while(!feof(fp1))
	{//No oversampling
		fread(&out, 2, 1, fp1);
		dsd = DSD_ConvertSamples_2(out);
		fwrite(&dsd,1 ,1, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}
