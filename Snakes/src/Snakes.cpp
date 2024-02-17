#include "skpch.h"
#include <Snakes/Core/EntryPoint.h>

#include "GameLayer.h"

namespace sk {
	class Snakes : public Application {
	public:
		Snakes()
			: Application(WindowProps("Snakes", WINDOWPOS_CENTERED, WINDOWPOS_CENTERED,
				1280, 720, WINDOW_OPENGL | WINDOW_ALLOW_HIGHDPI))
		{
			PushLayer(new GameLayer());
		}
	};
}

sk::Application* sk::CreateApplication() {
	return new Snakes();
}
