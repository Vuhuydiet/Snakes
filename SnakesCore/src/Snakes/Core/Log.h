#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <memory>

namespace sk {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetServerLogger() { return s_ServerLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_ServerLogger;
	};

}

#if defined(SK_LIB)
	#define SK_TRACE(x, ...)			::sk::Log::GetCoreLogger()->trace(x, __VA_ARGS__)
	#define SK_INFO(x, ...)				::sk::Log::GetCoreLogger()->info(x, __VA_ARGS__)
	#define SK_WARN(x, ...)				::sk::Log::GetCoreLogger()->warn(x, __VA_ARGS__)
	#define SK_ERROR(x, ...)			::sk::Log::GetCoreLogger()->error(x, __VA_ARGS__)
	#define SK_CRITICAL(x, ...)			::sk::Log::GetCoreLogger()->critical(x, __VA_ARGS__)
#elif defined(SK_APP)
	#define SK_TRACE(x, ...)			::sk::Log::GetClientLogger()->trace(x, __VA_ARGS__)
	#define SK_INFO(x, ...)				::sk::Log::GetClientLogger()->info(x, __VA_ARGS__)
	#define SK_WARN(x, ...)				::sk::Log::GetClientLogger()->warn(x, __VA_ARGS__)
	#define SK_ERROR(x, ...)			::sk::Log::GetClientLogger()->error(x, __VA_ARGS__)
	#define SK_CRITICAL(x, ...)			::sk::Log::GetClientLogger()->critical(x, __VA_ARGS__)
#elif defined(SK_SERVER)
	#define SK_TRACE(x, ...)			::sk::Log::GetServerLogger()->trace(x, __VA_ARGS__)
	#define SK_INFO(x, ...)				::sk::Log::GetServerLogger()->info(x, __VA_ARGS__)
	#define SK_WARN(x, ...)				::sk::Log::GetServerLogger()->warn(x, __VA_ARGS__)
	#define SK_ERROR(x, ...)			::sk::Log::GetServerLogger()->error(x, __VA_ARGS__)
	#define SK_CRITICAL(x, ...)			::sk::Log::GetServerLogger()->critical(x, __VA_ARGS__)

#endif