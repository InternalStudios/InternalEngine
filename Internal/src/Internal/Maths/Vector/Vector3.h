#pragma once
#include "Vector.h"
#include <stdint.h>

template<typename T>
struct Vector<T, 3>
{
	Vector<T, 3>(T aX, T aY, T aZ)
		: x(aX), y(aY), z(aZ)
	{
		
	}
	
	union { T x, r, s; };
	union { T y, g, t; };
	union { T z, b, p; };
};

template<typename T>
typedef Vector<T, 3> Vector3;

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int32_t> Vector3i;
typedef Vector3<uint32_t> Vector3ui;
