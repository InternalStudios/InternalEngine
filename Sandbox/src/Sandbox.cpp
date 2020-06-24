#include "Sandbox.h"

#include "Internal/Core/EntryPoint.h"

#include "Internal/Renderer/GraphicsContext.h"

Internal::GraphicsContexts Internal::GraphicsContext::s_GraphicsContext = Internal::GraphicsContexts::Vulkan;

Sandbox::Sandbox(const Internal::ApplicationData& data)
	: Internal::Application(data)
{

}