#include "skpch.h"

#include <Snakes/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace sk {

	class SKServer : public Application {
	public:
		SKServer()
			: Application(WindowProps("Snakes Server", WINDOWPOS_CENTERED, WINDOWPOS_CENTERED,
				1280, 720, WINDOW_OPENGL | WINDOW_RESIZABLE | WINDOW_ALLOW_HIGHDPI)) 
		{
			PushLayer(new EditorLayer());
		}


	};
}

sk::Application* sk::CreateApplication() {
	return new SKServer();
}