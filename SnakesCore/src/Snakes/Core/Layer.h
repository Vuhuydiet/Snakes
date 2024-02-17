#pragma once
#include "skpch.h"
#include "Snakes/Events/Event.h"

namespace sk {

	class Layer
	{
	public:
		Layer(const std::string& _name = "Layer") 
			: m_DebugName(_name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate(float dt) {}
		virtual void OnImGuiRender() {}

	private:
		std::string m_DebugName;
	};

}