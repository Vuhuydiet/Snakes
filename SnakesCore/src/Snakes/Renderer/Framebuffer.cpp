#include "skpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace sk {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specs)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SK_ASSERT(true, "RendererAPI \"None\" is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return NewRef<OpenGLFramebuffer>(specs);
		}

		SK_ASSERT(true, "Unknown renderer API!");
		return nullptr;
	}

}