#pragma once

#include "Internal/Renderer/Buffer.h"
#include "Internal/Types/Pointer.h"

namespace Internal
{
	class VertexArray
	{
	public:
	private:
		SharedPointer<VertexBuffer> m_VertexBuffer;
		SharedPointer<IndexBuffer> m_IndexBuffer;
	};
}