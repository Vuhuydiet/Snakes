#pragma once

#include "Snakes/Core/Core.h"

#include "Snakes/Utils/KeyCodes.h"
#include "Snakes/Utils/MouseCodes.h"
#include <glm/glm.hpp>

namespace sk {

	class Input
	{
	public:
		static bool Quit();

		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyReleased(KeyCode keycode);
		static bool IsKeyDown(KeyCode key);
		static bool IsKeyUp(KeyCode key);

		static bool IsMousePressed(MouseCode buttoncode);
		static bool IsMouseReleased(MouseCode buttoncode);
		static bool IsMouseDown(MouseCode button);
		static bool IsMouseUp(MouseCode button);

		static glm::vec2 GetMousePosition();
		static glm::vec2 GetMouseDeltaPos();
	};

}
