#include "skpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace sk {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_ServerLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("CORE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_ServerLogger = spdlog::stdout_color_mt("SERVER");
		s_ServerLogger->set_level(spdlog::level::trace);
	}

}