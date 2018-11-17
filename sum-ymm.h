#pragma once
/*  __ |  \   /   __|
 *    /     /    |
 *  ___|  _/ _\ \___|  */
#include<immintrin.h>
double sum_kahan(const double *arr, const long long len){
	long long i=0;
	double A = 0, C = 0;
		__m256d A4 = _mm256_set1_pd(0);
		__m256d C4 = _mm256_set1_pd(0);
	for(const long long len4 = len-4;i<len4;i+=4){
		__m256d y4 = _mm256_sub_pd(_mm256_loadu_pd(arr+i),C4);
		__m256d t4 = _mm256_add_pd(A4, y4);
		C4 = _mm256_sub_pd(_mm256_sub_pd(t4, A4), y4);
		A4 = t4;
	}
	for(;i<len;i++){
		double y = arr[i]-C;
		double t = A + y;
		C = (t - A) - y;
		A = t;
	}
	A4 = _mm256_hadd_pd(A4, A4);
	return(A4.m256d_f64[0]+A4.m256d_f64[2]+A);
}
