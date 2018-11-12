#include<stdio.h>
#include<stdlib.h>
#include"stat-analy.h"
#include"stat-synth.h"
#include"zxtsv.h"
inline double avg(double arr[], int num){
	double a = 0;
	for(int i=0;i<num;i++){
		a+=arr[i]/num;
	}
	return a;
}
int cmpf64sort(const double *a, const double *b){
	return (*a<*b)?-1:1;
}
typedef double probability_t;
int main(int argc, char* argv[]){
	if(argc<3)return printf("args : length lambda < samples.tsv");
	const int Length = atoi(argv[1]);
	const double Lambda = atof(argv[2]);
	printf("lambda = %f, 1/lambda = %f\n", Lambda, 1/Lambda);
	double *Samples = calloc(Length, sizeof(double));
	tsvdread(stdin, 1, Length, 1, Length, Samples);
	printf("tsvavg = %f\n", avg(Samples, Length));
	double *SimulArr = calloc(Length, sizeof(double));
	for(int i=0;i<Length;i++){
		Samples[i] = exp_cdf(Samples[i], Lambda);
		SimulArr[i] = expu01d(u01i53(ru64_rdrand()), Lambda);
	}
	printf("simulavg = %f\n", avg(SimulArr, Length));
	for(int i=0;i<Length;i++){
		SimulArr[i] = exp_cdf(SimulArr[i], Lambda);
	}
	qsort(Samples, Length, sizeof(double), cmpf64sort);
	qsort(SimulArr, Length, sizeof(double), cmpf64sort);
	const probability_t ksu_sample
		= ks_cdf(ks_test_eval(Samples, Length, 1),Length);
	const probability_t ksl_sample
		= ks_cdf(ks_test_eval(Samples, Length, 0),Length);
	printf("Sample : u = %f, l = %f\n", ksu_sample, ksl_sample);
	const probability_t ksu_simul
		= ks_cdf(ks_test_eval(SimulArr, Length,1),Length);
	const probability_t ksl_simul
		= ks_cdf(ks_test_eval(SimulArr, Length,1),Length);
	printf("Simul : u = %f, l = %f\n", ksu_simul, ksl_simul);
	return 0;
}
