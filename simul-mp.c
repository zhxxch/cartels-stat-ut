#include<stdio.h>
#include<stdlib.h>
#include"stat-analy.h"
#include"stat-synth.h"
#include"zxtsv.h"

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
		const int num_alive, const double p){
	if(population==0)return num_alive;
	for(int n = birth_days[0]; n<max_days;n++){
		if(u01i53(ru64_rdrand())<p){
			end_days[0] = n;
			return death_process(birth_days+1, end_days+1, max_days, population-1,num_alive-1, p);
		}
	}
	return death_process(birth_days+1, end_days+1, max_days, population-1,num_alive, p);
}
double inter_arrival_time(const int birth_days[], const int num){
	double avg_time = 0;
	for(int i=1;i<num;i++){
		avg_time+=(double)(birth_days[i]-birth_days[i-1])/(num-1.0);
	}
	return avg_time;
}
int count_alive(const int birth_days[], const int end_days[],
		const int num, const int num_alive){
	if(num==0)return num_alive;
	if(birth_days[0]<end_days[0])return count_alive(birth_days+1, end_days+1, num-1, num_alive+1);
	return count_alive(birth_days+1, end_days+1, num-1, num_alive);
}
double duration(const int birth_days[], const int end_days[], const int num, const int num_alive){
	double avg_duration = 0;
	for(int i=0;i<num;i++){
		if(birth_days[i]<end_days[i]){
			avg_duration+=(double)(end_days[i]-birth_days[i])/(num-num_alive);
		}
	}
	return avg_duration;
}
int main(int argc, char *argv[]){
	if(argc<4)return printf("args : Days Pr(birth) Pr(mort)");
	const int Length = atoi(argv[1]);
	probalility_t birth = atof(argv[2]);
	probalility_t mortal = atof(argv[3]);
	int *StartDay = calloc(Length, sizeof(int));
	int *EndDay = calloc(Length, sizeof(int));
	const int NumBirth = birth_process(StartDay, 0, 0, Length, birth);
	const int NumAlive = death_process(StartDay, EndDay, Length, NumBirth, NumBirth, mortal);
	printf("Num = %i, Avg-inter-arrival = %f\n", NumBirth, inter_arrival_time(StartDay, NumBirth));
	printf("Alive %i, Avg-duration = %f\n", NumAlive, duration(StartDay, EndDay, NumBirth, NumAlive));
	return 0;
}
