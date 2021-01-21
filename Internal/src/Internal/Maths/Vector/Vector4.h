#pragma once

#include "Vector.h"
#include <stdint.h>

template<typename T>
struct Vector<T, 4>
{
	union { T x, r, s; };
	union { T y, g, t; };
	union { T z, b, p; };
	union { T w, a, q; };
};