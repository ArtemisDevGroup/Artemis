// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// -- Win32 and DirectX --

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <Shlwapi.h>
#include <d3d11.h>
#include <dxgi.h>

// -- C++ STL --

// Streams
#include <iostream>
#include <sstream>
#include <fstream>

// Containers
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <optional>

// Types
#include <type_traits>
#include <concepts>
#include <typeinfo>

// Utility
#include <memory>
#include <exception>
#include <utility>
#include <ranges>
#include <regex>
#include <chrono>
#include <format>

// -- C STL --

#include <string.h>

// -- EXT --

#include "ImGui/imgui.h"

#endif // !PCH_H
