/*	__ |  \   /   __|
	/     /    |
	___|  _/ _\ \___|
*/
#pragma once
#ifdef __cplusplus
extern "C"{
#endif
#include<stdio.h>
	float* tsvsread(FILE * File, const int HeadersIgnore,
			const int m, const int n,  const int lda, float* A){
		for(int i = 0; i < HeadersIgnore; i++){
			fscanf_s(File, "%*s");
		}
		for(int i = 0; i < m*n; i++){
			fscanf_s(File, "%f", A + (i % n) * lda + i / n);
		}
		return(A);
	}
	double* tsvdread(FILE * File, const int HeadersIgnore,
			const int m, const int n, const int lda, double* A){
		for(int i = 0; i < HeadersIgnore; i++){
			fscanf_s(File, "%*s");
		}
		for(int i = 0; i < m*n; i++){
			fscanf_s(File, "%lf", A + (i % n) * lda + i / n);
		}
		return(A);
	}
	FILE* tsvsprint(FILE * File, const char title[],
			const int m, const int n, const int lda, const float* A){
		fprintf(File, "%s", title);
		for(int i = 0; i < m*n; i++){
			fprintf(File, (i % n  == n-1) ? "\n%.16g" : "\t%.16g",
					A[(i % n) * lda + i / n]);
		}
		fprintf(File, "\n");
		return(File);
	}
	FILE* tsvdprint(FILE * File, const char title[],
			const int m, const int n, const int lda, const double* A){
		fprintf(File, "%s", title);
		for(int i = 0; i < m*n; i++){
			fprintf(File, (i % n  == 0) ? "\n%.16g" : "\t%.16g",
					A[(i % n) * lda + i / n]);
		}
		fprintf(File, "\n");
		return(File);
	}
#ifdef __cplusplus
}
#endif
