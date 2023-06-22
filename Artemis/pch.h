// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// Windows Header Files
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#include <d3d11.h>
#include <dxgi.h>

// Aurora Headers
#include <Aurora/Array.h>
#include <Aurora/Binary.h>
#include <Aurora/CodeInjection.h>
#include <Aurora/Color.h>
#include <Aurora/Console.h>
#include <Aurora/ConstexprMath.h>
#include <Aurora/Definitions.h>
#include <Aurora/DllInjection.h>
#include <Aurora/Events.h>
#include <Aurora/Exceptions.h>
#include <Aurora/FileSystem.h>
#include <Aurora/Logger.h>
#include <Aurora/Memory.h>
#include <Aurora/MemoryTypes.h>
#include <Aurora/MemoryWin32.h>
#include <Aurora/Pipe.h>
#include <Aurora/ProcessInfo.h>
#include <Aurora/Property.h>
#include <Aurora/Shapes.h>
#include <Aurora/SharedHandle.h>
#include <Aurora/Signal.h>
#include <Aurora/Thread.h>
#include <Aurora/Time.h>
#include <Aurora/Trampoline.h>
#include <Aurora/Vector.h>

// ImGui header files.
#include <ImGui/imconfig.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imstb_rectpack.h>
#include <ImGui/imstb_textedit.h>
#include <ImGui/imstb_truetype.h>

// MinHook header file.
#include <MinHook/MinHook.h>

#endif //PCH_H
