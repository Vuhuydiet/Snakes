#pragma once

#include "Scene/Scene.h"

#include <Snakes.h>
#include <map>
#include <string>

namespace sk {

	class EditorLayer : public Layer {
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(float dt) override;
		virtual void OnImGuiRender() override;

	private:
		// UI
		void BeginDockspace();
		void EndDockspace();
		void OnImGuiRenderMenuBar();

		Ref<Scene> CreateEmptyScene(const std::string& map);
		Ref<Scene> CreateEmptyScene(const std::filesystem::path& map_path);
		void CloseScene();
		void CloseApp();

		// Networking
		void MapTransfer();
	private:
		bool m_IsRunning = true;
		std::map<UUID, Ref<Scene>> m_Scenes;
		Ref<Scene> m_SelectedScene = nullptr;

	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Texture2D> m_AppleTexture;

	private:
		Ref<Socket> m_RegisterManager;

		Ref<Socket> m_MapTransferer;
		Ref<std::thread> m_MapTransferThread;

	};
}

