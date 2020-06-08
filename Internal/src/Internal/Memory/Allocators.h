#pragma once

namespace Internal
{
	class Allocator
	{
	public:
		static void Init();
		template<typename T>
		static T* Allocate(size_t size);
	private:
		static void* s_Memory;
		static void* s_Pointer;
	};
}