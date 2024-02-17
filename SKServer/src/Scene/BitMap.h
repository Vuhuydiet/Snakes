#pragma once
#include <Snakes.h>

#include <filesystem>

// TEMP
#define CELL_EMPTY 0
#define CELL_APPLE 1

namespace sk {

	class BitMap {
	public:
		BitMap() = default;
		explicit BitMap(const std::filesystem::path& path);
		void Load(const std::filesystem::path& path);

		std::vector<size_t>& operator[](size_t i);
		const std::vector<size_t>& operator[](size_t i) const;
		size_t& operator[](const glm::vec2& pos);

		size_t GetHeight() const { return m_Height; }
		size_t GetWidth() const { return m_Width; }
		bool IsInBounds(const glm::vec2& position) const;
	private:
		size_t m_Height = 0;
		size_t m_Width = 0;
		matrix<size_t> m_Map;
	};
}