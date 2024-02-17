#pragma once

#include <Snakes.h>

namespace sk {

	class GameLayer : public Layer {
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

	private:


	};
}