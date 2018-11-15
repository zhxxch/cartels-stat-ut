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
int death_process(const int birth_days[], int end_days[],
		const int max_days, const int population,
		const int num_alive, const double mu){
}
int main(int argc, char *argv[]){
	return 0;
}
