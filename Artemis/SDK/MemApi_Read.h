#ifndef __MEMAPI_READ_H__
#define __MEMAPI_READ_H__

#include <Windows.h>

#include "Def.h"
#include "ErrorApi.h"
#include "MemShApi.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Reads data from a memory address.
	/// </summary>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	BOOL XERead(
		_In_ ADDRESS uAddress,
		_Out_writes_(dwSize) LPVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Reads data from a memory address. Does it in an unhandled manner. Use with caution.
	/// </summary>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="EXCEPTION_ACCESS_VIOLATION (UNHANDLED)"/>
	BOOL XEReadU(
		_In_ ADDRESS uAddress,
		_Out_writes_(dwSize) LPVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Gets the address from the end of a pointer.
	/// </summary>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="lpAddress">- A pointer to an ADDRESS object to receive the retrieved address.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	BOOL XEGetAddress(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets,
		_Out_ LPADDRESS lpAddress
	);

	/// <summary>
	/// Reads data from the end of a pointer.
	/// </summary>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	BOOL XEReadPtr(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets,
		_Out_writes_(dwSize) LPVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Reads an ANSI string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	BOOL XEReadStringA(
		_In_ ADDRESS uAddress,
		_Out_writes_z_(dwSize) LPSTR lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Reads a UTF-16LE string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	BOOL XEReadStringW(
		_In_ ADDRESS uAddress,
		_Out_writes_z_(dwSize) LPWSTR lpBuffer,
		_In_ DWORD dwSize
	);

#ifdef __cplusplus
}

#include "String.h"

namespace api {
	/// <summary>
	/// Reads data from a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <returns>The read value.</returns>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline T Read(_In_ ADDRESS uAddress) {
		T buf = T();
		if (!XERead(uAddress, &buf, sizeof(T))) throw ReadAccessViolationException(uAddress);
		return buf;
	}

	/// <summary>
	/// Reads data from a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void Read(
		_In_ ADDRESS uAddress,
		_Out_ T* lpBuffer
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		if (!XERead(uAddress, lpBuffer, sizeof(T))) throw ReadAccessViolationException(uAddress);
	}

	/// <summary>
	/// Reads data from a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	template<typename T>
	inline void Read(
		_In_ ADDRESS uAddress,
		_Out_writes_(dwSize) T* lpBuffer,
		_In_ DWORD dwSize
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		if (!XERead(uAddress, lpBuffer, dwSize)) throw ReadAccessViolationException(uAddress);
	}

	/// <summary>
	/// Reads data from a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <returns>The read value.</returns>
	/// <exception cref="EXCEPTION_ACCESS_VIOLATION"/>
	template<typename T>
	inline T ReadU(_In_ ADDRESS uAddress) {
		T buf = T();
		XEReadU(uAddress, &buf, sizeof(T));
		return buf;
	}

	/// <summary>
	/// Reads data from a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="EXCEPTION_ACCESS_VIOLATION"/>
	template<typename T>
	inline void ReadU(
		_In_ ADDRESS uAddress,
		_Out_ T* lpBuffer
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");
		XEReadU(uAddress, lpBuffer, sizeof(T));
	}

	/// <summary>
	/// Reads data from a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="EXCEPTION_ACCESS_VIOLATION"/>
	template<typename T>
	inline void ReadU(
		_In_ ADDRESS uAddress,
		_Out_writes_(dwSize) T* lpBuffer,
		_In_ DWORD dwSize
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");
		XEReadU(uAddress, lpBuffer, dwSize);
	}

	/// <summary>
	/// Gets the address from the end of a pointer.
	/// </summary>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <returns>The read address.</returns>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	inline ADDRESS GetAddress(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets
	) {
		if (!lpOffsets) throw ParameterNullException("lpOffsets");

		ADDRESS addr = ADDRESS();
		for (DWORD i = 0; i < lpOffsets->dwCount; i++) {
			if (!XERead(
				uAddress,
				&uAddress,
				sizeof(uAddress)
			)) throw ReadAccessViolationException(uAddress);

			uAddress += lpOffsets->szOffsets[i];
		}
		return addr;
	}

	/// <summary>
	/// Gets the address from the end of a pointer.
	/// </summary>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the retrieved address.</param>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	inline void GetAddress(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets,
		_Out_ LPADDRESS lpReturn
	) {
		if (!lpOffsets) throw ParameterNullException("lpOffsets");
		if (!lpReturn) throw ParameterNullException("lpReturn");

		ADDRESS addr = ADDRESS();
		for (DWORD i = 0; i < lpOffsets->dwCount; i++) {
			if (!XERead(
				uAddress,
				&uAddress,
				sizeof(uAddress)
			)) throw ReadAccessViolationException(uAddress);

			uAddress += lpOffsets->szOffsets[i];
		}

		*lpReturn = addr;
	}

	/// <summary>
	/// Gets the address from the end of a pointer.
	/// </summary>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="Offsets">- The pointer offsets.</param>
	/// <returns>The read address.</returns>
	/// <exception cref="ReadAccessViolationException"/>
	inline ADDRESS GetAddress(
		_In_ ADDRESS uAddress,
		_In_ OFFSETS Offsets
	) {
		OFFSET o;
		o.dwCount = Offsets.Size();
		memcpy(o.szOffsets, Offsets.Ptr(), o.dwCount * sizeof(ADDRESS));

		return GetAddress(uAddress, &o);
	}

	/// <summary>
	/// Gets the address from the end of a pointer.
	/// </summary>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="Offsets">- The pointer offsets.</param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the retrieved address.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	inline void GetAddress(
		_In_ ADDRESS uAddress,
		_In_ OFFSETS Offsets,
		_Out_ LPADDRESS lpReturn
	) {
		if (!lpReturn) throw ParameterNullException("lpReturn");

		OFFSET o;
		o.dwCount = Offsets.Size();
		memcpy(o.szOffsets, Offsets.Ptr(), o.dwCount * sizeof(ADDRESS));
		
		GetAddress(uAddress, &o, lpReturn);
	}

	/// <summary>
	/// Reads data from the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <returns>The read value.</returns>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline T ReadPtr(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets
	) {
		if (!lpOffsets) throw ParameterNullException("lpOffsets");

		uAddress = GetAddress(uAddress, lpOffsets);

		return Read<T>(uAddress);
	}

	/// <summary>
	/// Reads data from the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void ReadPtr(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets,
		_Out_ T* lpBuffer
	) {
		if (!lpOffsets) throw ParameterNullException("lpOffsets");
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		uAddress = GetAddress(uAddress, lpOffsets);

		Read<T>(uAddress, lpBuffer);
	}

	/// <summary>
	/// Reads data from the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void ReadPtr(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets,
		_Out_ T* lpBuffer,
		_In_ DWORD dwSize
	) {
		if (!lpOffsets) throw ParameterNullException("lpOffsets");
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		uAddress = GetAddress(uAddress, lpOffsets);
		Read<T>(uAddress, lpBuffer, dwSize);
	}

	/// <summary>
	/// Reads data from the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="Offsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <returns>The read value.</returns>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline T ReadPtr(
		_In_ ADDRESS uAddress,
		_In_ OFFSETS Offsets
	) {
		uAddress = GetAddress(uAddress, Offsets);
		return Read<T>(uAddress);
	}

	/// <summary>
	/// Reads data from the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="Offsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void ReadPtr(
		_In_ ADDRESS uAddress,
		_In_ OFFSETS Offsets,
		_Out_ T* lpBuffer
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		uAddress = GetAddress(uAddress, Offsets);
		Read<T>(uAddress, lpBuffer);
	}

	/// <summary>
	/// Reads data from the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="Offsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void ReadPtr(
		_In_ ADDRESS uAddress,
		_In_ OFFSETS Offsets,
		_Out_ T* lpBuffer,
		_In_ DWORD dwSize
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		uAddress = GetAddress(uAddress, Offsets);
		Read<T>(uAddress, lpBuffer, dwSize);
	}

	/// <summary>
	/// Reads an ANSI string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	inline void ReadStringA(
		_In_ ADDRESS uAddress,
		_Out_writes_z_(dwSize) LPSTR lpBuffer,
		_In_ DWORD dwSize
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		if (!XEReadStringA(uAddress, lpBuffer, dwSize)) throw ReadAccessViolationException(uAddress);
	}

	/// <summary>
	/// Reads an ANSI string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="dwCount">- The number of characters to read.</param>
	/// <returns>A String object containing the read string.</returns>
	/// <exception cref="MemoryFullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	inline String ReadStringA(
		_In_ ADDRESS uAddress,
		_In_ DWORD dwCount
	) {
		LPSTR lpStr = (LPSTR)calloc(dwCount + 1, sizeof(CHAR));
		if (!lpStr) throw MemoryFullException();

		if (!XEReadStringA(
			uAddress,
			lpStr,
			dwCount
		)) {
			free(lpStr);
			throw ReadAccessViolationException(uAddress);
		}
		lpStr[dwCount] = '\0';

		String ret(lpStr);
		
		free(lpStr);
		return ret;
	}

	/// <summary>
	/// Reads a UTF-16LE string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	inline void ReadStringW(
		_In_ ADDRESS uAddress,
		_Out_writes_z_(dwSize) LPWSTR lpBuffer,
		_In_ DWORD dwSize
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		if (!XEReadStringW(uAddress, lpBuffer, dwSize)) throw ReadAccessViolationException(uAddress);
	}

	/// <summary>
	/// Reads a UTF-16LE string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="dwCount">- The number of characters to read.</param>
	/// <returns>A WString object containing the read string.</returns>
	/// <exception cref="MemoryFullException"/>
	/// <exception cref="ReadAccessViolationException"/>
	inline WString ReadStringW(
		_In_ ADDRESS uAddress,
		_In_ DWORD dwCount
	) {
		LPWSTR lpStr = (LPWSTR)calloc(dwCount + 1, sizeof(WCHAR));
		if (!lpStr) throw MemoryFullException();

		if (!XEReadStringW(
			uAddress,
			lpStr,
			dwCount * 2
		)) {
			free(lpStr);
			throw ReadAccessViolationException(uAddress);
		}
		lpStr[dwCount] = L'\0';

		WString ret(lpStr);

		free(lpStr);
		return ret;
	}

#ifdef __VECTOR_H__

	/// <summary>
	/// Reads a Vec2 from an address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpMask">- The vector mask that indicates in which order the values should be read from memory.</param>
	/// <returns>The read Vec2.</returns>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	Vec2<T> ReadVec2(
		_In_ ADDRESS uAddress,
		_In_ LPCSTR lpMask = "12"
	) {
		if (!lpMask) throw ParameterNullException("lpMask");
		if (!XECheckVecMask(lpMask, 2)) throw ParameterInvalidException("lpMask");

		Vec2<T> ret;
		ADDRESS currentAddress;

		for (INT i = 0; i < 2; i++) {
			currentAddress = uAddress + (lpMask[i] - 0x31) * sizeof(T);
			if (!XERead(currentAddress, &ret.Vec[i], sizeof(T))) throw ReadAccessViolationException(currentAddress);
		}

		return ret;
	}

	/// <summary>
	/// Reads a Vec3 from an address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpMask">- The vector mask that indicates in which order the values should be read from memory.</param>
	/// <returns>The read Vec3.</returns>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	Vec3<T> ReadVec3(
		_In_ ADDRESS uAddress,
		_In_ LPCSTR lpMask = "123"
	) {
		if (!lpMask) throw ParameterNullException("lpMask");
		if (!XECheckVecMask(lpMask, 3)) throw ParameterInvalidException("lpMask");

		Vec3<T> ret;
		ADDRESS currentAddress;

		for (INT i = 0; i < 3; i++) {
			currentAddress = uAddress + (lpMask[i] - 0x31) * sizeof(T);
			if (!XERead(currentAddress, &ret.Vec[i], sizeof(T))) throw ReadAccessViolationException(currentAddress);
		}

		return ret;
	}

	/// <summary>
	/// Reads a Vec4 from an address.
	/// </summary>
	/// <typeparam name="T">- The type to read.</typeparam>
	/// <param name="uAddress">- The address to read data from.</param>
	/// <param name="lpMask">- The vector mask that indicates in which order the values should be read from memory.</param>
	/// <returns>The read Vec4.</returns>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	Vec4<T> ReadVec4(
		_In_ ADDRESS uAddress,
		_In_ LPCSTR lpMask = "1234"
	) {
		if (!lpMask) throw ParameterNullException("lpMask");
		if (!XECheckVecMask(lpMask, 4)) throw ParameterInvalidException("lpMask");

		Vec4<T> ret;
		ADDRESS currentAddress;

		for (INT i = 0; i < 4; i++) {
			currentAddress = uAddress + (lpMask[i] - 0x31) * sizeof(T);
			if (!XERead(currentAddress, &ret.Vec[i], sizeof(T))) throw ReadAccessViolationException(currentAddress);
		}

		return ret;
	}

#endif // __VECTOR_H__

}
#endif // __cplusplus

#endif // !__MEMAPI_READ_H__