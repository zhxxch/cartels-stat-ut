#pragma once
/*   __ |  \   /   __|
 *     /     /    |
 *   ___|  _/ _\ \___|  */
#include<stdint.h>
#define _RR(x, r) (((x)>>r)|((x)<<((sizeof(x)*8)-r)))
#define _LL(x, r) (((x)<<r)|((x)>>((sizeof(x)*8)-r)))
#define _SPECKXXR(x, y, k) (y=_RR(x^y, 3), x=_LL((uint64_t)((x^(k))-y), 8))
#define _SPECKXX(Hi, Lo, k) (Hi=(_RR(Hi, 8)+Lo)^(k), Lo=_LL(Lo, 3)^Hi)
#ifdef __cplusplus
template<int _speck64u96_round = 26>
#else
#ifndef _speck64u96_round
#define _speck64u96_round 26
#endif
#endif
inline uint64_t speck64u96(const uint64_t Pt,
		uint32_t KeyLo, uint32_t KeyHiLo, uint32_t KeyHiHi){
	uint32_t CtHi = Pt >> 32,
	CtLo = Pt & 0xffffffffu;
	for(int i = 0; i < _speck64u96_round;)
		(_SPECKXX(CtHi, CtLo, KeyLo),
		 _SPECKXX(KeyHiLo, KeyLo, i++),
		 _SPECKXX(CtHi, CtLo, KeyLo),
		 _SPECKXX(KeyHiHi, KeyLo, i++));
	return((((uint64_t)CtHi) << 32) | CtLo);
}
