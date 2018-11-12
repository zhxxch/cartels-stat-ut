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
