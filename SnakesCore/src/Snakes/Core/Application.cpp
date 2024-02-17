#include "skpch.h"
#include "Application.h"

#include "Snakes/Core/Input.h"

#include "Snakes/Renderer/Renderer.h"

#include "Snakes/ImGui/ImGuiLayer.h"

#include "Snakes/Networking/Socket.h"

namespace sk {

	Application* Application::s_Instance = nullptr;

	Application::Application(WindowProps props)
		: m_LastFrameTime(0.0f),
		m_Running(true)
	{
		SK_ASSERT(s_Instance, "Already has an application!");
		s_Instance = this;

		m_Window = Scope<Window>(Window::Create(props));
		EventManager::Init();
		EventManager::SetCallBackFn(SK_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		Socket::Init();
	}

	Application::~Application()
	{
		EventManager::ShutDown();
		Renderer::Shutdown();
		s_Instance = nullptr;
	}

	void Application::OnEvent(Event& e)
	{
		m_Window->OnEvent(e);

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			if (e.handled)
				break;
			(*it)->OnEvent(e);
		}

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch(QUIT, SK_BIND_EVENT_FN(Application::OnQuitEvent));
		dispatcher.Dispatch(WINDOW_EVENT, SK_BIND_EVENT_FN(Application::OnWindowEvent));
	}

	void Application::Run()
	{
		while (IsRunning())
		{
			float currentTime = m_Clock.GetSeconds();
			float dt = currentTime - m_LastFrameTime;
			m_LastFrameTime = currentTime;
			if (dt > 0.1f)
			{
				m_Window->ClearEventQueue();
				continue;
			}

			m_Window->OnUpdate(dt);

			if (!m_Window->IsMinimized()) {
				for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
				{
					(*it)->OnUpdate(dt);
				}
			}

			m_ImGuiLayer->Begin();
			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
			{
				(*it)->OnImGuiRender();
			}
			m_ImGuiLayer->End();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnQuitEvent(Event& e)
	{
		Close();
	}

	void Application::OnWindowEvent(Event& event)
	{
		switch (event.WindowType())
		{
		case WINDOWEVENT_RESIZED:
		case WINDOWEVENT_SIZE_CHANGED:
			Renderer::OnWindowResize(m_Window->GetWidth(), m_Window->GetHeight());
			break;
		}
	}
}