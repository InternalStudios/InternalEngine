#pragma once

namespace Internal
{
	enum RenderingAPIs
	{
		None = 0, OpenGL, Vulkan, DirectX, Metal
	};
	class RenderingAPI
	{
	public:
		virtual void ClearColor() = 0;
		virtual void OnUpdate() = 0;
	private:

	};
}