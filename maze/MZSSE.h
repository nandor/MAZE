// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZSSE_H
#define MZSSE_H
#pragma once

namespace MAZE
{
	
	/**
		Allocates memory aligned on a 16 byte boundary
		so the much faster movaps can be used. Aligned
		memory comes with an overhead of 19 bytes.
	*/
	class Aligned
	{
	public:

		/**
			Allocate aligned memory
		*/
		void* operator new(size_t sz)
		{
			void *m, *p;

			if (!(m = p = malloc(sz + 0xF + sizeof(void*))))
			{
				throw std::bad_alloc();
			}

			p = (void*)(((size_t)p + 0x13) & ~0xF);
			((size_t*)p)[-1] = (size_t)m;

			return p;
		}

		/**
			Free aligned memory
		*/
		void operator delete(void *p)
		{
			free((void*)((size_t*)p)[-1]);
		}

		/**
			Create an array of dynamically allocated objects
		*/
		void* operator new[] (size_t sz)
		{
			return operator new(sz);
		}

		/**
			Delete an array of dynamically allocated elements
		*/
		void operator delete[] (void *p)
		{
			operator delete(p);
		}
	};

	/**
		Computes the dot product of two vectors
		@param a First vector
		@param b Second vector
		@return Vector with all members set to the dot product
	*/
	inline __m128 Dot(const __m128& a, const __m128& b)
	{
		__m128 tmp;
		
		tmp = _mm_mul_ps(a, b);
		tmp = _mm_hadd_ps(tmp, tmp);
		tmp = _mm_hadd_ps(tmp, tmp);

		return tmp;
	}

	/**
		Computes the cross product of two vectors
		@param a First vector
		@param b Second vector
		@return Cross product, w is equal to 0.0
	*/
	inline __m128 Cross(const __m128& a, const __m128& b)
	{ 
		return _mm_sub_ps(
			_mm_mul_ps(
				_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), 
				_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2))
			), 
			_mm_mul_ps(
				_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2)), 
				_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1))
			)
		);
	}

	/**
		Computes the length of a vector
		@param a Input vector
		@return Vector with all members set to length
	*/
	inline __m128 Length(const __m128& a)
	{
		__m128 t;

		t = _mm_mul_ps(a, a);
		t = _mm_hadd_ps(t, t);
		t = _mm_hadd_ps(t, t);

		return _mm_sqrt_ps(t);
	}

	/**
		Computes the intersection point of three planes
		@param a First plane
		@param b Second plane
		@param c Third plane
		@return Intersection point (if exists) or zero
	*/
	inline __m128 Intersect(const __m128& a, 
							const __m128& b, 
							const __m128& c)
	{
		glm::vec4 aa = glm::vec4(a.m128_f32[0], a.m128_f32[1], a.m128_f32[2], a.m128_f32[3]);
		glm::vec4 bb = glm::vec4(b.m128_f32[0], b.m128_f32[1], b.m128_f32[2], b.m128_f32[3]);
		glm::vec4 cc = glm::vec4(c.m128_f32[0], c.m128_f32[1], c.m128_f32[2], c.m128_f32[3]);

		glm::vec3 v = Intersect(aa, bb, cc);

		return _mm_setr_ps(v.x, v.y, v.z, 1.0f);
	}

	/**
		Inverts the sign of a vector VERY quickly by 
		changing the sign bit
	*/
	inline __m128 Invert(const __m128& v)
	{
		union
		{
			float fval;
			int ival;
		} mask;
		
		mask.ival = 0x80000000;

		return _mm_xor_ps(v, _mm_set_ps1(mask.fval));
	}

	/**
		Check if two points are on the same side of a line
	*/
	inline bool SameSide(const __m128& p1, 
						 const __m128& p2, 
						 const __m128& a, 
						 const __m128& b)
	{
		__m128 side1, side2, dot;
		
		side1 = Cross(_mm_sub_ps(b, a), _mm_sub_ps(p1, a));
		side2 = Cross(_mm_sub_ps(b, a), _mm_sub_ps(p2, a));

		dot = _mm_mul_ps(side1, side2);
		dot = _mm_hadd_ps(dot, dot);
		dot = _mm_hadd_ps(dot, dot);
	
		return _mm_movemask_ps(_mm_cmpge_ps(dot, _mm_set_ps1(-EPS))) == 0xF;
	}

	/**
		Check if a point is inside a triangle
	*/
	inline bool Inside(const __m128& a, 
					   const __m128& b, 
					   const __m128& c, 
					   const __m128& p)
	{
		return SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b);
	}
};

#endif
