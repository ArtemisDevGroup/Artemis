#ifndef __ARTEMIS_WINDOW_MANAGER_H__
#define __ARTEMIS_WINDOW_MANAGER_H__

#include "pch.h"
#include "Definitions.h"

namespace Artemis {
	class ARTEMIS_API IWindow {
		bool bVisible;
		char szWindowName[MAX_NAME];

	public:
		IWindow(_In_z_ const char* lpWindowName, bool bVisible);

		const char* GetWindowName() const;

		bool GetWindowVisibility() const;
		bool* GetWindowVisibilityPtr();

		void SetWindowVisibility(bool bVisible);

		virtual void Window() = 0;

		void Present();
	};

	using WindowIndex = int;

	template<class T>
	concept WindowType = std::is_base_of<IWindow, T>::value;

	class ARTEMIS_API WindowManager {
		IWindow* WindowCollection[MAX_INVOKE];

	public:
		static bool GetGlobalWindowVisibility();
		static void SetGlobalWindowVisibility(bool bVisibility);

		WindowManager();
		~WindowManager();

		WindowIndex Add(_In_ IWindow* pWindow);

		void Release(_In_range_(INVALID_INDEX, MAX_INVOKE) WindowIndex nIndex = INVALID_INDEX);

		AURORA_NDWR_GET("Get") _Ret_maybenull_ IWindow* Get(_In_range_(0, MAX_INVOKE) WindowIndex nIndex);

		template<WindowType Window>
		AURORA_NDWR_GET("Get") _Ret_maybenull_ Window* Get(_In_range_(0, MAX_INVOKE) WindowIndex nIndex) { return (Window*)Get(nIndex); }

		void PresentAll();
	};
}

#endif // !__ARTEMIS_WINDOW_MANAGER_H__