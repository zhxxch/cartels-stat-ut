#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"stat-synth.h"
#define _speck128u128_round 32
#include"zxcrypto-ymm.h"
#include"sum-ymm.h"
typedef double probalility_t;
typedef double* counter_stream_t;
double avgd(const double arr[], const long long num){
	const double a = sum_kahan(arr, num);
	return a/num;
}
int cmpf64sort(const double *a, const double *b){
	return (*a<*b)?-1:1;
}
double weibull_invPDF(const double x,
		const double k, const double lambda){
	/*	weibull distribution RNG. */
	return lambda*pow(-log(1-x),1/k);
}
int life_process(
		const double lambda, const double k,
		const double mu, const long long max_days,
		counter_stream_t ctr_begin, counter_stream_t ctr_end
		){
	if(ctr_begin+max_days < ctr_end)return 0;
	if(ctr_begin[0] < lambda){
		ctr_begin[0] = 1;
		const long long duration = (long long)
			(0.5+weibull_invPDF(u01i53(ru64_rdrand()), k, mu));
		if(ctr_begin+duration < ctr_end){
			ctr_begin[duration] -= 1;
		}
	}else ctr_begin[0] = 0;
	return life_process(lambda, k, mu, max_days,
			ctr_begin-1, ctr_end);
}
double count_alive(counter_stream_t counter,
		const long long max_days){
	if(max_days==1)return counter[0];
	counter[1] = counter[1]+counter[0];
	return count_alive(counter+1, max_days-1);
}
void gen_uniform01_random(uint64_t arr[], const long long num){
	long long n;
#pragma omp parallel for num_threads(2) schedule(static, 1)
	for(n=0;n<num;n++){
		arr[n] = n;
	}
	uint64_t keys[32];
	speck128key32(keys, ru64_rdrand(), ru64_rdrand());
#pragma omp parallel for num_threads(2) schedule(static, 1)
	for(n=0;n<num;n+=8){
		speck4x128u128(arr+n, arr+n, keys);
	}
#pragma omp parallel for num_threads(2) schedule(static, 1)
	for(n=0;n<num;n++){
		((double*)arr)[n] = u01i53(arr[n]);
	}
}
int main(int argc, char *argv[]){
	if(argc<5)return printf("args : lambda k mu maxdays numsimul\n");
	const double Lambda = atof(argv[1]);
	const double K = atof(argv[2]);
	const double Mu = atof(argv[3]);
	const long long MaxDays = (long long)atof(argv[4]);
	const long long NumSimul = atoll(argv[5]);
	counter_stream_t AliveCounter
		= malloc(MaxDays*sizeof(double));
	double *AliveAvgs = malloc(NumSimul*sizeof(double));
	for(int i=0;i<NumSimul;i++){
		memset(AliveCounter, 0, MaxDays*sizeof(int));
		gen_uniform01_random((uint64_t*)AliveCounter, MaxDays);
		life_process(Lambda, K, Mu, MaxDays,
				AliveCounter+MaxDays-1, AliveCounter+MaxDays);
		count_alive(AliveCounter, MaxDays);
		AliveAvgs[i] = avgd(AliveCounter+1000, MaxDays-1000);
	}
	printf("%f\n", avgd(AliveAvgs,NumSimul));
	return 0;
}
