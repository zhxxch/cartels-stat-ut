#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"stat-analy.h"
#include"stat-synth.h"
typedef double probalility_t;
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
int birth_process(int birth_days[],
		const int day_num, const int population,
		const int Length, const double p){
	if(Length==day_num)return population;
	if(u01i53(ru64_rdrand())<p){
		birth_days[0] = day_num;
		return birth_process(birth_days+1, day_num+1,
				population+1,Length, p);
	}
	return birth_process(birth_days, day_num+1, population, Length, p);
}
double inv_weibull_pdf(const double x,
		const double k, const double lambda){
	/*	weibull distribution RNG. */
	return lambda*pow(-log(x),1/k);
}
int sample_lifetime(int end_days[], const int num, const double k, const double lambda){

	return 0;
}
int count_alive(const int starts[], const int ends[],
		int counter[], const int max_days, const int num_coming){
	for(int j=1;j<max_days;j++){
		for(int i=0;i<num_coming;i++){
			if(j==starts[i]){
				counter[j] = counter[j-1]+1;
			}else if(j==ends[i]){
				counter[j]= counter[j-1]-1;
			}else counter[j] = counter[j-1];
		}}
	return 0;
}
int main(int argc, char *argv[]){
	if(argc<4)return printf("args : k lambda maxdays\n");
	const double K = atof(argv[1]);
	const double Lambda = atof(argv[2]);
	const int MaxDays = atoi(argv[3]);
	const int* StartDays = calloc(MaxDays/2, sizeof(int));
	const int* Lifetimes = calloc(MaxDays/2, sizeof(int));
	const int* AlivesCtr = calloc(MaxDays/2, sizeof(int));
	return 0;
}
