#pragma once

#include "Core.h"

#include "Window.h"
#include "Clock.h"

#include "LayerStack.h"
#include "Snakes/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace sk {

	class Application
	{
	public:
		Application(WindowProps props = WindowProps());
		virtual ~Application();

		bool IsRunning() const { return m_Running; }
		void Close() { m_Running = false; }

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }
	
	protected:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	
	private:
		void OnEvent(Event& e);

		void OnQuitEvent(Event& e);
		void OnWindowEvent(Event& e);

	private:
		void Run();
		bool m_Running;

		Clock m_Clock;
		float m_LastFrameTime;
	private:
		Scope<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}

