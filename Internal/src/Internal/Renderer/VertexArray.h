#pragma once

#include "Internal/Renderer/Buffer.h"
#include "Internal/Types/Pointer.h"

namespace Internal
{
	class VertexArray
	{
	public:
	private:
		Pointer<VertexBuffer> m_VertexBuffer;
		Pointer<IndexBuffer> m_IndexBuffer;
	};
}