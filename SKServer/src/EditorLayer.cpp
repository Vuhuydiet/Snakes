#include "EditorLayer.h"

#include "Const.h"

#include <Snakes.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SK_EXT ".sk"

#define _SERVER_IP 				"127.0.0.1"
#define _REGISTER_GAME_PORT		8080
#define _MAP_TRANSFER_PORT		8081

namespace sk {

	static glm::vec2 s_ViewportSize(800, 600);
	static glm::vec2 s_ViewportBounds[2];
	static std::unordered_map<int, Ref<Texture2D>> s_ObjectTextures;

	// TEMP
	Ref<Snake> vu;
	float v = 2.0f;

	void EditorLayer::OnAttach() {
		FramebufferSpecification specs;
		specs.width = 800;
		specs.height = 600;
		specs.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::Depth };
		m_Framebuffer = Framebuffer::Create(specs);

		m_AppleTexture = Texture2D::Create("resources/images/objects/apple.png");
		s_ObjectTextures[CELL_APPLE] = m_AppleTexture;

		// Networking
		m_RegisterManager = Socket::Create(SocketProps::AF::INET, SocketProps::Type::_SOCK_STREAM, SocketProps::Protocol::TCP);
		m_RegisterManager->Bind(_REGISTER_GAME_PORT);
		m_RegisterManager->Listen();

		m_MapTransferer = Socket::Create(SocketProps::AF::INET, SocketProps::Type::_SOCK_DGRAM, SocketProps::Protocol::UDP);
		m_MapTransferer->Bind(_MAP_TRANSFER_PORT);

		m_MapTransferThread = NewRef<std::thread>(&EditorLayer::MapTransfer, this);

		// TEMP
		std::filesystem::path map_path = "resources/bitmaps/map0.sk";
		auto s = CreateEmptyScene(map_path);
		m_SelectedScene = s;
		vu = NewRef<Snake>();
		for(int i = 0; i < 5; i++)
			vu->AddSegmentToTheBack();
		vu->SetVelocity({ v, 0 });
		s->RegisterSnake(vu, { COLOR_WHITE, nullptr});
	}

	void EditorLayer::OnDetach() {
		m_IsRunning = false;
		m_MapTransferThread->join();
		m_MapTransferThread = nullptr;
	}

	void EditorLayer::OnUpdate(float dt) {
		// TEMP
		if (m_SelectedScene->IsSnakeAlive(vu->GetID())) {
			if (Input::IsKeyDown(Key::A))
				vu->SetVelocity({ 0, -v });
			else if (Input::IsKeyDown(Key::D))
				vu->SetVelocity({ 0, v });
			else if (Input::IsKeyDown(Key::W))
				vu->SetVelocity({ -v, 0 });
			else if (Input::IsKeyDown(Key::S))
				vu->SetVelocity({ v, 0 });
		}

		// update game logic
		for (auto& [uuid, scene] : m_Scenes) {
			scene->Update(dt);
		}

		// update rendering (FOR SELECTED SCENE ONLY)
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			s_ViewportSize.x > 0.0f && s_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != s_ViewportSize.x || spec.height != s_ViewportSize.y))
		{
			m_Framebuffer->Resize((int)s_ViewportSize.x, (int)s_ViewportSize.y);
		}

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(67, 70, 75, 255);
		RenderCommand::Clear();
		m_Framebuffer->ClearAttachment(1, 0);
		if (m_SelectedScene) {
			const BitMap& mp = m_SelectedScene->GetBitMap();
			const auto& skins = m_SelectedScene->GetSnakeSkins();
			
			float left = 0;
			float right = mp.GetWidth();
			float bottom = mp.GetHeight();
			float top = 0;
			Renderer2D::BeginScene(glm::ortho(left, right, bottom, top));

			for (int i = 0; i < mp.GetHeight(); i++) {
				for (int j = 0; j < mp.GetWidth(); j++) {
					Quad quad;
					quad.size = { 1.0f, 1.0f };
					quad.transform = glm::translate(glm::mat4(1.0f), { j + 0.5f, i + 0.5f, 0.0f });

					size_t bit = mp[i][j];
					if (sk_found(s_ObjectTextures, bit)) {
						quad.texture = s_ObjectTextures[bit];
					}
					else if (bit == CELL_EMPTY) {
						quad.color = COLOR_GRAY;
					}
					else {
						quad.color = skins.at(bit).color;
						quad.texture = skins.at(bit).texture;
					}

					Renderer2D::DrawQuad(quad);
				}
			}

			Renderer2D::EndScene();
		}
		m_Framebuffer->Unbind();

	}

	void EditorLayer::OnImGuiRender() {
		BeginDockspace();

		OnImGuiRenderMenuBar();
		
		// actual window rendering

		// --------------- Scene Hierarchy ------------------------------------------------ //
		ImGui::Begin("Running Scenes");
		
		for (const auto& [uuid, scene] : m_Scenes) {
			ImGuiTreeNodeFlags flags =
				(m_SelectedScene == scene ? ImGuiTreeNodeFlags_Selected : 0) |
				ImGuiTreeNodeFlags_Leaf |
				ImGuiTreeNodeFlags_SpanAvailWidth;
			std::string node_name = std::to_string(uuid);
			bool opened = ImGui::TreeNodeEx((void*)(size_t)uuid, flags, node_name.c_str());

			if (ImGui::IsItemClicked())
				m_SelectedScene = scene;
			if (opened)
				ImGui::TreePop();
		}
		
		//End Running Scenes
		ImGui::End();

		// --------------- Viewport --------------------------------------------------------- //
		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::PopStyleVar();

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		s_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		s_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		// Resize
		ImVec2 windowAvailSize = ImGui::GetContentRegionAvail();
		s_ViewportSize = { windowAvailSize.x, windowAvailSize.y };

		size_t texIndex = m_Framebuffer->GetColorAttachmentID();
		ImGui::Image((void*)texIndex, ImVec2{ windowAvailSize.x, windowAvailSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// End Viewport
		ImGui::End();

		EndDockspace();
	}

	void EditorLayer::OnEvent(Event& event) {
		if (event.type() == QUIT)
			CloseApp();
	}

	void EditorLayer::OnImGuiRenderMenuBar() {
		if (!ImGui::BeginMenuBar())
			return;
		
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Create Scene")) {
				auto map_path = FileDialogs::OpenFile("Map files (*.sk)\0*.sk\0");
				m_SelectedScene = CreateEmptyScene(map_path);
			}
			if (ImGui::MenuItem("Close Scene"))
				CloseScene();
			ImGui::Separator();
			if (ImGui::MenuItem("Close"))
				CloseApp();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options")) {
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();

	}

	Ref<Scene> EditorLayer::CreateEmptyScene(const std::filesystem::path& map_path) {
		Ref<Scene> newScene = NewRef<Scene>();
		newScene->LoadMap(map_path);
		m_Scenes[newScene->GetUUID()] = newScene;

		return newScene;
	}

	Ref<Scene> EditorLayer::CreateEmptyScene(const std::string& map) {
		std::filesystem::path map_path = "resources/bitmaps/" + map + SK_EXT;

		return CreateEmptyScene(map_path);
	}

	void EditorLayer::CloseScene() {

	}

	void EditorLayer::CloseApp() {
		Application::Get().Close();
	}

	void EditorLayer::BeginDockspace() {
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			//ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f;
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;
	}

	void EditorLayer::EndDockspace() {
		ImGui::End();
	}

	void EditorLayer::MapTransfer() {
		std::string cli_ip;
		int cli_port = -1;
		UUID game_uuid = 0;
		int x, y, w, h;

		const long wait_time = 5; // ms
		while (m_IsRunning) {
			if (!m_MapTransferer->HasIncomingData(wait_time))
				continue;

			int req_type = BMTP::Server::GetRequest(
				m_MapTransferer, cli_ip, cli_port,
				game_uuid, 
				x, y, w, h
			);

			if (m_Scenes.find(game_uuid) == m_Scenes.end())
				continue;

			const auto& scene = m_Scenes[game_uuid];

			if (req_type == RQ_CELLSIZE) {
				// TEMP
				size_t cell_size = sizeof(size_t);
				
				BMTP::Server::SendCellSize(
					m_MapTransferer, cli_ip, cli_port,
					game_uuid, 
					cell_size
				);
			}
			else if (req_type == RQ_MAP) {
				// TEMP
				size_t cell_size = sizeof(size_t);
				void* buffer = malloc(cell_size * w * h);
				if (!buffer)
					continue;
				// TEMP
				// fill the buffer with the map data by copying each row
				int row_size = w * cell_size;
				for (int i = 0; i < h; i++) {
					memcpy((char*)buffer + row_size * i, scene->GetBitMap()[x + i].data() + y, row_size);
				}

				bool success = BMTP::Server::SendMap(
					m_MapTransferer, cli_ip, cli_port,
					game_uuid, 
					x, y, w, h,
					cell_size, buffer
				);
				free(buffer);
				buffer = nullptr;
			}
		}
	}

}

