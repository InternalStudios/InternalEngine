#include "Allocators.h"
#include <cstdlib>

namespace Internal
{
	template<typename T>
	T* Internal::Allocator::Allocate(size_t size)
	{
		void* position = s_Pointer;
		s_Pointer += size;
		return (T*)position;
	}
	void Allocator::Init()
	{
		s_Memory = malloc(2147482548);
		s_Pointer = s_Memory;
	}
}