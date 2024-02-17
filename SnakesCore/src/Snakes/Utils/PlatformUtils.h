#pragma once

#include <string>
#include <filesystem>

namespace sk {

	namespace FileDialogs {

		std::filesystem::path OpenFile(const char* filter);
		std::filesystem::path SaveFile(const char* filter);

	}

}