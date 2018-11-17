#pragma once
/*  __ |  \   /   __|
 *    /     /    |
 *  ___|  _/ _\ \___|  */
#include<immintrin.h>
#include<stdint.h>
#include"zxcrypto.h"
#ifndef _speck128u128_round
#define _speck128u128_round 32
#endif
inline void speck4x128u128(uint64_t Ct[8],
		const uint64_t Pt[8], const uint64_t Key[32]){
	__m256i CtSr = _mm256_loadu_si256((const __m256i*)Pt);
	__m256i CtSl = _mm256_loadu_si256((const __m256i*)(Pt + 4));
	__m256i CtHi = _mm256_unpackhi_epi64(CtSr, CtSl);
	__m256i CtLo = _mm256_unpacklo_epi64(CtSr, CtSl);
	for(int i = 0; i < _speck128u128_round; i++){
		CtSr = _mm256_srli_epi64(CtHi, 8);
		CtSl = _mm256_slli_epi64(CtHi, 56);
		CtHi = _mm256_xor_si256(CtSr, CtSl);
		CtHi = _mm256_add_epi64(CtHi, CtLo);
		CtHi = _mm256_xor_si256(CtHi,
				_mm256_set1_epi64x(Key[i]));
		CtSr = _mm256_srli_epi64(CtLo, 61);
		CtSl = _mm256_slli_epi64(CtLo, 3);
		CtLo = _mm256_xor_si256(CtSr, CtSl);
		CtLo = _mm256_xor_si256(CtLo, CtHi);
	}
	CtSr = _mm256_unpacklo_epi64(CtLo, CtHi);
	CtSl = _mm256_unpackhi_epi64(CtLo, CtHi);
	_mm256_storeu_si256((__m256i*)Ct, CtSr);
	_mm256_storeu_si256((__m256i*)(Ct + 4), CtSr);
}
inline void speckinv4x128u128(uint64_t Pt[8],
		const uint64_t Ct[8], const uint64_t Key[32]){
	__m256i CtSr = _mm256_loadu_si256((const __m256i*)Ct);
	__m256i CtSl = _mm256_loadu_si256((const __m256i*)(Ct + 4));
	__m256i CtHi = _mm256_unpackhi_epi64(CtSr, CtSl);
	__m256i CtLo = _mm256_unpacklo_epi64(CtSr, CtSl);
	for(int i = _speck128u128_round; i--;){
		CtLo = _mm256_xor_si256(CtLo, CtHi);
		CtSr = _mm256_srli_epi64(CtLo, 3);
		CtSl = _mm256_slli_epi64(CtLo, 61);
		CtLo = _mm256_xor_si256(CtSr, CtSl);
		CtHi = _mm256_xor_si256(CtHi,
				_mm256_set1_epi64x(Key[i]));
		CtHi = _mm256_sub_epi64(CtHi, CtLo);
		CtSr = _mm256_srli_epi64(CtHi, 56);
		CtSl = _mm256_slli_epi64(CtHi, 8);
		CtHi = _mm256_xor_si256(CtSr, CtSl);
	}
	CtSr = _mm256_unpacklo_epi64(CtLo, CtHi);
	CtSl = _mm256_unpackhi_epi64(CtLo, CtHi);
	_mm256_storeu_si256((__m256i*)Pt, CtSr);
	_mm256_storeu_si256((__m256i*)(Pt + 4), CtSr);
}
inline void speck128key32(uint64_t RoundKeys[32],
		uint64_t KeyLo, uint64_t KeyHi){
	for(uint64_t i = 0; i < 32; i++){
		RoundKeys[i] = KeyLo;
		_SPECKXX(KeyHi, KeyLo, i);
	}
}
