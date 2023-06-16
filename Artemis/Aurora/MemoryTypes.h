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

#ifndef __AURORA_MEMORY_TYPES_H__
#define __AURORA_MEMORY_TYPES_H__

#include "Definitions.h"
#include "Array.h"

namespace Aurora {
	/// <summary>
	/// A collection of bytes representing CPU-instructions.
	/// </summary>
	template<A_I32 nSize>
	class InstructionBytes {
	public:
		/// <summary>
		/// An array containing the instruction bytes.
		/// </summary>
		A_BYTE szBytes[nSize];

		/// <summary>
		/// Gets the size of the array and returns it.
		/// </summary>
		/// <returns>The size of the array.</returns>
		AURORA_NDWR_GET("size") constexpr const A_I32 size() const noexcept { return nSize; }

		/// <summary>
		/// Gets the begin const iterator.
		/// </summary>
		/// <returns>The begin const iterator.</returns>
		AURORA_NDWR_GET("begin") constexpr A_LPCBYTE begin() const noexcept { return szBytes; }

		/// <summary>
		/// Gets the end const iterator.
		/// </summary>
		/// <returns>The end const iterator.</returns>
		AURORA_NDWR_GET("end") constexpr A_LPCBYTE end() const noexcept { return &szBytes[nSize]; }
	};

	/// <summary>
	/// <para>A pair of byte collections representing CPU-instructions.</para>
	/// <para>Used as a toggleable patch to change the behavior of different functions.</para>
	/// </summary>
	template<A_I32 nSize>
	class InstructionPatch {
		InstructionBytes<nSize> EnableBytes;
		InstructionBytes<nSize> DisableBytes;

	public:
		/// <summary>
		/// Constructs an instruction patch.
		/// </summary>
		/// <param name="EnableBytes">- The instructions to enable the patch.</param>
		/// <param name="DisableBytes">- The instructions to disable the patch. Usually this parameter is the original bytes of the patch.</param>
		constexpr InstructionPatch(
			_In_ const InstructionBytes<nSize>& EnableBytes,
			_In_ const InstructionBytes<nSize>& DisableBytes
		) : EnableBytes(EnableBytes), DisableBytes(DisableBytes) {}

		/// <summary>
		/// Gets a const reference to the bytes used to enable the patch.
		/// </summary>
		/// <returns>A const reference to the enable code.</returns>
		AURORA_NDWR_GET("GetEnableCode") constexpr const InstructionBytes<nSize>& GetEnableCode() const noexcept { return EnableBytes; }

		/// <summary>
		/// Gets a const reference to the bytes used to disable the patch.
		/// </summary>
		/// <returns>A const reference to the disable code.</returns>
		AURORA_NDWR_GET("GetDisableCode") constexpr const InstructionBytes<nSize>& GetDisableCode() const noexcept { return DisableBytes; }
	};

	/// <summary>
	/// <para>A pair of byte collections representing CPU-instructions.</para>
	/// <para>Additionally contains an offset of the base address of the module where the patch shall be written.</para>
	/// <para>Used as a toggleable patch to change the behavior of different functions.</para>
	/// </summary>
	template<A_I32 nSize>
	class BaseInstructionPatch : public InstructionPatch<nSize> {
		A_DWORD dwOffset;

	public:
		/// <summary>
		/// Constructs a base instruction patch.
		/// </summary>
		/// <param name=dwBaseOffset">- The offset from the base address.</param>
		/// <param name="EnableBytes">- The instructions to enable the patch.</param>
		/// <param name="DisableBytes">- The instructions to disable the patch. Usually this parameter is the original bytes of the patch.</param>
		constexpr BaseInstructionPatch(
			_In_ A_DWORD dwBaseOffset,
			_In_ const InstructionBytes<nSize>& EnableBytes,
			_In_ const InstructionBytes<nSize>& DisableBytes
		) : InstructionPatch<nSize>(EnableBytes, DisableBytes), dwOffset(dwBaseOffset) {}

		/// <summary>
		/// Gets the base offset of the patch point.
		/// </summary>
		/// <returns>The base offset.</returns>
		AURORA_NDWR_GET("GetOffset") constexpr A_DWORD GetOffset() const noexcept { return dwOffset; }
	};

	/// <summary>
	/// A pointer chain of 32-bit pointers.
	/// </summary>
	/// <typeparam name="ReturnType">- The type of data at the end of the pointer.</typeparam>
	template<typename ReturnType = void> class Pointer32 : public ArgumentList<A_ADDR32> { public: using ArgumentList<A_ADDR32>::ArgumentList; };

	/// <summary>
	/// A pointer chain of 64-bit pointers.
	/// </summary>
	/// <typeparam name="ReturnType">- The type of data at the end of the pointer.</typeparam>
	template<typename ReturnType = void> class Pointer64 : public ArgumentList<A_ADDR64> { public: using ArgumentList<A_ADDR64>::ArgumentList; };

	/// <summary>
	/// A pointer chain of 32-bit pointers with a base offset.
	/// </summary>
	/// <typeparam name="ReturnType">- The type of data at the end of the pointer.</typeparam>
	template<typename ReturnType = void>
	class BasePointer32 : public Pointer32<ReturnType> {
		A_DWORD dwBaseOffset;
	public:
		inline BasePointer32() noexcept : Pointer32<ReturnType>(), dwBaseOffset(0) {}

		/// <summary>
		/// Constructs a base pointer.
		/// </summary>
		/// <param name="dwBaseOffset">- The offset from the base address.</param>
		/// <param name="list">- The pointer list.</param>
		inline BasePointer32(_In_ A_DWORD dwBaseOffset, _In_ const std::initializer_list<A_ADDR32>& list) : Pointer32<ReturnType>(list), dwBaseOffset(dwBaseOffset) {}

		/// <summary>
		/// Gets the base offset of the pointer.
		/// </summary>
		/// <returns>The base offset.</returns>
		AURORA_NDWR_GET("GetOffset") constexpr A_DWORD GetOffset() const noexcept { return dwBaseOffset; }
	};

	/// <summary>
	/// A pointer chain of 64-bit pointers with a base offset.
	/// </summary>
	/// <typeparam name="ReturnType">- The type of data at the end of the pointer.</typeparam>
	template<typename ReturnType = void>
	class BasePointer64 : public Pointer64<ReturnType> {
		A_DWORD dwBaseOffset;
	public:
		inline BasePointer64() noexcept : Pointer64<ReturnType>(), dwBaseOffset(0) {}

		/// <summary>
		/// Constructs a base pointer.
		/// </summary>
		/// <param name="dwBaseOffset">- The offset from the base address.</param>
		/// <param name="list">- The pointer list.</param>
		inline BasePointer64(_In_ A_DWORD dwBaseOffset, _In_ const std::initializer_list<A_ADDR64>& list) : Pointer64<ReturnType>(list), dwBaseOffset(dwBaseOffset) {}

		/// <summary>
		/// Gets the base offset of the pointer.
		/// </summary>
		/// <returns>The base offset.</returns>
		AURORA_NDWR_GET("GetOffset") constexpr A_DWORD GetOffset() const noexcept { return dwBaseOffset; }
	};

#ifdef _WIN64
	/// <summary>
	/// A pointer chain of 64-bit pointers.
	/// </summary>
	/// <typeparam name="ReturnType">- The type of data at the end of the pointer.</typeparam>
	template<typename ReturnType = void> using Pointer = Pointer64<ReturnType>;

	/// <summary>
	/// A pointer chain of 64-bit pointers with a base offset.
	/// </summary>
	/// <typeparam name="ReturnType">- The type of data at the end of the pointer.</typeparam>
	template<typename ReturnType = void> using BasePointer = BasePointer64<ReturnType>;
#else
	/// <summary>
	/// A pointer chain of 32-bit pointers.
	/// </summary>
	/// <typeparam name="ReturnType">- The type of data at the end of the pointer.</typeparam>
	template<typename ReturnType = void> using Pointer = Pointer32<ReturnType>;

	/// <summary>
	/// A pointer chain of 32-bit pointers with a base offset.
	/// </summary>
	/// <typeparam name="ReturnType">- The type of data at the end of the pointer.</typeparam>
	template<typename ReturnType = void> using BasePointer = BasePointer32<ReturnType>;
#endif // _WIN64
}

#endif // !__AURORA_MEMORY_TYPES_H__