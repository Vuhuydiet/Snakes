#include "skpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace sk {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SK_ASSERT(true, "RendererAPI \"None\" is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return NewRef<OpenGLVertexArray>();
		}

		SK_ASSERT(true, "Unknown renderer API!");
		return nullptr;
	}


}