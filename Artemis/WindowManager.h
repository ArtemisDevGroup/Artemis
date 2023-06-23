#ifndef __ARTEMIS_WINDOW_MANAGER_H__
#define __ARTEMIS_WINDOW_MANAGER_H__

#include "pch.h"
#include "Definitions.h"
#include "Manager.h"

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

#ifdef _ARTEMIS_EXPORT
	extern template class Manager<IWindow, WindowIndex>;
#else
	template class ARTEMIS_IMPORT Manager<IWindow, WindowIndex>;
#endif // _ARTEMIS_EXPORT

	class ARTEMIS_API WindowManager : public Manager<IWindow, WindowIndex> {
	public:
		static bool GetGlobalWindowVisibility();
		static void SetGlobalWindowVisibility(bool bVisibility);

		void PresentAll();
	};
}

#endif // !__ARTEMIS_WINDOW_MANAGER_H__