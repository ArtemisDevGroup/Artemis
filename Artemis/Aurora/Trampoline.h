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


#ifndef __AURORA_TRAMPOLINE_H__
#define __AURORA_TRAMPOLINE_H__

#include "Definitions.h"
#include "Array.h"
#include "ProcessInfo.h"

namespace Aurora {
	constexpr A_DWORD c_dwAllocationSize = 0x10000;							// The size of every trampoline allocation.
	constexpr A_DWORD c_dwPageSize = 0x80;									// The size of every trampoline page.
	constexpr A_DWORD c_dwPageCount = c_dwAllocationSize / c_dwPageSize;	// The number of trampoline pages available in the allocation.

	/// <summary>
	/// A descriptor for a trampoline instance.
	/// </summary>
	struct TrampolineDescriptor32 {
		A_I32 nPageIndex;
		A_I32 nPageCount;
		A_ADDR32 uTrampolineAddress;
		A_DWORD dwRequestedSize;
		A_DWORD dwTrampolineSize;
	};

	/// <summary>
	/// A descriptor for a trampoline instance.
	/// </summary>
	struct TrampolineDescriptor64 {
		A_I32 nPageIndex;
		A_I32 nPageCount;
		A_ADDR64 uTrampolineAddress;
		A_DWORD dwRequestedSize;
		A_DWORD dwTrampolineSize;
	};

	/// <summary>
	/// <para>Manages trampoline instances for a module.</para>
	/// <para>This class shares one allocated instance per module which means that you can create an infinite amount of instances without side effects</para>
	/// </summary>
	class AURORA_API TrampolineManager32 {
		A_DWORD dwProcessId;

	public:
		/// <summary>
		/// Constructs a TrampolineManager32 targetting a local process.
		/// </summary>
		TrampolineManager32() noexcept;

		/// <summary>
		/// Constructs a TrampolineManager32 targetting a remote process.
		/// </summary>
		/// <param name="refProcessInfo">- A reference to the target process.</param>
		TrampolineManager32(_In_ const ProcessInfo32& refProcessInfo) noexcept;

		/// <summary>
		/// Creates a new trampoline.
		/// </summary>
		/// <param name="dwCodeSize">- The trampoline size.</param>
		/// <returns>A pointer to the newly created trampoline descriptor.</returns><
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ResourceException"/>
		AURORA_NDWR_DISP("CreateTrampoline") _Ret_valid_ TrampolineDescriptor32* CreateTrampoline(_In_ A_DWORD dwCodeSize) const;

		/// <summary>
		/// Releases a previously created trampoline.
		/// </summary>
		/// <param name="lpTrampolineDescriptor">- A pointer to the trampoline descriptor to release.</param>
		/// <exception cref="WindowsApiException"/>
		A_VOID ReleaseTrampoline(_In_ const TrampolineDescriptor32* lpTrampolineDescriptor) const;
	};

	/// <summary>
	/// <para>Manages trampoline instances for a module.</para>
	/// <para>This class shares one allocated instance per module which means that you can create an infinite amount of instances without side effects</para>
	/// </summary>
	class AURORA_API TrampolineManager64 {
		A_DWORD dwProcessId;

	public:
		/// <summary>
		/// Constructs a TrampolineManager64 targetting the current process.
		/// </summary>
		TrampolineManager64() noexcept;

		/// <summary>
		/// Constructs a TrampolineManager64 targetting a remote process.
		/// </summary>
		/// <param name="refProcessInfo">- A reference to the target process.</param>
		TrampolineManager64(_In_ const ProcessInfo64& refProcessInfo) noexcept;

		/// <summary>
		/// Creates a new trampoline.
		/// </summary>
		/// <param name="dwCodeSize">- The trampoline size.</param>
		/// <returns>A pointer to the newly created trampoline descriptor.</returns>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ResourceException"/>
		AURORA_NDWR_DISP("CreateTrampoline") _Ret_valid_ TrampolineDescriptor64* CreateTrampoline(_In_ A_DWORD dwCodeSize) const;

		/// <summary>
		/// Releases a previously created trampoline.
		/// </summary>
		/// <param name="lpTrampolineDescriptor">- A pointer to the trampoline descriptor to release.</param>
		/// <exception cref="WindowsApiException"/>
		A_VOID ReleaseTrampoline(_In_ const TrampolineDescriptor64* lpTrampolineDescriptor) const;
	};

#ifdef _WIN64
	/// <summary>
	/// A descriptor for a trampoline instance.
	/// </summary>
	using TrampolineDescriptor = TrampolineDescriptor64;

	/// <summary>
	/// <para>Manages trampoline instances for a module.</para>
	/// <para>This class shares one allocated instance per module which means that you can create an infinite amount of instances without side effects</para>
	/// </summary>
	using TrampolineManager = TrampolineManager64;
#else
	/// <summary>
	/// A descriptor for a trampoline instance.
	/// </summary>
	using TrampolineDescriptor = TrampolineDescriptor32;

	/// <summary>
	/// <para>Manages trampoline instances for a module.</para>
	/// <para>This class shares one allocated instance per module which means that you can create an infinite amount of instances without side effects</para>
	/// </summary>
	using TrampolineManager = TrampolineManager32;
#endif
}

#endif // !__AURORA_TRAMPOLINE_H__