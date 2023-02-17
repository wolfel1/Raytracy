#pragma once

#include <memory>
#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>
#include <future>
#include <type_traits>

#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "raytracy/core/Core.h"
#include "raytracy/core/Log.h"
#include "raytracy/core/Random.h"
#include "raytracy/debug/Instrumentor.h"

#ifdef RTY_PLATFORM_WINDOWS
#include <Windows.h>
#endif