#pragma once

#include "Snakes/Core/Core.h"
#include "Snakes/Renderer/GraphicsContext.h"

struct SDL_Window;

namespace sk {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(SDL_Window* window);

		virtual void Init() override;

		static void SetAttributes();

	private:
		SDL_Window* m_WindowHandle;
	};

}