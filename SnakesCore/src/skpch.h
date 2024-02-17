#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <future>
#include <chrono>
#include <ctime>
#include <cmath>
#include <typeinfo>
#include <sstream>

#include <list>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <bitset>

#include "Snakes/Core/Log.h"
#include "Snakes/Core/Core.h"

#include "Snakes/Utils/Vec3.h"

#include "Snakes/Debug/Instrumentor.h"

#ifdef SK_PLATFORM_WINDOWS
	#include <Windows.h>
#endif