#pragma once

#include <Snakes.h>
#include <list>
#include <glm/glm.hpp>

namespace sk {

	class Snake {
	public:
		Snake() = default;

		void SetVelocity(const glm::vec2& velocity);

		const glm::vec2& GetHead() const { return m_Body.front(); }
		const std::list<glm::vec2>& GetBody() const { return m_Body; }
		const glm::vec2& GetVelocity() const { return m_Velocity; }
		const UUID& GetID() const { return m_ID; }

		glm::vec2 GetNextMove() const;
		glm::vec2 Move(const glm::vec2& move);
		void AddSegmentToTheBack();
	private:
		std::list<glm::vec2> m_Body;
		// time elapses between each move. 0 means do not move
		glm::vec2 m_Velocity = { 0, 0 };

	private:
		UUID m_ID;
	};

}