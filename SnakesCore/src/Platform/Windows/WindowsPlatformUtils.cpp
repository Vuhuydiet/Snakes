#include "skpch.h"
#include "Snakes/Utils/PlatformUtils.h"

#ifdef SK_PLATFORM_WINDOWS

#include <commdlg.h>
#include <SDL_syswm.h>

#include "Snakes/Core/Application.h"

namespace sk {
	namespace FileDialogs {

		std::filesystem::path OpenFile(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);

			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo((SDL_Window*)Application::Get().GetWindow().GetNativeWindow(), &wmInfo);
			ofn.hwndOwner = wmInfo.info.win.window;

			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetOpenFileNameA(&ofn) == TRUE)
			{
				return ofn.lpstrFile;
			}
			return std::string();
		}

		std::filesystem::path SaveFile(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);

			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo((SDL_Window*)Application::Get().GetWindow().GetNativeWindow(), &wmInfo);
			ofn.hwndOwner = wmInfo.info.win.window;

			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetSaveFileNameA(&ofn) == TRUE)
			{
				return ofn.lpstrFile;
			}
			return std::string();
		}

	}
}

#endif