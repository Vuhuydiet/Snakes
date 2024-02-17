#include "skpch.h"
#include "Scene.h"

namespace sk {

	void Scene::LoadMap(const std::filesystem::path& path) {
		m_BitMap.Load(path);
	}

	void Scene::RegisterSnake(const Ref<Snake>& snake, const SnakeSkin& skin) {
		m_Snakes[snake->GetID()] = { 0.0f, snake};
		m_SnakeSkins[snake->GetID()] = skin;
	}

	void Scene::RemoveSnake(const UUID& uuid) {
		m_Snakes.erase(uuid);
	}

	bool Scene::IsSnakeAlive(const UUID& uuid) const {
		return sk_found(m_Snakes, uuid);
	}

	void Scene::Update(float ts) {
		std::vector<UUID> to_remove;
		for (auto& [uuid, snake_status] : m_Snakes) {
			auto vel = snake_status.ref->GetVelocity();
			snake_status.timeCounter += ts;
			if (snake_status.timeCounter * fabs(vel.x) < 1.0f && snake_status.timeCounter * fabs(vel.y) < 1.0f)
				continue;

			snake_status.timeCounter = 0.0f;

			auto next_move = snake_status.ref->GetNextMove();
			if (!m_BitMap.IsInBounds(next_move)) {
				snake_status.timeCounter = 0.0f;
				continue;
			}

			// clear the snake
			for (const auto& pos : snake_status.ref->GetBody()) {
				m_BitMap[pos.x][pos.y] = CELL_EMPTY;
			}

			// move into another snake
			if (m_BitMap[next_move.x][next_move.y] != uuid && sk_found(m_Snakes, m_BitMap[next_move.x][next_move.y])) {
				to_remove.push_back(uuid);
				continue;
			}
			// eat apple
			if (m_BitMap[next_move.x][next_move.y] == CELL_APPLE) {
				auto old_tail = snake_status.ref->Move(next_move);
				snake_status.ref->AddSegmentToTheBack();
			}
			// move into empty cell
			else {
				auto old_tail = snake_status.ref->Move(next_move);
			}

			// redraw snake
			for (const auto& pos : snake_status.ref->GetBody()) {
				m_BitMap[pos] = uuid;
			}

		}

		for (const auto& uuid : to_remove) {
			RemoveSnake(uuid);
		}
	}

}