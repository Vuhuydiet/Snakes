#pragma once

#include "Core.h"
#include "Log.h"
#include "Application.h"
#include "Snakes/Debug/Instrumentor.h"

extern sk::Application* sk::CreateApplication();

int main(int argc, char** argv)
{
	sk::Log::Init();

	SK_PROFILE_BEGIN_SESSION("Startup", "HudiProfile-Startup.json");
	sk::Application* app = sk::CreateApplication();
	SK_PROFILE_END_SESSION();

	SK_PROFILE_BEGIN_SESSION("Runtime", "HudiProfile-Runtime.json");
	app->Run();
	SK_PROFILE_END_SESSION();

	SK_PROFILE_BEGIN_SESSION("Shutdown", "HudiProfile-Shutdown.json");
	delete app;
	SK_PROFILE_END_SESSION();

	return 0;
}
