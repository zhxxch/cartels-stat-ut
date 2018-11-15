#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"zxtsv.h"
double weibull_k_eq(const double x[], const double k, const int N){
	double a=0;
	double b=0;
	double c=0;
	for(int i=0;i<N;i++){
		a += pow(x[i], k)*log(x[i]);
	}
	for(int i=0;i<N;i++){
		b += pow(x[i], k);
	}
	for(int i=0;i<N;i++){
		c += log(x[i]);
	}
	return(a/b-1/k-(1./N)*c);
}
double weibull_lambda(const double x[], const double k, const int N){
	double avg = 0;
	for(int i=0;i<N;i++){
		avg+=(1.0/N)*pow(x[i], k);
	}
	return avg;
}
double weibull_avg(const double lambda, const double k){
	return lambda*tgamma(1+1/k);
}
double bsolv_weibull_eq(const double x[], const int N, const double min, const double max){
	const double mid = (max+min)/2;
	if(max-min<1e-9)return mid;
	const double eq_at_min = weibull_k_eq(x, min, N);
	const double eq_at_mid = weibull_k_eq(x, mid, N);
	const double eq_at_max = weibull_k_eq(x, max, N);
	if(eq_at_min*eq_at_mid<=0){
		return bsolv_weibull_eq(x, N, min, mid);
	}
	if(eq_at_max*eq_at_mid<=0){
		return bsolv_weibull_eq(x, N, mid, max);
	}
	return NAN;
}
int main(int argc, char* argv[]){
	if(argc<4)return printf("args : Length min max < data.tsv");
	const int Length = atoi(argv[1]);
	const double min = atof(argv[2]);
	const double max = atof(argv[3]);
	double *Arr = calloc(Length, sizeof(double));
	tsvdread(stdin, 1,Length,1,Length, Arr);
	const double k = bsolv_weibull_eq(Arr, Length, min, max);
	const double lambda = weibull_lambda(Arr, k, Length);
	const double Avg = weibull_avg(lambda, k);
	printf("k = %f\nlambda = %f\nAvg = %f\n", k, lambda, Avg);
	return 0;
}
