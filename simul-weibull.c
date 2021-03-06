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
	return a / num;
}
int cmpf64sort(const double *a, const double *b){
	return (*a < *b) ? -1 : 1;
}
double weibull_invPDF(const double x,
	const double k, const double lambda){
/*	weibull distribution RNG. */
	return lambda * pow(-log(1 - x), 1 / k);
}
void life_process(
	flux_t birth_at[], flux_t end_at[],
	unsigned long long rng[40],
	const double lambda, const double k,
	const double mu, const long long max_days
){
	for(long long daysvalid=0;
		daysvalid < max_days;daysvalid++){
		rng[0] += daysvalid;
		speckw128u128(rng, rng + 4, rng + 8);
		if(u01i53(rng[0]) < lambda){
			*birth_at = 1;
			const long long duration = (long long)(
				weibull_invPDF(u01i53(rng[4]), k, mu));
			if(duration < daysvalid){
				end_at[duration] -= 1;
			}
		} else *birth_at = 0;
		end_at--;
		birth_at--;
	}
}
state_t count_alive(state_t alive_ints[],
	const long long max_days, const state_t alive,
	const flux_t birth_fluxs[]){
	if(max_days == 0)return alive;
	*alive_ints = *birth_fluxs + alive;
	return count_alive(alive_ints + 1, max_days - 1, *alive_ints, birth_fluxs + 1);
}
int main(int argc, char *argv[]){
	if(argc < 5)return printf("args : lambda(birth) k(weibull) mu(weibull) maxdays numsimul\n");
	const double Lambda = atof(argv[1]);
	const double K = atof(argv[2]);
	const double Mu = atof(argv[3]);
	const long long MaxDays = (long long)atof(argv[4]);
	const long long NumSimul = (long long)atof(argv[5]);
	state_t* AliveCounter
		= malloc(MaxDays * sizeof(double));
	flux_t* AliveCtrEnd = AliveCounter + MaxDays - 1;
	double *AliveAvgs = malloc(NumSimul * sizeof(double));
	unsigned long long rng_stat[40] = {0};
	for(int i = 0; i < NumSimul; i++){
		speck128key32(rng_stat + 8, ru64_rdrand(), ru64_rdrand());
		speck128key32T(rng_stat + 8, rng_stat + 8);
		life_process(AliveCtrEnd, AliveCtrEnd,
			rng_stat, Lambda, K, Mu, MaxDays);
		count_alive(AliveCounter, MaxDays, 0, AliveCounter);
		AliveAvgs[i] = avgd(AliveCounter + 1000, MaxDays - 1000);
	}
	printf("%f\n", avgd(AliveAvgs, NumSimul));
	return 0;
}
