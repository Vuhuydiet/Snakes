#pragma once

#include "Snake.h"
#include "BitMap.h"

#include <Snakes.h>


namespace sk {

	struct SnakeSkin {
		glm::vec4 color = glm::vec4(1.0f);
		Ref<Texture2D> texture = nullptr;
	};



	class Scene {
	public:
		void LoadMap(const std::filesystem::path& path);

		void RegisterSnake(const Ref<Snake>& snake, const SnakeSkin& skin);
		void RemoveSnake(const UUID& uuid);
		bool IsSnakeAlive(const UUID& uuid) const;

	public:
		void Update(float ts);

		UUID GetUUID() const { return m_SceneUUID; }
		const BitMap& GetBitMap() const { return m_BitMap; }
		const std::unordered_map<UUID, SnakeSkin>& GetSnakeSkins() const { return m_SnakeSkins; }
	private:
		UUID m_SceneUUID;

		// game related
		BitMap m_BitMap;

		struct SnakeStatus {
			float timeCounter = 0.0f;
			Ref<Snake> ref;
		};
		std::unordered_map<UUID, SnakeStatus> m_Snakes;

		// skins
		std::unordered_map<UUID, SnakeSkin> m_SnakeSkins;
	};
}