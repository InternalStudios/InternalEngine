#pragma once
#include "Vector.h"
#include <stdint.h>

template<typename T>
struct Vector<T, 2>
{
	Vector<T, 2>(T aX, T aY)
		: x(aX), y(aY)
	{
		
	}
	
	union { T x, r, s; };
	union { T y, g, t; };
};

template<typename T>
typedef Vector<T, 2> Vector2;

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int32_t> Vector2i;
typedef Vector2<uint32_t> Vector2ui;
