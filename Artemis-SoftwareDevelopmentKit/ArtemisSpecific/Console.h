//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdio.h>
#include <Windows.h>

#include "..\Definitions.h"
#include "..\Interfaces.h"

namespace Artemis {
	/// <summary>
	/// A class for managing a console instance in a non console application.
	/// </summary>
	class ARTEMIS_API Console : public IDisposable {
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