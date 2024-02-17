#include "skpch.h"
#include "Snake.h"

namespace sk {

	void Snake::AddSegmentToTheBack() {
		if (m_Body.size() == 0) {
			m_Body.push_back({ 0, 0 });
		}
		else {
			m_Body.push_back(m_Body.back());
		}
	}

	void Snake::SetVelocity(const glm::vec2& vel) {
		m_Velocity = vel;
	}

	glm::vec2 Snake::GetNextMove() const {
		glm::vec2 head = m_Body.front();
		if (m_Velocity.x < 0) {
			head.x--;
		}
		else if (m_Velocity.x > 0) {
			head.x++;
		}
		else if (m_Velocity.y < 0) {
			head.y--;
		}
		else if (m_Velocity.y > 0) {
			head.y++;
		}
		return head;
	}

	glm::vec2 Snake::Move(const glm::vec2& move) {
		glm::vec2 head = move;
		m_Body.push_front(head);
		glm::vec2 ret = m_Body.back();
		m_Body.pop_back();
		return ret;
	}
}