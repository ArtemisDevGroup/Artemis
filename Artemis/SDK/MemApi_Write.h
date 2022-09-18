#ifndef __MEMAPI_WRITE_H__
#define __MEMAPI_WRITE_H__

#include <Windows.h>

#include "Def.h"
#include "ErrorApi.h"
#include "MemShApi.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Writes data to a memory address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_WRITE_ACCESS_VIOLATION"/>
	BOOL XEWrite(
		_In_ ADDRESS uAddress,
		_In_ LPCVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Writes data to a memory address. Does it in an unhandled manner. Use with caution.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_WRITE_ACCESS_VIOLATION"/>
	BOOL XEWriteU(
		_In_ ADDRESS uAddress,
		_In_ LPCVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Writes data to the end of a pointer.
	/// </summary>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_WRITE_ACCESS_VIOLATION"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	BOOL XEWritePtr(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets,
		_In_ LPCVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Writes an ANSI string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpString">- A pointer to a string containing the data to write.</param>
	/// <param name="bWriteNullTerm">- A bool indicating whether to write the string null-terminator or not.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_WRITE_ACCESS_VIOLATION"/>
	BOOL XEWriteStringA(
		_In_ ADDRESS uAddress,
		_In_z_ LPCSTR lpString,
		_In_ BOOL bWriteNullTerm
	);

	/// <summary>
	/// Writes a UTF-16LE string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpString">- A pointer to a string containing the data to write.</param>
	/// <param name="bWriteNullTerm">- A bool indicating whether to write the string null-terminator or not.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_WRITE_ACCESS_VIOLATION"/>
	BOOL XEWriteStringW(
		_In_ ADDRESS uAddress,
		_In_z_ LPCWSTR lpString,
		_In_ BOOL bWriteNullTerm
	);

	/// <summary>
	/// Writes bytes to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <param name="dwNop">- The number of bytes to set to 0x90 after the written bytes.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_WRITE_ACCESS_VIOLATION"/>
	BOOL XEWriteBytes(
		_In_ ADDRESS uAddress,
		_In_ LPCBYTE lpBuffer,
		_In_ DWORD dwSize,
		_In_ DWORD dwNop
	);

	/// <summary>
	/// Sets a number of bytes at an address to the specified value.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="uValue">- The value to set the bytes to.</param>
	/// <param name="dwCount">- The number of bytes to set.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WRITE_ACCESS_VIOLATION"/>
	BOOL XESetBytes(
		_In_ ADDRESS uAddress,
		_In_ BYTE uValue,
		_In_ DWORD dwCount
	);

#ifdef __cplusplus
}

#include "MemApi_Read.h"
#include "String.h"

namespace api {
	/// <summary>
	/// Writes data to a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="Buffer">- The data to write.</param>
	/// <exception cref="WriteAccessViolationException"/>
	template<typename T>
	inline void Write(
		_In_ ADDRESS uAddress,
		_In_ T Buffer
	) {
		if (!XEWrite(uAddress, &Buffer, sizeof(T))) throw WriteAccessViolationException(uAddress);
	}

	/// <summary>
	/// Writes data to a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpData">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WriteAccessViolationException"/>
	template<typename T>
	inline void Write(
		_In_ ADDRESS uAddress,
		_In_ T* lpData,
		_In_ DWORD dwSize
	) {
		if (!lpData) throw ParameterNullException("lPData");

		if (!XEWrite(
			uAddress,
			lpData,
			dwSize
		)) throw WriteAccessViolationException(uAddress);
	}

	/// <summary>
	/// Writes data to a memory address. Does it in an unhandled manner. Use with caution.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="Buffer">- The data to write.</param>
	template<typename T>
	inline void WriteU(
		_In_ ADDRESS uAddress,
		_In_ T Buffer
	) {
		XEWriteU(uAddress, &Buffer, sizeof(T));
	}

	/// <summary>
	/// Writes data to a memory address.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpData">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <exception cref="ParameterNullException"/>
	template<typename T>
	inline void WriteU(
		_In_ ADDRESS uAddress,
		_In_ T* lpBuffer,
		_In_ DWORD dwSize
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		XEWriteU(uAddress, lpBuffer, dwSize);
	}

	/// <summary>
	/// Writes data to the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="Buffer">- The data to write.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WriteAccessViolationException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void WritePtr(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets,
		_In_ T Buffer
	) {
		if (!lpOffsets) throw ParameterNullException("lpOffsets");

		uAddress = GetAddress(uAddress, lpOffsets);
		Write<T>(uAddress, Buffer);
	}

	/// <summary>
	/// Writes data to the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="lpOffsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WriteAccessViolationException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void WritePtr(
		_In_ ADDRESS uAddress,
		_In_ LPOFFSET lpOffsets,
		_In_ T* lpBuffer,
		_In_ DWORD dwSize
	) {
		if (!lpOffsets) throw ParameterNullException("lpOffsets");
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		uAddress = GetAddress(uAddress, lpOffsets);
		Write<T>(uAddress, lpBuffer, dwSize);
	}

	/// <summary>
	/// Writes data to the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="Offsets">- The pointer offsets.</param>
	/// <param name="Buffer">- The data to write.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WriteAccessViolationException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void WritePtr(
		_In_ ADDRESS uAddress,
		_In_ OFFSETS Offsets,
		_In_ T Buffer
	) {
		OFFSET o;
		o.dwCount = Offsets.Size();
		memcpy(o.szOffsets, Offsets.Ptr(), o.dwCount * sizeof(ADDRESS));

		WritePtr<T>(uAddress, &o, Buffer);
	}

	/// <summary>
	/// Writes data to the end of a pointer.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The starting address of the pointer.</param>
	/// <param name="Offsets">- The pointer offsets.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WriteAccessViolationException"/>
	/// <exception cref="ReadAccessViolationException"/>
	template<typename T>
	inline void WritePtr(
		_In_ ADDRESS uAddress,
		_In_ OFFSETS Offsets,
		_In_ T* lpBuffer,
		_In_ DWORD dwSize
	) {
		OFFSET o;
		o.dwCount = Offsets.Size();
		memcpy(o.szOffsets, Offsets.Ptr(), o.dwCount * sizeof(ADDRESS));

		WritePtr<T>(uAddress, &o, lpBuffer, dwSize);
	}

	/// <summary>
	/// Writes an ANSI string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpString">- A pointer to a string containing the data to write.</param>
	/// <param name="bWriteNullTerminator">- A bool indicating whether or not to write the string null-terminator or not.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WriteAccessViolationException"/>
	inline void WriteStringA(
		_In_ ADDRESS uAddress,
		_In_z_ LPCSTR lpString,
		_In_ BOOL bWriteNullTerminator = TRUE
	) {
		if (!lpString) throw ParameterNullException("lpString");

		if (!XEWriteStringA(uAddress, lpString, bWriteNullTerminator)) throw WriteAccessViolationException(uAddress);
	}

	/// <summary>
	/// Writes an ANSI string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="string">- A string containing the data to write.</param>
	/// <param name="bWriteNullTerminator">- A bool indicating whether or not to write the string null-terminator or not.</param>
	/// <exception cref="WriteAccessViolationException"/>
	inline void WriteStringA(
		_In_ ADDRESS uAddress,
		_In_ String string,
		_In_ BOOL bWriteNullTerminator = TRUE
	) {
		if (!XEWriteStringA(uAddress, string.CStr(), bWriteNullTerminator)) throw WriteAccessViolationException(uAddress);
	}

	/// <summary>
	/// Writes a UTF-16LE string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpString">- A pointer to a string containing the data to write.</param>
	/// <param name="bWriteNullTerminator">- A bool indicating whether or not to write the string null-terminator or not.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WriteAccessViolationException"/>
	inline void WriteStringW(
		_In_ ADDRESS uAddress,
		_In_z_ LPCWSTR lpString,
		_In_ BOOL bWriteNullTerminator = TRUE
	) {
		if (!lpString) throw ParameterNullException("lpString");

		if (!XEWriteStringW(uAddress, lpString, bWriteNullTerminator)) throw WriteAccessViolationException(uAddress);
	}

	/// <summary>
	/// Writes a UTF-16LE string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="string">- A string containing the data to write.</param>
	/// <param name="bWriteNullTerminator">- A bool indicating whether or not to write the string null-terminator or not.</param>
	/// <exception cref="WriteAccessViolationException"/>
	inline void WriteStringW(
		_In_ ADDRESS uAddress,
		_In_ WString string,
		_In_ BOOL bWriteNullTerminator = TRUE
	) {
		if (!XEWriteStringW(uAddress, string.CStr(), bWriteNullTerminator)) throw WriteAccessViolationException(uAddress);
	}

	/// <summary>
	/// Writes bytes to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <param name="dwNop">- The number of bytes to set to 0x90 after the written bytes.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WriteAccessViolationException"/>
	inline void WriteBytes(
		_In_ ADDRESS uAddress,
		_In_ LPCBYTE lpBuffer,
		_In_ DWORD dwSize,
		_In_ DWORD dwNop = 0
	) {
		if (!lpBuffer) throw ParameterNullException("lpBuffer");

		if (!XEWriteBytes(uAddress, lpBuffer, dwSize, dwNop)) throw WriteAccessViolationException(uAddress);
	}

	/// <summary>
	/// Sets a number of bytes at an address to the specified value.
	/// </summary>
	/// <param name="uAddress">- The address to write data to.</param>
	/// <param name="uValue">- The value to set the bytes to.</param>
	/// <param name="dwCount">- The number of bytes to set.</param>
	/// <exception cref="WriteAccessViolationException"/>
	inline void SetBytes(
		_In_ ADDRESS uAddress,
		_In_ BYTE uValue,
		_In_ DWORD dwCount
	) {
		if (!XESetBytes(uAddress, uValue, dwCount)) throw WriteAccessViolationException(uAddress);
	}

#ifdef __VECTOR_H__

	/// <summary>
	/// Writes a Vec2 to an address.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="Vec">- The Vec2 to write.</param>
	/// <param name="lpMask">- The vector mask that indicates in which order the values should be written to memory.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WriteAccessViolationException"/>
	template<typename T>
	inline void WriteVec2(
		_In_ ADDRESS uAddress,
		_In_ Vec2<T> Vec,
		_In_ LPCSTR lpMask = "12"
	) {
		if (!XECheckVecMask(lpMask, 2)) throw ParameterInvalidException("lpMask");

		for (INT i = 0; i < 2; i++) {
			Write<T>(uAddress + (lpMask[i] - 0x31) * sizeof(T), Vec.Vec[i]);
		}
	}

	/// <summary>
	/// Writes a Vec3 to an address.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="Vec">- The Vec3 to write.</param>
	/// <param name="lpMask">- The vector mask that indicates in which order the values should be written to memory.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	template<typename T>
	inline void WriteVec3(
		_In_ ADDRESS uAddress,
		_In_ Vec3<T> Vec,
		_In_ LPCSTR lpMask = "123"
	) {
		if (!XECheckVecMask(lpMask, 3)) throw ParameterInvalidException("lpMask");

		for (INT i = 0; i < 3; i++) {
			Write<T>(uAddress + (lpMask[i] - 0x31) * sizeof(T), Vec.Vec[i]);
		}
	}

	/// <summary>
	/// Writes a Vec4 to an address.
	/// </summary>
	/// <typeparam name="T">- The type to write.</typeparam>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="Vec">- The Vec4 to write.</param>
	/// <param name="lpMask">- The vector mask that indicates in which order the values should be written to memory.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_READ_ACCESS_VIOLATION"/>
	template<typename T>
	inline void WriteVec4(
		_In_ ADDRESS uAddress,
		_In_ Vec4<T> Vec,
		_In_ LPCSTR lpMask = "1234"
	) {
		if (!XECheckVecMask(lpMask, 4)) throw ParameterInvalidException("lpMask");

		for (INT i = 0; i < 4; i++) {
			Write<T>(uAddress + (lpMask[i] - 0x31) * sizeof(T), Vec.Vec[i]);
		}
	}

#endif // __VECTOR_H__

}
#endif // __cplusplus

#endif // !__MEMAPI_WRITE_H__