#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"stat-synth.h"
#include"zxcrypto-ymm.h"
typedef double probalility_t;
typedef int* counter_stream_t;
inline double avg(const int arr[], const long long num){
	double a = 0;
	for(long long i=0;i<num;i++){
		a+=(double)arr[i]/num;
	}
	return a;
}
inline double avgd(const double arr[], const long long num){
	double a = 0;
	for(long long i=0;i<num;i++){
		a+=arr[i]/num;
	}
	return a;
}
double* block_avg(double avg_arr_out[], const int *counter,
		const long long max_days, const long long block_days){
	for(long long i=0;i<max_days/block_days;i++){
		avg_arr_out[i] = avg(counter+i*block_days, block_days);
	}
	return avg_arr_out;
}
int cmpf64sort(const double *a, const double *b){
	return (*a<*b)?-1:1;
}
double inv_weibull_pdf(const double x,
		const double k, const double lambda){
	/*	weibull distribution RNG. */
	return lambda*pow(-log(1-x),1/k);
}
counter_stream_t birth_process(counter_stream_t counter,
		const double lambda, const double u01s[],
		const long long max_days){
	for(long long j=max_days;j--;){
		if(u01s[j]<lambda){
			counter[j] = 1;
		}
	}
	return counter;
}
counter_stream_t life_process(counter_stream_t counter,
		const double lambda, const double k, const double mu,
		const double u01s[], const long long max_days){
	for(long long j=max_days;j--;){
		if(u01s[j]<lambda){
			counter[j] = 1;
			const long long duration
				= (long long)inv_weibull_pdf(u01i53(
							ru64_rdrand()), k, mu);
			if((j+duration)<max_days){
				counter[j+duration] -= 1;
			}
		}
	}
	return counter;
}
counter_stream_t count_alive(counter_stream_t counter,
		const long long max_days){
	for(long long j=1;j<max_days;j++){
		counter[j] = counter[j-1]+counter[j];
	}
	return counter;
}
void gen_uniform01_random(uint64_t arr[], const long long num){
	for(long long i=0;i<num;i++){
		arr[i] = i;
	}
	uint64_t keys[32];
	speck128key32(keys, ru64_rdrand(), ru64_rdrand());
	long long n;
#pragma omp parallel for
	for(n=0;n<num;n+=8){
		speck4x128u128(arr+n, arr+n, keys);
	}
	speck4x128u128fin(arr, arr, keys, num);
#pragma omp parallel for
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
	int* AliveCounter = malloc(MaxDays*sizeof(int));
	double *AliveAvgs = malloc(NumSimul*sizeof(double));
	double *U01Rnds = malloc(MaxDays*sizeof(double));
	for(int i=0;i<NumSimul;i++){
		memset(AliveCounter, 0, MaxDays*sizeof(int));
		gen_uniform01_random((uint64_t*)U01Rnds, MaxDays);
		life_process(AliveCounter, Lambda, K, Mu, U01Rnds, MaxDays);
		count_alive(AliveCounter, MaxDays);
		AliveAvgs[i] = avg(AliveCounter, MaxDays);
	}
	printf("%f\n", avgd(AliveAvgs,NumSimul));
	return 0;
}
