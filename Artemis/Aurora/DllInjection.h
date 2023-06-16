//------------------------------------------------------------------------>
// MIT License
// 
// Copyright (c) 2023 Artemis Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------>
// Aurora: https://github.com/ArtemisDevGroup/Aurora
// This file was authored by:
// @Astrea0014: https://github.com/Astrea0014
//------------------------------------------------------------------------>

#ifndef __AURORA_DLL_INJECTION_H__
#define __AURORA_DLL_INJECTION_H__

#include "Definitions.h"
#include "Exceptions.h"
#include "ProcessInfo.h"

#undef UNICODE

#include <Windows.h>

namespace Aurora {
	/// <summary>
	/// Defines what the provided path is relative to, if at all.
	/// </summary>
	enum class PathRelativity {
		ToCaller, // The path is relative to the calling process.
		ToTarget, // The path is relative to the target process.
		Absolute  // The path is absolute and starts with a drive letter.
	};

	/// <summary>
	/// Injects a dll into a process.
	/// </summary>
	/// <param name="lpProcessName">- The name of the target process.</param>
	/// <param name="lpDllName">- The payload dll path.</param>
	/// <param name="nPathRelativity">- The relativity of the payload path.</param>
	/// <exception cref="WindowsApiException"/>
	/// <exception cref="ObjectNotFoundException"/>
	/// <exception cref="ParameterInvalidException"/>
	AURORA_API A_VOID InjectDll(
		_In_z_ A_LPCSTR lpProcessName,
		_In_z_ A_LPCSTR lpDllName,
		_In_ PathRelativity nPathRelativity
	);

	/// <summary>
	/// Injects a dll into a process.
	/// </summary>
	/// <param name="refProcessInfo">- A reference to the target process information.</param>
	/// <param name="lpDllName">- The payload dll path.</param>
	/// <param name="nPathRelativity">- The relativity of the payload path.</param>
	/// <exception cref="WindowsApiException"/>
	/// <exception cref="ParameterInvalidException"/>
	AURORA_API A_VOID InjectDll(
		_In_ const ProcessInfo32& refProcessInfo,
		_In_z_ A_LPCSTR lpDllName,
		_In_ PathRelativity nPathRelativity
	);

	/// <summary>
	/// Injects a dll into a process.
	/// </summary>
	/// <param name="refProcessInfo">- A reference to the target process information.</param>
	/// <param name="lpDllName">- The payload dll path.</param>
	/// <param name="nPathRelativity">- The relativity of the payload path.</param>
	/// <exception cref="WindowsApiException"/>
	/// <exception cref="ParameterInvalidException"/>
	AURORA_API A_VOID InjectDll(
		_In_ const ProcessInfo64& refProcessInfo,
		_In_z_ A_LPCSTR lpDllName,
		_In_ PathRelativity nPathRelativity
	);
}

#endif // __AURORA_DLL_INJECTION_H__