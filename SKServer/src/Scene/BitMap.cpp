#include "skpch.h"
#include "BitMap.h"

namespace sk {
	// Functions
	BitMap::BitMap(const std::filesystem::path& path) {
		Load(path);
	}

	void BitMap::Load(const std::filesystem::path& path) {
		std::ifstream fin(path);
		fin >> m_Height >> m_Width;
		m_Map = CreateMatrix<size_t>(m_Height, m_Width, 0);
		for(int i= 0; i < m_Height; i++)
			for(int j = 0; j < m_Width; j++)
				fin >> m_Map[i][j];
	}

	std::vector<size_t>& BitMap::operator[](size_t i) {
		return m_Map[i];
	}

	size_t& BitMap::operator[](const glm::vec2& pos) {
		return m_Map[pos.x][pos.y];
	}

	const std::vector<size_t>& BitMap::operator[](size_t i) const {
		return m_Map[i];
	}

	bool BitMap::IsInBounds(const glm::vec2& cell) const {
		return cell.x >= 0 && cell.x < m_Height && cell.y >= 0 && cell.y < m_Width;
	}

}