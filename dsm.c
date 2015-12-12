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
		dsd = DSD_ConvertSamples(out);
		fwrite(&dsd,1 ,1, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}
