#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdio.h>
#include <Windows.h>

#include "..\Definitions.h"

namespace Artemis {
	/// <summary>
	/// A class for managing a console instance in a non console application.
	/// </summary>
	class Console {
		BOOL bAllocated;
		BOOL bShowing;

		FILE* pOut;
		FILE* pIn;

		HWND hConsoleWnd;
		HANDLE hConsoleOut;
	public:
		Console();

		/// <summary>
		/// Allocates the console.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="Exception"/>
		void Allocate();

		/// <summary>
		/// Releases the console instance.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Release();

		/// <summary>
		/// Shows the console window.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Show() const;

		/// <summary>
		/// Hides the console window.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Hide() const;

		/// <summary>
		/// Sets the text color of the console window.
		/// </summary>
		/// <param name="wColor">- The windows color constants.</param>
		/// <exception cref="WindowsApiException"/>
		void SetTextColor(_In_ WORD wColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) const;

		~Console();
	};
}

#endif // !__CONSOLE_H__