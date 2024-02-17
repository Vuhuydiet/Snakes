#pragma once

#include <glm/glm.hpp>

namespace sk {

	// Colors
	using Color = glm::vec4;
	
	const Color COLOR_RED = { 1.0f, 0.0f, 0.0f, 1.0f };
	const Color COLOR_GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
	const Color COLOR_BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
	const Color COLOR_YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
	const Color COLOR_MAGENTA = { 1.0f, 0.0f, 1.0f, 1.0f };
	const Color COLOR_WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
	const Color COLOR_BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
	const Color COLOR_GRAY = { 0.5f, 0.5f, 0.5f, 1.0f };
	const Color COLOR_LIGHT_GRAY = { 0.8f, 0.8f, 0.8f, 1.0f };
	const Color COLOR_DARK_GRAY = { 0.2f, 0.2f, 0.2f, 1.0f };
	const Color COLOR_TRANSPARENT = { 0.0f, 0.0f, 0.0f, 0.0f };
	const Color COLOR_ORANGE = { 1.0f, 0.5f, 0.0f, 1.0f };
	const Color COLOR_PURPLE = { 0.5f, 0.0f, 0.5f, 1.0f };
	const Color COLOR_PINK = { 1.0f, 0.5f, 0.5f, 1.0f };
	const Color COLOR_LIGHT_BLUE = { 0.5f, 0.5f, 1.0f, 1.0f };
	const Color COLOR_LIGHT_GREEN = { 0.5f, 1.0f, 0.5f, 1.0f };
	const Color COLOR_LIGHT_MAGENTA = { 1.0f, 0.5f, 1.0f, 1.0f };
	const Color COLOR_DARK_BLUE = { 0.0f, 0.0f, 0.5f, 1.0f };
	const Color COLOR_DARK_YELLOW = { 0.5f, 0.5f, 0.0f, 1.0f };
	const Color COLOR_DARK_MAGENTA = { 0.5f, 0.0f, 0.5f, 1.0f };
	const Color COLOR_GOLD = { 1.0f, 0.84f, 0.0f, 1.0f };
	const Color COLOR_SILVER = { 0.75f, 0.75f, 0.75f, 1.0f };
	const Color COLOR_BRONZE = { 0.8f, 0.5f, 0.2f, 1.0f };
	const Color COLOR_TEAL = { 0.0f, 0.5f, 0.5f, 1.0f };
	const Color COLOR_OLIVE = { 0.5f, 0.5f, 0.0f, 1.0f };
	const Color COLOR_MAROON = { 0.5f, 0.0f, 0.0f, 1.0f };
	const Color COLOR_NAVY = { 0.0f, 0.0f, 0.5f, 1.0f };
	const Color COLOR_LIME = { 0.0f, 1.0f, 0.0f, 1.0f };
	const Color COLOR_AQUA = { 0.0f, 1.0f, 1.0f, 1.0f };
	const Color COLOR_FUCHSIA = { 1.0f, 0.0f, 1.0f, 1.0f };
	const Color COLOR_LIGHT_SEA_GREEN = { 0.13f, 0.7f, 0.67f, 1.0f };
	const Color COLOR_DARK_SLATE_GRAY = { 0.18f, 0.31f, 0.31f, 1.0f };
	const Color COLOR_DARK_SLATE_GREY = { 0.18f, 0.31f, 0.31f, 1.0f };
	const Color COLOR_DARK_CYAN = { 0.0f, 0.55f, 0.55f, 1.0f };
	const Color COLOR_DARK_TURQUOISE = { 0.0f, 0.81f, 0.82f, 1.0f };
	const Color COLOR_MEDIUM_TURQUOISE = { 0.28f, 0.82f, 0.8f, 1.0f };
	const Color COLOR_TURQUOISE = { 0.25f, 0.88f, 0.82f, 1.0f };
	const Color COLOR_CYAN = { 0.0f, 1.0f, 1.0f, 1.0f };
	const Color COLOR_LIGHT_CYAN = { 0.88f, 1.0f, 1.0f, 1.0f };
	const Color COLOR_PALE_TURQUOISE = { 0.69f, 0.93f, 0.93f, 1.0f };
	const Color COLOR_AQUA_MARINE = { 0.5f, 1.0f, 0.83f, 1.0f };
	const Color COLOR_MEDIUM_AQUA_MARINE = { 0.4f, 0.8f, 0.67f, 1.0f };
	const Color COLOR_MEDIUM_SPRING_GREEN = { 0.0f, 0.98f, 0.6f, 1.0f };
	const Color COLOR_SPRING_GREEN = { 0.0f, 1.0f, 0.5f, 1.0f };
	const Color COLOR_MEDIUM_SEA_GREEN = { 0.24f, 0.7f, 0.44f, 1.0f };
	const Color COLOR_SEA_GREEN = { 0.18f, 0.55f, 0.34f, 1.0f };
	const Color COLOR_FOREST_GREEN = { 0.13f, 0.55f, 0.13f, 1.0f };
	const Color COLOR_DARK_GREEN = { 0.0f, 0.39f, 0.0f, 1.0f };
	const Color COLOR_DARK_OLIVE_GREEN = { 0.33f, 0.42f, 0.18f, 1.0f };
	const Color COLOR_DARK_SEA_GREEN = { 0.56f, 0.74f, 0.56f, 1.0f };
	const Color COLOR_LIME_GREEN = { 0.2f, 0.8f, 0.2f, 1.0f };
	const Color COLOR_LAWN_GREEN = { 0.49f, 0.99f, 0.0f, 1.0f };
	const Color COLOR_CHART_REUSE = { 0.5f, 1.0f, 0.0f, 1.0f };
	const Color COLOR_GREEN_YELLOW = { 0.68f, 1.0f, 0.18f, 1.0f };
	const Color COLOR_YELLOW_GREEN = { 0.6f, 0.8f, 0.2f, 1.0f };
	const Color COLOR_OLIVE_DRAB = { 0.42f, 0.56f, 0.14f, 1.0f };
	const Color COLOR_BEIGE = { 0.96f, 0.96f, 0.86f, 1.0f };
	const Color COLOR_IVORY = { 1.0f, 1.0f, 0.94f, 1.0f };
	const Color COLOR_LIGHT_YELLOW = { 1.0f, 1.0f, 0.88f, 1.0f };
	const Color COLOR_LIGHT_GOLDEN_ROD_YELLOW = { 0.98f, 0.98f, 0.82f, 1.0f };
	const Color COLOR_PAPAYA_WHIP = { 1.0f, 0.94f, 0.84f, 1.0f };
	const Color COLOR_MOCCASIN = { 1.0f, 0.89f, 0.71f, 1.0f };
	const Color COLOR_PEACH_PUFF = { 1.0f, 0.85f, 0.73f, 1.0f };
	const Color COLOR_PALE_GOLDEN_ROD = { 0.93f, 0.91f, 0.67f, 1.0f };
	const Color COLOR_KHAKI = { 0.94f, 0.9f, 0.55f, 1.0f };
	const Color COLOR_DARK_KHAKI = { 0.74f, 0.72f, 0.42f, 1.0f };
	const Color COLOR_GOLDEN_ROD = { 0.85f, 0.65f, 0.13f, 1.0f };
	const Color COLOR_DARK_GOLDEN_ROD = { 0.72f, 0.53f, 0.04f, 1.0f };
	const Color COLOR_PERU = { 0.8f, 0.52f, 0.25f, 1.0f };
	const Color COLOR_CHOCOLATE = { 0.82f, 0.41f, 0.12f, 1.0f };
	const Color COLOR_SADDLE_BROWN = { 0.55f, 0.27f, 0.07f, 1.0f };
	const Color COLOR_SIENNA = { 0.63f, 0.32f, 0.18f, 1.0f };
	const Color COLOR_BROWN = { 0.65f, 0.16f, 0.16f, 1.0f };
	const Color COLOR_DARK_RED = { 0.55f, 0.0f, 0.0f, 1.0f };
	const Color COLOR_FIRE_BRICK = { 0.7f, 0.13f, 0.13f, 1.0f };
	const Color COLOR_INDIAN_RED = { 0.8f, 0.36f, 0.36f, 1.0f };
	const Color COLOR_LIGHT_CORAL = { 0.94f, 0.5f, 0.5f, 1.0f };
	const Color COLOR_SALMON = { 0.98f, 0.5f, 0.45f, 1.0f };
	const Color COLOR_DARK_SALMON = { 0.91f, 0.59f, 0.48f, 1.0f };
	const Color COLOR_LIGHT_SALMON = { 1.0f, 0.63f, 0.48f, 1.0f };
	const Color COLOR_CORAL = { 1.0f, 0.5f, 0.31f, 1.0f };
	const Color COLOR_TOMATO = { 1.0f, 0.39f, 0.28f, 1.0f };
	const Color COLOR_ORANGE_RED = { 1.0f, 0.27f, 0.0f, 1.0f };
	const Color COLOR_DARK_ORANGE = { 1.0f, 0.55f, 0.0f, 1.0f };

}