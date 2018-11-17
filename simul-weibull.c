#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"stat-synth.h"
#define _speck128u128_round 32
#include"zxcrypto-ymm.h"
#include"sum-ymm.h"
typedef double flux_t;
typedef double state_t;
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
		flux_t birth_at[], flux_t end_at[],
		const long long daysvalid, const double u01randoms[],
		const double lambda, const double k,
		const double mu, const long long max_days
		){
	if(daysvalid==max_days)return 0;
	if(*u01randoms < lambda){
		*birth_at = 1;
		const long long duration = (long long)(
				weibull_invPDF(u01i53(ru64_rdrand()), k, mu));
		if(duration < daysvalid){
			end_at[duration] -= 1;
		}
	}else *birth_at = 0;
	return life_process(
			birth_at-1, end_at-1, daysvalid+1, u01randoms-1,
			lambda, k, mu, max_days);
}
double count_alive(state_t alive_ints[],
		const long long max_days, const state_t alive,
		const flux_t birth_fluxs[]){
	if(max_days==0)return alive;
	*alive_ints = *birth_fluxs + alive;
	return count_alive(alive_ints+1, max_days-1, *alive_ints, birth_fluxs+1);
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
	const long long NumSimul = (long long)atof(argv[5]);
	state_t* AliveCounter
		= malloc(MaxDays*sizeof(double));
	flux_t* AliveCtrEnd = AliveCounter+MaxDays-1;
	double *AliveAvgs = malloc(NumSimul*sizeof(double));
	for(int i=0;i<NumSimul;i++){
		memset(AliveCounter, 0, MaxDays*sizeof(int));
		gen_uniform01_random((uint64_t*)AliveCounter, MaxDays);
		life_process(AliveCtrEnd, AliveCtrEnd, 0, AliveCtrEnd, Lambda, K, Mu, MaxDays);
		count_alive(AliveCounter, MaxDays,0,AliveCounter);
		AliveAvgs[i] = avgd(AliveCounter+1000, MaxDays-1000);
	}
	printf("%f\n", avgd(AliveAvgs,NumSimul));
	return 0;
}
