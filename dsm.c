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
