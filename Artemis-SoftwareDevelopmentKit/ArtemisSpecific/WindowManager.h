//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__

#include "..\Definitions.h"
#include "..\Interfaces.h"
#include "..\Events.h"
#include "..\Exceptions.h"
#include "..\ImGui\imgui.h"

namespace Artemis {
	/// <summary>
	/// An abstract class for easily creating an ImGui window.
	/// </summary>
	class ARTEMIS_API IWindow : public IRegisterable {
	public:
		/// <summary>
		/// Sets the data fields.
		/// </summary>
		/// <param name="lpWndName">- The name of the window instance.</param>
		/// <param name="dwWndId">- The window id. Has to be an unique id used by no other window. Id 0-19 are reserved.</param>
		inline IWindow(_In_z_ LPCSTR lpWndName, _In_ DWORD dwWndId) : IRegisterable(), lpWndName(lpWndName), dwWndId(dwWndId), bShow(TRUE) {}

		/// <summary>
		/// A bool that dictates wether the window should be displayed or hidden.
		/// <para>Setting it to TRUE will show the window while setting it to FALSE will hide it.</para>
		/// </summary>
		BOOL bShow;
		LPCSTR lpWndName;		// The name of the window instance.
		const DWORD dwWndId;	// The unique window identifier.

		/// <summary>
		/// A purely virtual function that handles the window widgets.
		/// </summary>
		virtual void Window() = 0;

		/// <summary>
		/// Presents the ImGui window using the widgets provided in the Window function.
		/// </summary>
		void Present();
	};

	/// <summary>
	/// A class for managing the ImGui window instancws presented by project Artemis.
	/// </summary>
	class ARTEMIS_API WindowManager : public IDisposable {
		IWindow* lpszWndArray[MAX_INVOKE];

	public:
		WindowManager();

		/// <summary>
		/// Registers a window class instance for a window to be displayed.
		/// <para>The pointer MUST be to memory created with the "new" keyword.</para>
		/// </summary>
		/// <param name="lpWndInst">- A pointer to the derived class.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="AttributeException"/>
		void RegisterWnd(_In_ IWindow* lpWndInst);

		/// <summary>
		/// Unregisters a previously registered window class instance.
		/// <para>The pointer MUST be to memory created with the "new" keyword.</para>
		/// </summary>
		/// <param name="lpWndInst">- A pointer to the derived class.</param>
		/// <exception cref="ObjectNotFoundException"/>
		void UnregisterWnd(_In_ IWindow* lpWndInst);

		/// <summary>
		/// Deletes all memory pointed to by the registered pointers.
		/// </summary>
		void Release();

		/// <summary>
		/// Invokes all registered windows.
		/// </summary>
		void InvokeWnd();

		/// <summary>
		/// Gets the pointer to a window instance by its unique identifier.
		/// </summary>
		/// <param name="dwWndId">- The identifier of the window to get.</param>
		/// <exception cref="ObjectNotFoundException"/>
		_Check_return_ _When_(IN_RANGE(dwWndId, 0, 19), _Ret_notnull_) _When_(!IN_RANGE(dwWndId, 0, 19), _Ret_maybenull_) IWindow* GetWndById(_In_ DWORD dwWndId);

		/// <summary>
		/// Sets the visibility of the ImGui windows.
		/// <para>NOTE: This function ONLY affects a head bool that shows or hides ALL windows. It does not affect the bShow bool in any window instance.</para>
		/// </summary>
		/// <param name="bShow">- Set to TRUE to show ImGui. Set to FALSE to hide.</param>
		static void ShowWnd(_In_ BOOL bShow);

		/// <summary>
		/// Gets a pointer to the visibility bool that determines wether to show or hide all windows.
		/// </summary>
		/// <returns>A pointer to the visibility bool.</returns>
		static _Check_return_ LPBOOL GetWndVisibilityPtr();
	};
}

#endif // !__WINDOW_MANAGER_H__