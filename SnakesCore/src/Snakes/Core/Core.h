#pragma once

#ifdef SK_PLATFORM_WINDOWS
	#ifdef SK_DYNAMIC_LINK
		#ifdef SK_BUILD_DLL
			#define SNAKES_API __declspec(dllexport)
		#else
			#define SNAKES_API __declspec(dllimport)
		#endif
	#else
		#define SNAKES_API
	#endif
#else
	#ifdef SK_PLATFORM_MACOS
		#define SNAKES_API
	#else
		#error Snakes only supports Windows!
	#endif
#endif

#ifdef SK_DEBUG
	#define SK_ENABLE_ASSERTS
#endif

#ifdef SK_ENABLE_ASSERTS
	#include "Log.h"
	#define SK_ASSERT(flag, msg, ...) {\
		if ((flag)) {\
			SK_CRITICAL("Assertion Failed!");\
			SK_CRITICAL(msg, __VA_ARGS__);\
			__debugbreak();\
		}\
	}
#else
	#define SK_CORE_ASSERT_S(x)
	#define SK_CORE_ASSERT(x, ...)
	#define SK_ASSERT_S(x)
	#define SK_ASSERT(x, ...)
#endif


#define SK_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define GET_COUNT(x) *(&x+1) - x

#define sk_found(container, key) (container.find(key) != container.end())

#include <memory>
namespace sk {
	
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> NewScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ... Args>
	constexpr Ref<T> NewRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T> 
	using matrix = std::vector<std::vector<T>>;

	template <typename T>
	matrix<T> CreateMatrix(int rows, int cols, const T& value = T())
	{
		return matrix<T>(rows, std::vector<T>(cols, value));
	}

	template <typename T>
	T _max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template <typename T>
	T _min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}
}

