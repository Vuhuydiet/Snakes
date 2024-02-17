#pragma once

#include "Snakes/Core/Layer.h"
#include "Snakes/Core/Window.h"

namespace sk {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;

		void Begin();
		void End();

		void SetDarkThemeColors();
	};

}