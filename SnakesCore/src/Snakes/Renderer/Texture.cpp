#include "skpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace sk {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SK_ASSERT(true, "RendererAPI \"None\" is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return NewRef<OpenGLTexture2D>(width, height);
		}

		SK_ASSERT(true, "Unknown renderer API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SK_ASSERT(true, "RendererAPI \"None\" is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return NewRef<OpenGLTexture2D>(filepath);
		}

		SK_ASSERT(true, "Unknown renderer API!");
		return nullptr;
	}

}