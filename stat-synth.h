#pragma once
#ifdef __cplusplus
extern "C"{
#endif
	inline unsigned long long ru64_rdrand(void){
		extern int _rdrand64_step(unsigned long long*);
		unsigned long long r;
		while(!_rdrand64_step(&r));
		return  r;
	}
#ifdef __cplusplus
} //extern_C
#endif
inline float u01i24(const int i){
	return (1.f / (float)(1 << 24))
		*(float)((int)(i & 0x00ffffff));
}
inline double u01i53(const long long lli){
	return (1. / (double)(1ll << 53))
		*(double)((long long)(lli & 0x001fffffffffffffull));
}

float n01u01s(const float u, const float v){
	const float pi = 3.14159265358979323846f;
	return(sqrtf(-2*logf(1-u))*cosf(2*pi*v));
}
double n01u01d(const double u, const double v){
	const double pi = 3.14159265358979323846;
	return(sqrt(-2*log(1-u))*cos(2*pi*v));
}
double expu01d(const double x, const double lambda){
	return -log(1-x)/lambda;
}
