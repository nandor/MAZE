// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
__m128 MAZE::Determinant(const __m128& a, const __m128& b, const __m128& c)
{
	__m128 d, aa, bb, cc;

	aa = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2));
	bb = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
	cc = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 1, 0, 2));
		
	d = _mm_sub_ps(
		_mm_mul_ps(a, _mm_mul_ps(bb, cc)), 
		_mm_mul_ps(c, _mm_mul_ps(bb, aa))
	);

	d = _mm_hadd_ps(d, d);
	d = _mm_hadd_ps(d, d);

	return d;
}
// ------------------------------------------------------------------------------------------------
__m128 MAZE::Intersect(const __m128& a, const __m128& b, const __m128& c)
{
	int aw, bw, cw;
	__m128 c0, c1, c2, dd, dn, v, t;

	dd = Determinant(a, b, c);
						
	aw = ((int*)&a)[3] ^ 0x80000000;
	bw = ((int*)&b)[3] ^ 0x80000000;
	cw = ((int*)&c)[3] ^ 0x80000000;

	c0 = a; ((int*)&c0)[0] = aw;
	c1 = b; ((int*)&c1)[0] = bw;
	c2 = c; ((int*)&c2)[0] = cw;
	t = Determinant(c0, c1, c2);
	((int*)&dn)[0] = ((int*)&t)[0];
				
	c0 = a; ((int*)&c0)[1] = aw;
	c1 = b; ((int*)&c1)[1] = bw;
	c2 = c; ((int*)&c2)[1] = cw;
	t = Determinant(c0, c1, c2);
	((int*)&dn)[1] = ((int*)&t)[1];
		
	c0 = a; ((int*)&c0)[2] = aw;
	c1 = b; ((int*)&c1)[2] = bw;
	c2 = c; ((int*)&c2)[2] = cw;
	t = Determinant(c0, c1, c2);
	((int*)&dn)[2] = ((int*)&t)[2];

	v = _mm_div_ps(dn, dd);
	_mm_store_ss((float*)&v + 3, _mm_set_ps1(1.0f));
		
	return v;
}

// ------------------------------------------------------------------------------------------------
bool MAZE::SameSide(const __m128& p1, const __m128& p2, const __m128& a, const __m128& b)
{
	__m128 side1, side2, dot;
		
	side1 = Cross(_mm_sub_ps(b, a), _mm_sub_ps(p1, a));
	side2 = Cross(_mm_sub_ps(b, a), _mm_sub_ps(p2, a));

	dot = _mm_mul_ps(side1, side2);
	dot = _mm_hadd_ps(dot, dot);
	dot = _mm_hadd_ps(dot, dot);
	
	return _mm_movemask_ps(_mm_cmpge_ps(dot, _mm_set_ps1(-EPS))) == 0xF;
}
