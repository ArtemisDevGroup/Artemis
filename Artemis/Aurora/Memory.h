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

#ifndef __AURORA_MEMORY_H__
#define __AURORA_MEMORY_H__

#include "Definitions.h"
#include "ProcessInfo.h"
#include "MemoryTypes.h"

namespace Aurora {
	/// <summary>
	/// Reads memory from an address into a buffer. This function is unsafe and shall not be used unless speed is mandetory.
	/// </summary>
	/// <param name="uAddress">- The address to read memory from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer that receives the read data.</param>
	/// <param name="dwSize">- The number of bytes to read.</param>
	/// <exception cref="ParameterInvalidException"/>
	AURORA_API A_VOID UnsafeRead(
		_In_a_ A_ADDR uAddress,
		_Out_writes_bytes_(dwSize) A_LPVOID lpBuffer,
		_In_ A_DWORD dwSize
	);

	/// <summary>
	/// Reads memory from an address and returns it. This function is unsafe and shall not be used unless speed is mandetory.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read memory from.</param>
	/// <returns>The read data.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType>
	AURORA_NDWR_PURE("UnsafeRead") inline ReturnType UnsafeRead(_In_a_ A_ADDR uAddress) {
		AuroraContextStart();

		ReturnType ret = ReturnType();
		UnsafeRead(uAddress, (A_LPVOID)&ret, sizeof(ReturnType));

		AuroraContextEnd();
		return ret;
	}

	/// <summary>
	/// Reads memory from an address and returns it. This function is unsafe and shall not be used unless speed is mandetory.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read memory from.</param>
	/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, A_I32 nSize>
	inline A_VOID UnsafeRead(
		_In_a_ A_ADDR uAddress,
		_Out_writes_(nSize) ReturnType(&lpBuffer)[nSize]
	) {
		AuroraContextStart();
		UnsafeRead(uAddress, (A_LPVOID)lpBuffer, nSize * sizeof(ReturnType));
		AuroraContextEnd();
	}

	/// <summary>
	/// Reads memory from an address into a buffer.
	/// </summary>
	/// <param name="uAddress">- The address to read memory from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer that receives the read data.</param>
	/// <param name="dwSize">- The number of bytes to read.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	AURORA_API A_VOID Read(
		_In_a_ A_ADDR uAddress,
		_Out_writes_bytes_(dwSize) A_LPVOID lpBuffer,
		_In_ A_DWORD dwSize
	);

	/// <summary>
	/// Reads memory from an address and returns it.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read memory from.</param>
	/// <returns>The read data.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType>
	AURORA_NDWR_PURE("Read") inline ReturnType Read(_In_a_ A_ADDR uAddress) {
		AuroraContextStart();
		
		ReturnType ret = ReturnType();
		Read(uAddress, (A_LPVOID)&ret, sizeof(ReturnType));

		AuroraContextEnd();
		return ret;
	}

	/// <summary>
	/// Reads memory from an address.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read memory from.</param>
	/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, A_I32 nSize>
	inline A_VOID Read(
		_In_a_ A_ADDR uAddress,
		_Out_writes_(nSize) ReturnType(&lpBuffer)[nSize]
	) {
		AuroraContextStart();
		Read(uAddress, (A_LPVOID)lpBuffer, nSize * sizeof(ReturnType));
		AuroraContextEnd();
	}

	/// <summary>
	/// Reads the address at the end of a pointer chain and returns it.
	/// </summary>
	/// <typeparam name="PointerType">- The type of data at the end of the pointer. In this instance, it does not have an effect on the return value.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <returns>The address at the end of the pointer chain.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<typename PointerType>
	AURORA_NDWR_PURE("ReadPtrAddress") inline A_ADDR ReadPtrAddress(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<PointerType>& refPointer
	) {
		AuroraContextStart();

		for (A_ADDR uOffset : refPointer)
			uBaseAddress = Read<A_ADDR>(uBaseAddress) + uOffset;

		AuroraContextEnd();
		return uBaseAddress;
	}

	/// <summary>
	/// Reads the address at the end of a pointer chain and returns it.
	/// </summary>
	/// <typeparam name="PointerType">- The type of data at the end of the pointer. In this instance, it does not have an effect on the return value.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <returns>The address at the end of the pointer chain.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<typename PointerType>
	AURORA_NDWR_PURE("ReadPtrAddress") inline A_ADDR ReadPtrAddress(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<PointerType>& refPointer
	) {
		AuroraContextStart();

		A_ADDR uAddress = refModuleInfo.GetModuleBaseAddress() + refPointer.GetOffset();

		for (A_ADDR uOffset : refPointer)
			uAddress = Read<A_ADDR>(uAddress) + uOffset;

		AuroraContextEnd();
		return uAddress;
	}

	// ReadPtr definitions dealing with standard pointers and requiring explicit specification of return type.

	/// <summary>
	/// Reads memory from the end of a pointer chain and returns it.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <returns>The read data.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, typename PointerType>
	AURORA_NDWR_PURE("ReadPtr") inline ReturnType ReadPtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<PointerType>& refPointer
	) {
		AuroraContextStart();
		ReturnType retValue = Read<ReturnType>(ReadPtrAddress<PointerType>(uBaseAddress, refPointer));
		AuroraContextEnd();
		return retValue;
	}

	/// <summary>
	/// Reads memory from the end of a pointer.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <typeparam name="nSize">- The number of elements in the array.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, typename PointerType, A_I32 nSize>
	inline A_VOID ReadPtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<PointerType>& refPointer,
		_Out_writes_(nSize) ReturnType(&lpBuffer)[nSize]
	) {
		AuroraContextStart();
		Read<ReturnType, nSize>(ReadPtrAddress<PointerType>(uBaseAddress, refPointer), lpBuffer);
		AuroraContextEnd();
	}

	/// <summary>
	/// Reads memory from the end of a pointer.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
	/// <param name="nSize">- The size of the buffer in elements.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, typename PointerType>
	inline A_VOID ReadPtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<PointerType>& refPointer,
		_Out_writes_(nSize) ReturnType* lpBuffer,
		_In_ A_I32 nSize
	) {
		AuroraContextStart();
		Read(ReadPtrAddress<PointerType>(uBaseAddress, refPointer), lpBuffer, nSize * sizeof(ReturnType));
		AuroraContextEnd();
	}

	// ReadPtr definitions dealing with base pointers and requiring explicit specification of return type.

	/// <summary>
	/// Reads memory from the end of a pointer chain and returns it.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <returns>The read data.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, typename PointerType>
	AURORA_NDWR_PURE("ReadPtr") inline ReturnType ReadPtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<PointerType>& refPointer
	) {
		AuroraContextStart();
		ReturnType retValue = Read<ReturnType>(ReadPtrAddress<PointerType>(refModuleInfo, refPointer));
		AuroraContextEnd();
		return retValue;
	}

	/// <summary>
	/// Reads memory from the end of a pointer chain.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <typeparam name="nSize">- The number of elements in the array.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, typename PointerType, A_I32 nSize>
	inline A_VOID ReadPtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<PointerType>& refPointer,
		_Out_writes_(nSize) ReturnType(&lpBuffer)[nSize]
	) {
		AuroraContextStart();
		Read<ReturnType, nSize>(ReadPtrAddress<PointerType>(refModuleInfo, refPointer), lpBuffer);
		AuroraContextEnd();
	}

	/// <summary>
	/// Reads memory from the end of a pointer chain.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
	/// <param name="nSize">- The size of the buffer in elements.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, typename PointerType>
	inline A_VOID ReadPtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<PointerType>& refPointer,
		_Out_writes_(nSize) ReturnType* lpBuffer,
		_In_ A_I32 nSize
	) {
		AuroraContextStart();
		Read(ReadPtrAddress<PointerType>(refModuleInfo, refPointer), lpBuffer, nSize * sizeof(ReturnType));
		AuroraContextEnd();
	}

	// ReadPtr definitions dealing with standard pointers and inheriting the return type of the pointer.

	/// <summary>
	/// Reads data from the end of a pointer and returns it.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <returns>The read data.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType>
	AURORA_NDWR_PURE("ReadPtr") inline ReturnType ReadPtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<ReturnType>& refPointer
	) {
		AuroraContextStart();
		ReturnType retValue = Read<ReturnType>(ReadPtrAddress<ReturnType>(uBaseAddress, refPointer));
		AuroraContextEnd();
		return retValue;
	}

	/// <summary>
	/// Reads memory from the end of a pointer.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <typeparam name="nSize">- The number of elements in the array.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, A_I32 nSize>
	inline A_VOID ReadPtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<ReturnType>& refPointer,
		_Out_writes_(nSize) ReturnType(&lpBuffer)[nSize]
	) {
		AuroraContextStart();
		Read<ReturnType, nSize>(ReadPtrAddress<ReturnType>(uBaseAddress, refPointer), lpBuffer);
		AuroraContextEnd();
	}

	/// <summary>
	/// Reads memory from the end of a pointer.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
	/// <param name="nSize">- The size of the buffer in elements.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType>
	inline A_VOID ReadPtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<ReturnType>& refPointer,
		_Out_writes_(nSize) ReturnType* lpBuffer,
		_In_ A_I32 nSize
	) {
		AuroraContextStart();
		Read(ReadPtrAddress<ReturnType>(uBaseAddress, refPointer), lpBuffer, nSize * sizeof(ReturnType));
		AuroraContextEnd();
	}

	// ReadPtr definitions dealing with base pointers and inheriting the return type of the pointer.

	/// <summary>
	/// Reads memory from the end of a pointer chain and returns it.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <returns>The read data.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType>
	AURORA_NDWR_PURE("ReadPtr") inline ReturnType ReadPtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<ReturnType>& refPointer
	) {
		AuroraContextStart();
		ReturnType retValue = Read<ReturnType>(ReadPtrAddress<ReturnType>(refModuleInfo, refPointer));
		AuroraContextEnd();
		return retValue;
	}

	/// <summary>
	/// Reads memory from the end of a pointer chain.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <typeparam name="nSize">- The number of elements in the array.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType, A_I32 nSize>
	inline A_VOID ReadPtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<ReturnType>& refPointer,
		_Out_writes_(nSize) ReturnType(&lpBuffer)[nSize]
	) {
		AuroraContextStart();
		Read<ReturnType, nSize>(ReadPtrAddress<ReturnType>(refModuleInfo, refPointer), lpBuffer);
		AuroraContextEnd();
	}

	/// <summary>
	/// Reads memory from the end of a pointer chain.
	/// </summary>
	/// <typeparam name="ReturnType">- The type to read.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
	/// <param name="nSize">- The size of the buffer in elements.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<ReadReturnType ReturnType>
	inline A_VOID ReadPtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<ReturnType>& refPointer,
		_Out_writes_(nSize) ReturnType* lpBuffer,
		_In_ A_I32 nSize
	) {
		AuroraContextStart();
		Read(ReadPtrAddress<ReturnType>(refModuleInfo, refPointer), lpBuffer, nSize * sizeof(ReturnType));
		AuroraContextEnd();
	}

	/// <summary>
	/// Reads an ANSI string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
	/// <param name="dwCount">- The number of characters to read.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	AURORA_API A_VOID ReadStringA(
		_In_a_ A_ADDR uAddress,
		_Out_writes_z_(dwCount) A_LPSTR lpBuffer,
		_In_ A_DWORD dwCount
	);

	/// <summary>
	/// Reads an ANSI string from an address.
	/// </summary>
	/// <typeparam name="nCount">- The size fo the array.</typeparam>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A reference to a buffer to receive the read string.</param>
	/// <param name="lpNumberOfCharactersRead">- A pointer to a DWORD to receive the number of characters read. This parameter is optional.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<A_I32 nCount>
	inline A_VOID ReadStringA(
		_In_a_ A_ADDR uAddress,
		_Out_writes_z_(nCount) A_CHAR(&lpBuffer)[nCount]
	) {
		AuroraContextStart();
		ReadStringA(uAddress, lpBuffer, nCount);
		AuroraContextEnd();
	}

	/// <summary>
	/// Reads a UTF-16LE string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
	/// <param name="dwCount">- The number of characters to read.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	AURORA_API A_VOID ReadStringW(
		_In_a_ A_ADDR uAddress,
		_Out_writes_z_(dwCount) A_LPWSTR lpBuffer,
		_In_ A_DWORD dwCount
	);

	/// <summary>
	/// Reads a UTF-16LE string from an address.
	/// </summary>
	/// <typeparam name="nCount">- The number of characters in the array.</typeparam>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A reference to a buffer to receive the read string.</param>
	/// <param name="lpNumberOfCharactersRead">- A pointer to a DWORD to receive the number of characters read. This parameter is optional.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadException"/>
	template<A_I32 nCount>
	inline A_VOID ReadStringW(
		_In_a_ A_ADDR uAddress,
		_Out_writes_z_(nCount) A_WCHAR(&lpBuffer)[nCount]
	) {
		AuroraContextStart();
		ReadStringW(uAddress, lpBuffer, nCount);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to an address from a buffer. This function is unsafe and shall not be used unless speed is mandetory.
	/// </summary>
	/// <param name="uAddress">- The address to write memory to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer that contains the data to write.</param>
	/// <param name="dwSize">- The number of bytes to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	AURORA_API A_VOID UnsafeWrite(
		_In_a_ A_ADDR uAddress,
		_In_reads_bytes_(dwSize) A_LPCVOID lpBuffer,
		_In_ A_DWORD dwSize
	);

	/// <summary>
	/// Writes memory to an address from a buffer. This function is unsafe and shall not be used unless speed is mandetory.
	/// </summary>
	/// <param name="uAddress">- The address to write memory to.</param>
	/// <param name="refData">- The data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	template<WriteDataType DataType>
	inline A_VOID UnsafeWrite(
		_In_a_ A_ADDR uAddress,
		_In_ const DataType& refData
	) {
		AuroraContextStart();
		UnsafeWrite(uAddress, (A_LPCVOID)&refData, sizeof(DataType));
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to an address from a buffer. This function is unsafe and shall not be used unless speed is mandetory.
	/// </summary>
	/// <param name="uAddress">- The address to write memory to.</param>
	/// <param name="lpData">- A reference to an array containing the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, A_I32 nSize>
	inline A_VOID UnsafeWrite(
		_In_a_ A_ADDR uAddress,
		_In_ const DataType(&lpData)[nSize]
	) {
		AuroraContextStart();
		Write(uAddress, (A_LPCVOID)lpData, nSize * sizeof(DataType));
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to an address from a buffer.
	/// </summary>
	/// <param name="uAddress">- The address to write memory to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer that contains the data to write.</param>
	/// <param name="dwSize">- The number of bytes to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	AURORA_API A_VOID Write(
		_In_a_ A_ADDR uAddress,
		_In_reads_bytes_(dwSize) A_LPCVOID lpBuffer,
		_In_ A_DWORD dwSize
	);

	/// <summary>
	/// Writes memory to an address from a buffer.
	/// </summary>
	/// <param name="uAddress">- The address to write memory to.</param>
	/// <param name="refData">- The data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType>
	inline A_VOID Write(
		_In_a_ A_ADDR uAddress,
		_In_ const DataType& refData
	) {
		AuroraContextStart();
		Write(uAddress, (A_LPCVOID)&refData, sizeof(DataType));
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to an address from a buffer.
	/// </summary>
	/// <param name="uAddress">- The address to write memory to.</param>
	/// <param name="lpData">- A reference to an array containing the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, A_I32 nSize>
	inline A_VOID Write(
		_In_a_ A_ADDR uAddress,
		_In_ const DataType(&lpData)[nSize]
	) {
		AuroraContextStart();
		Write(uAddress, (A_LPCVOID)lpData, nSize * sizeof(DataType));
		AuroraContextEnd();
	}

	// WritePtr definitions dealing with standard pointers and requiring explicit specification of data type.

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="refData">- A reference to the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, typename PointerType>
	inline A_VOID WritePtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<PointerType>& refPointer,
		_In_ const DataType& refData
	) {
		AuroraContextStart();
		Write<DataType>(ReadPtrAddress<PointerType>(uBaseAddress, refPointer), refData);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <typeparam name="nSize">- The number of elements in the array.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="lpData">- A reference to a buffer containing the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, typename PointerType, A_I32 nSize>
	inline A_VOID WritePtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<PointerType>& refPointer,
		_In_ const DataType(&lpData)[nSize]
	) {
		AuroraContextStart();
		Write<DataType, nSize>(ReadPtrAddress<PointerType>(uBaseAddress, refPointer), lpData);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="lpData">- A pointer to a buffer containing the data to write.</param>
	/// <param name="nSize">- The size of the buffer in elements.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, typename PointerType>
	inline A_VOID WritePtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<PointerType>& refPointer,
		_In_ const DataType* lpData,
		_In_ A_I32 nSize
	) {
		AuroraContextStart();
		Write(ReadPtrAddress<PointerType>(uBaseAddress, refPointer), lpData, nSize * sizeof(DataType));
		AuroraContextEnd();
	}

	// WritePtr definitions dealing with base pointers and requiring explicit specification of data type.

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="refData">- A reference to the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, typename PointerType>
	inline A_VOID WritePtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<PointerType>& refPointer,
		_In_ const DataType& refData
	) {
		AuroraContextStart();
		Write<DataType>(ReadPtrAddress<PointerType>(refModuleInfo, refPointer), refData);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="lpData">- A reference to a buffer containing the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, typename PointerType, A_I32 nSize>
	inline A_VOID WritePtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<PointerType>& refPointer,
		_In_ const DataType(&lpData)[nSize]
	) {
		AuroraContextStart();
		Write<DataType, nSize>(ReadPtrAddress<PointerType>(refModuleInfo, refPointer), lpData);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <typeparam name="PointerType">- The type specified by the pointer.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="lpData">- A pointer to a buffer containing the data to write.</param>
	/// <param name="nSize">- The size of the buffer in elements.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, typename PointerType>
	inline A_VOID WritePtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<PointerType>& refPointer,
		_In_ const DataType* lpData,
		_In_ A_I32 nSize
	) {
		AuroraContextStart();
		Write(ReadPtrAddress<PointerType>(refModuleInfo, refPointer), lpData, nSize * sizeof(DataType));
		AuroraContextEnd();
	}

	// WritePtr definitions dealing with standard pointers and inheriting the data type of the pointer.

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="refData">- A reference to the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType>
	inline A_VOID WritePtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<DataType>& refPointer,
		_In_ const DataType& refData
	) {
		AuroraContextStart();
		Write<DataType>(ReadPtrAddress<DataType>(uBaseAddress, refPointer), refData);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <typeparam name="nSize">- The number of elements in the array.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="lpData">- A reference to a buffer containing the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, A_I32 nSize>
	inline A_VOID WritePtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<DataType>& refPointer,
		_In_ const DataType(&lpData)[nSize]
	) {
		AuroraContextStart();
		Write<DataType, nSize>(ReadPtrAddress<DataType>(uBaseAddress, refPointer), lpData);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <param name="uBaseAddress">- The base address of the pointer.</param>
	/// <param name="refPointer">- A reference to the pointer.</param>
	/// <param name="lpData">- A pointer to a buffer containing the data to write.</param>
	/// <param name="nSize">- The size of the buffer in elements.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType>
	inline A_VOID WritePtr(
		_In_a_ A_ADDR uBaseAddress,
		_In_ const Pointer<DataType>& refPointer,
		_In_ const DataType* lpData,
		_In_ A_I32 nSize
	) {
		AuroraContextStart();
		Write(ReadPtrAddress<DataType>(uBaseAddress, refPointer), lpData, nSize * sizeof(DataType));
		AuroraContextEnd();
	}

	// WritePtr definitions dealing with base pointers and inheriting the data type of the pointer.

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="refData">- A reference to the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType>
	inline A_VOID WritePtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<DataType>& refPointer,
		_In_ const DataType& refData
	) {
		AuroraContextStart();
		Write<DataType>(ReadPtrAddress<DataType>(refModuleInfo, refPointer), refData);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="lpData">- A reference to a buffer containing the data to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType, A_I32 nSize>
	inline A_VOID WritePtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<DataType>& refPointer,
		_In_ const DataType(&lpData)[nSize]
	) {
		AuroraContextStart();
		Write<DataType, nSize>(ReadPtrAddress<DataType>(refModuleInfo, refPointer), lpData);
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes memory to the end of a pointer.
	/// </summary>
	/// <typeparam name="DataType">- The type to write.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the base of the pointer.</param>
	/// <param name="refPointer">- A reference to the base pointer.</param>
	/// <param name="lpData">- A pointer to a buffer containing the data to write.</param>
	/// <param name="nSize">- The size of the buffer in elements.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<WriteDataType DataType>
	inline A_VOID WritePtr(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BasePointer<DataType>& refPointer,
		_In_ const DataType* lpData,
		_In_ A_I32 nSize
	) {
		AuroraContextStart();
		Write(ReadPtrAddress<DataType>(refModuleInfo, refPointer), lpData, nSize * sizeof(DataType));
		AuroraContextEnd();
	}

	/// <summary>
	/// Writes an ANSI string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="lpBuffer">- A pointer to the string to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	AURORA_API A_VOID WriteStringA(
		_In_a_ A_ADDR uAddress,
		_In_z_ A_LPCSTR lpBuffer
	);

	/// <summary>
	/// Writes a UTF-16LE string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="lpBuffer">- A pointer to the string to write.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	AURORA_API A_VOID WriteStringW(
		_In_a_ A_ADDR uAddress,
		_In_z_ A_LPCWSTR lpBuffer
	);

	/// <summary>
	/// Enables an instruction patch.
	/// </summary>
	/// <typeparam name="nSize">- The number of bytes to patch.</typeparam>
	/// <param name="uAddress">- The address to patch.</param>
	/// <param name="refPatch">- A reference to the patch.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<A_I32 nSize>
	inline A_VOID EnablePatch(
		_In_a_ A_ADDR uAddress,
		_In_ const InstructionPatch<nSize>& refPatch
	) {
		AuroraContextStart();
		Write<A_BYTE, nSize>(uAddress, refPatch.GetEnableCode().szBytes);
		AuroraContextEnd();
	}

	/// <summary>
	/// Enables an instruction patch.
	/// </summary>
	/// <typeparam name="nSize">- The number of bytes to patch.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the code to patch.</param>
	/// <param name="refPatch">- A reference to the patch.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<A_I32 nSize>
	inline A_VOID EnablePatch(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BaseInstructionPatch<nSize>& refPatch
	) {
		AuroraContextStart();
		Write<A_BYTE, nSize>(refModuleInfo->GetModuleBaseAddress() + refPatch.GetOffset(), refPatch.GetEnableCode().szBytes);
		AuroraContextEnd();
	}

	/// <summary>
	/// Disables an instruction patch.
	/// </summary>
	/// <typeparam name="nSize">- The number of bytes to patch.</typeparam>
	/// <param name="uAddress">- The address to patch.</param>
	/// <param name="refPatch">- A reference to the patch.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<A_I32 nSize>
	inline A_VOID DisablePatch(
		_In_a_ A_ADDR uAddress,
		_In_ const InstructionPatch<nSize>& refPatch
	) {
		AuroraContextStart();
		Write<A_BYTE, nSize>(uAddress, refPatch.GetDisableCode().szBytes);
		AuroraContextEnd();
	}

	/// <summary>
	/// Disables an instruction patch.
	/// </summary>
	/// <typeparam name="nSize">- The number of bytes to patch.</typeparam>
	/// <param name="refModuleInfo">- A reference to the module containing the code to patch.</param>
	/// <param name="refPatch">- A reference to the patch.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteException"/>
	template<A_I32 nSize>
	inline A_VOID DisablePatch(
		_In_ const ModuleInfo& refModuleInfo,
		_In_ const BaseInstructionPatch<nSize>& refPatch
	) {
		AuroraContextStart();
		Write<A_BYTE, nSize>(refModuleInfo->GetModuleBaseAddress() + refPatch.GetOffset(), refPatch.GetDisableCode().szBytes);
		AuroraContextEnd();
	}
}

#endif // !__AURORA_MEMORY_H__