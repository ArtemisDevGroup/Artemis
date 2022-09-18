#ifndef __MEMAPI_SCAN_H__
#define __MEMAPI_SCAN_H__

#include "Def.h"
#include "ErrorApi.h"
#include "MemShApi.h"

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Scans a code style pattern and mask.
	/// </summary>
	/// <param name="lpPattern">- The pattern to scan.</param>
	/// <param name="lpMask">- The mask for the pattern.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_SCAN_NOT_FOUND"/>
	BOOL XEScanCodePattern(
		_In_ LPCSTR lpPattern,
		_In_z_ LPCSTR lpMask,
		_In_ ADDRESS uStartAddress,
		_In_ SIZE_T uScanSize,
		_In_ DWORD dwAlignment,
		_In_ SCAN_PROTECTION nScanProtection,
		_Out_ LPADDRESS lpReturn
	);

	/// <summary>
	/// Scans an IDA style pattern and mask.
	/// </summary>
	/// <param name="lpPattern">- The pattern to scan.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_SCAN_NOT_FOUND"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	/// <exception cref="XE_ERROR_BUFFER_SMALL"/>
	BOOL XEScanIdaPattern(
		_In_z_ LPCSTR lpPattern,
		_In_ ADDRESS uStartAddress,
		_In_ SIZE_T uScanSize,
		_In_ DWORD dwAlignment,
		_In_ SCAN_PROTECTION nScanProtection,
		_Out_ LPADDRESS lpReturn
	);

	/// <summary>
	/// Scans an ANSI string.
	/// </summary>
	/// <param name="lpString">- The string to scan.</param>
	/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_SCAN_NOT_FOUND"/>
	BOOL XEScanStringA(
		_In_z_ LPCSTR lpString,
		_In_ BOOL bCaseSensitive,
		_In_ ADDRESS uStartAddress,
		_In_ SIZE_T uScanSize,
		_In_ DWORD dwAlignment,
		_In_ SCAN_PROTECTION nScanProtection,
		_Out_ LPADDRESS lpReturn
	);

	/// <summary>
	/// Scans a UTF-16LE string.
	/// </summary>
	/// <param name="lpString">- The string to scan.</param>
	/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_SCAN_NOT_FOUND"/>
	BOOL XEScanStringW(
		_In_z_ LPCWSTR lpString,
		_In_ BOOL bCaseSensitive,
		_In_ ADDRESS uStartAddress,
		_In_ SIZE_T uScanSize,
		_In_ DWORD dwAlignment,
		_In_ SCAN_PROTECTION nScanProtection,
		_Out_ LPADDRESS lpReturn
	);

#ifdef __cplusplus
}

#include "ThreadApi.h"
#include "String.h"

namespace api {
	/// <summary>
	/// Scans a code style pattern and mask.
	/// </summary>
	/// <param name="lpPattern">- The pattern to scan.</param>
	/// <param name="lpMask">- The mask for the pattern.</param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="EntryNotFoundException"/>
	void ScanCodePattern(
		_In_ LPCSTR lpPattern,
		_In_z_ LPCSTR lpMask,
		_Out_ LPADDRESS lpReturn,
		_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
		_In_ SIZE_T uScanSize = CE_MAX_ADDR,
		_In_ DWORD dwAlignment = 1,
		_In_ SCAN_PROTECTION nScanProtection = SP_READ
	) {
		if (!XEScanCodePattern(lpPattern, lpMask, uStartAddress, uScanSize, dwAlignment, nScanProtection, lpReturn)) {
			switch ((DWORD)XEGetLastError()) {
			case XE_ERROR_PARAMETER_NULL:
				throw ParameterNullException("lpReturn");
			case XE_ERROR_PARAMETER_INVALID:
				throw ParameterInvalidException("nScanProtection");
			case XE_ERROR_SCAN_NOT_FOUND:
				throw EntryNotFoundException("Code pattern scan.");
			}
		}
	}

	/// <summary>
	/// A class for scanning a pattern in a different thread.
	/// </summary>
	class ScanCodePatternAsync {
		typedef struct _SCAN_CODE_PATTERN_FLAGS {
			CHAR szPattern[MAX_PATTERN_LENGTH];
			CHAR szMask[MAX_PATTERN_LENGTH];
			ADDRESS uStartAddress;
			SIZE_T uScanSize;
			DWORD dwAlignment;
			SCAN_PROTECTION nScanProtection;
			LPADDRESS lpReturn;
		} SCAN_CODE_PATTERN_FLAGS, * LPSCAN_CODE_PATTERN_FLAGS;

		static DWORD APIENTRY ScanThread(_Inout_ LPSCAN_CODE_PATTERN_FLAGS lpFlags) {
			if (!XEScanCodePattern(
				lpFlags->szPattern,
				lpFlags->szMask,
				lpFlags->uStartAddress,
				lpFlags->uScanSize,
				lpFlags->dwAlignment,
				lpFlags->nScanProtection,
				lpFlags->lpReturn
			)) return (DWORD)XEGetLastError();
		}

		SCAN_CODE_PATTERN_FLAGS flags;
		Thread<SCAN_CODE_PATTERN_FLAGS> thread;
		ADDRESS uReturn;
	public:
		/// <summary>
		/// Constructs a scan.
		/// </summary>
		/// <param name="lpPattern">- The pattern to scan.</param>
		/// <param name="lpMask">- The mask for the pattern.</param>
		/// <param name="uStartAddress">- The address to start scanning at.</param>
		/// <param name="uScanSize">- Number of bytes to scan.</param>
		/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
		/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
		/// <param name="nScanProtection">- Defines which memory protections to scan for.
		/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
		ScanCodePatternAsync(
			_In_ LPCSTR lpPattern,
			_In_z_ LPCSTR lpMask,
			_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
			_In_ SIZE_T uScanSize = CE_MAX_ADDR,
			_In_ DWORD dwAlignment = 1,
			_In_ SCAN_PROTECTION nScanProtection = SP_READ
		) {
			uReturn = 0;

			memcpy(flags.szPattern, lpPattern, strlen(lpMask));
			strncpy_s(flags.szMask, lpMask, strlen(lpMask) + 1);
			flags.uStartAddress = uStartAddress;
			flags.uScanSize = uScanSize;
			flags.dwAlignment = dwAlignment;
			flags.nScanProtection = nScanProtection;
			flags.lpReturn = &uReturn;

			thread.SetFunctionPtr(ScanThread);
			thread.SetParameterPtr(&flags);
		}

		/// <summary>
		/// Starts the scan.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void StartScan() { thread.Start(); }

		/// <summary>
		/// Waits for the scan to finish.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void Wait() { thread.Wait(); }

		/// <summary>
		/// Gets the return value of the thread function when it has finished.
		/// <para>A generic exception is thrown when an attempt to call this function has been made before the thread has finished execution.</para>
		/// </summary>
		/// <returns>The thread return value.</returns>
		/// <exception cref="WinApiException"/>
		/// <exception cref="Exception"/>
		DWORD GetReturnValue() { return thread.GetReturnValue(); }

		/// <summary>
		/// Gets the return address. If the function has not finished executing this function will return 0.
		/// </summary>
		/// <returns>The found address.</returns>
		ADDRESS GetReturnAddress() { return uReturn; }
	};

	/// <summary>
	/// Scans an IDA style pattern and mask.
	/// </summary>
	/// <param name="lpPattern">- The pattern to scan.</param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="EntryNotFoundException"/>
	/// <exception cref="WinApiException"/>
	/// <exception cref="BufferSizeException"/>
	void ScanIdaPattern(
		_In_z_ LPCSTR lpPattern,
		_Out_ LPADDRESS lpReturn,
		_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
		_In_ SIZE_T uScanSize = CE_MAX_ADDR,
		_In_ DWORD dwAlignment = 1,
		_In_ SCAN_PROTECTION nScanProtection = SP_READ
	) {
		if (!XEScanIdaPattern(lpPattern, uStartAddress, uScanSize, dwAlignment, nScanProtection, lpReturn)) {
			switch ((DWORD)XEGetLastError()) {
			case XE_ERROR_PARAMETER_NULL:
				throw ParameterNullException("lpReturn");
			case XE_ERROR_PARAMETER_INVALID:
				throw ParameterInvalidException("nScanProtection");
			case XE_ERROR_SCAN_NOT_FOUND:
				throw EntryNotFoundException("Ida pattern scan.");
			case XE_ERROR_WINAPI_FAIL:
				throw WinApiException("VirtualAlloc");
			case XE_ERROR_BUFFER_SMALL:
				throw BufferSizeException(0, MAX_PATTERN_LENGTH);
			}
		}
	}

	/// <summary>
	/// A class for scanning a pattern in a different thread.
	/// </summary>
	class ScanIdaPatternAsync {
		typedef struct _SCAN_IDA_PATTERN_FLAGS {
			CHAR szPattern[MAX_PATTERN_LENGTH * 2];
			ADDRESS uStartAddress;
			SIZE_T uScanSize;
			DWORD dwAlignment;
			SCAN_PROTECTION nScanProtection;
			LPADDRESS lpReturn;
		} SCAN_IDA_PATTERN_FLAGS, * LPSCAN_IDA_PATTERN_FLAGS;

		static DWORD APIENTRY ScanThread(_Inout_ LPSCAN_IDA_PATTERN_FLAGS lpFlags) {
			if (!XEScanIdaPattern(
				lpFlags->szPattern,
				lpFlags->uStartAddress,
				lpFlags->uScanSize,
				lpFlags->dwAlignment,
				lpFlags->nScanProtection,
				lpFlags->lpReturn
			)) return (DWORD)XEGetLastError();
		}

		SCAN_IDA_PATTERN_FLAGS flags;
		Thread<SCAN_IDA_PATTERN_FLAGS> thread;
		ADDRESS uReturn;
	public:
		/// <summary>
		/// Constructs a scan.
		/// </summary>
		/// <param name="lpPattern">- The pattern to scan.</param>
		/// <param name="uStartAddress">- The address to start scanning at.</param>
		/// <param name="uScanSize">- Number of bytes to scan.</param>
		/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
		/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
		/// <param name="nScanProtection">- Defines which memory protections to scan for.
		/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
		ScanIdaPatternAsync(
			_In_ LPCSTR lpPattern,
			_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
			_In_ SIZE_T uScanSize = CE_MAX_ADDR,
			_In_ DWORD dwAlignment = 1,
			_In_ SCAN_PROTECTION nScanProtection = SP_READ
		) {
			uReturn = 0;

			strcpy_s(flags.szPattern, lpPattern);
			flags.uStartAddress = uStartAddress;
			flags.uScanSize = uScanSize;
			flags.dwAlignment = dwAlignment;
			flags.nScanProtection = nScanProtection;
			flags.lpReturn = &uReturn;

			thread.SetFunctionPtr(ScanThread);
			thread.SetParameterPtr(&flags);
		}

		/// <summary>
		/// Starts the scan.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void StartScan() { thread.Start(); }

		/// <summary>
		/// Waits for the scan to finish.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void Wait() { thread.Wait(); }

		/// <summary>
		/// Gets the return value of the thread function when it has finished.
		/// <para>A generic exception is thrown when an attempt to call this function has been made before the thread has finished execution.</para>
		/// </summary>
		/// <returns>The thread return value.</returns>
		/// <exception cref="WinApiException"/>
		/// <exception cref="Exception"/>
		DWORD GetReturnValue() { return thread.GetReturnValue(); }

		/// <summary>
		/// Gets the return address. If the function has not finished executing this function will return 0.
		/// </summary>
		/// <returns>The found address.</returns>
		ADDRESS GetReturnAddress() { return uReturn; }
	};

	/// <summary>
	/// Scans an ANSI string.
	/// </summary>
	/// <param name="lpString">- The string to scan.</param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="EntryNotFoundException"/>
	void ScanStringA(
		_In_z_ LPCSTR lpString,
		_Out_ LPADDRESS lpReturn,
		_In_ BOOL bCaseSensitive = TRUE,
		_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
		_In_ SIZE_T uScanSize = CE_MAX_ADDR,
		_In_ DWORD dwAlignment = 1,
		_In_ SCAN_PROTECTION nScanProtection = SP_READ
	) {
		if (!XEScanStringA(lpString, bCaseSensitive, uStartAddress, uScanSize, dwAlignment, nScanProtection, lpReturn)) {
			switch ((DWORD)XEGetLastError()) {
			case XE_ERROR_PARAMETER_NULL:
				throw ParameterNullException("lpReturn");
			case XE_ERROR_PARAMETER_INVALID:
				throw ParameterInvalidException("nScanProtection");
			case XE_ERROR_SCAN_NOT_FOUND:
				throw EntryNotFoundException("Code pattern scan.");
			}
		}
	}

	/// <summary>
	/// Scans an ANSI string.
	/// </summary>
	/// <param name="string">- The string to scan.</param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="EntryNotFoundException"/>
	void ScanStringA(
		_In_ String string,
		_Out_ LPADDRESS lpReturn,
		_In_ BOOL bCaseSensitive = TRUE,
		_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
		_In_ SIZE_T uScanSize = CE_MAX_ADDR,
		_In_ DWORD dwAlignment = 1,
		_In_ SCAN_PROTECTION nScanProtection = SP_READ
	) {
		if (!XEScanStringA(string.CStr(), bCaseSensitive, uStartAddress, uScanSize, dwAlignment, nScanProtection, lpReturn)) {
			switch ((DWORD)XEGetLastError()) {
			case XE_ERROR_PARAMETER_NULL:
				throw ParameterNullException("lpReturn");
			case XE_ERROR_PARAMETER_INVALID:
				throw ParameterInvalidException("nScanProtection");
			case XE_ERROR_SCAN_NOT_FOUND:
				throw EntryNotFoundException("Code pattern scan.");
			}
		}
	}

	/// <summary>
	/// A class for scanning a string in a different thread.
	/// </summary>
	class ScanStringAsyncA {
		typedef struct _SCAN_STRING_FLAGS_A {
			CHAR szString[MAX_PATTERN_LENGTH];
			BOOL bCaseSensitive;
			ADDRESS uStartAddress;
			SIZE_T uScanSize;
			DWORD dwAlignment;
			SCAN_PROTECTION nScanProtection;
			LPADDRESS lpReturn;
		} SCAN_STRING_FLAGS_A, * LPSCAN_STRING_FLAGS_A;

		static DWORD APIENTRY ScanThread(_Inout_ LPSCAN_STRING_FLAGS_A lpFlags) {
			if (!XEScanStringA(
				lpFlags->szString,
				lpFlags->bCaseSensitive,
				lpFlags->uStartAddress,
				lpFlags->uScanSize,
				lpFlags->dwAlignment,
				lpFlags->nScanProtection,
				lpFlags->lpReturn
			)) return (DWORD)XEGetLastError();
		}

		SCAN_STRING_FLAGS_A flags;
		Thread<SCAN_STRING_FLAGS_A> thread;
		ADDRESS uReturn;
	public:
		/// <summary>
		/// Scans an ANSI string.
		/// </summary>
		/// <param name="lpString">- The string to scan.</param>
		/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
		/// <param name="uStartAddress">- The address to start scanning at.</param>
		/// <param name="uScanSize">- Number of bytes to scan.</param>
		/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
		/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
		/// <param name="nScanProtection">- Defines which memory protections to scan for.
		/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
		ScanStringAsyncA(
			_In_z_ LPCSTR lpString,
			_In_ BOOL bCaseSensitive = TRUE,
			_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
			_In_ SIZE_T uScanSize = CE_MAX_ADDR,
			_In_ DWORD dwAlignment = 1,
			_In_ SCAN_PROTECTION nScanProtection = SP_READ
		) {
			uReturn = 0;

			strcpy_s(flags.szString, lpString);
			flags.bCaseSensitive = bCaseSensitive;
			flags.uStartAddress = uStartAddress;
			flags.uScanSize = uScanSize;
			flags.dwAlignment = dwAlignment;
			flags.nScanProtection = nScanProtection;
			flags.lpReturn = &uReturn;

			thread.SetFunctionPtr(ScanThread);
			thread.SetParameterPtr(&flags);
		}

		/// <summary>
		/// Scans an ANSI string.
		/// </summary>
		/// <param name="string">- The string to scan.</param>
		/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
		/// <param name="uStartAddress">- The address to start scanning at.</param>
		/// <param name="uScanSize">- Number of bytes to scan.</param>
		/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
		/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
		/// <param name="nScanProtection">- Defines which memory protections to scan for.
		/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
		ScanStringAsyncA(
			_In_ String string,
			_In_ BOOL bCaseSensitive = TRUE,
			_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
			_In_ SIZE_T uScanSize = CE_MAX_ADDR,
			_In_ DWORD dwAlignment = 1,
			_In_ SCAN_PROTECTION nScanProtection = SP_READ
		) {
			uReturn = 0;

			strcpy_s(flags.szString, string.CStr());
			flags.bCaseSensitive = bCaseSensitive;
			flags.uStartAddress = uStartAddress;
			flags.uScanSize = uScanSize;
			flags.dwAlignment = dwAlignment;
			flags.nScanProtection = nScanProtection;
			flags.lpReturn = &uReturn;

			thread.SetFunctionPtr(ScanThread);
			thread.SetParameterPtr(&flags);
		}

		/// <summary>
		/// Starts the scan.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void StartScan() { thread.Start(); }

		/// <summary>
		/// Waits for the scan to finish.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void Wait() { thread.Wait(); }

		/// <summary>
		/// Gets the return value of the thread function when it has finished.
		/// <para>A generic exception is thrown when an attempt to call this function has been made before the thread has finished execution.</para>
		/// </summary>
		/// <returns>The thread return value.</returns>
		/// <exception cref="WinApiException"/>
		/// <exception cref="Exception"/>
		DWORD GetReturnValue() { return thread.GetReturnValue(); }

		/// <summary>
		/// Gets the return address. If the function has not finished executing this function will return 0.
		/// </summary>
		/// <returns>The found address.</returns>
		ADDRESS GetReturnAddress() { return uReturn; }
	};

	/// <summary>
	/// Scans a UTF-16LE string.
	/// </summary>
	/// <param name="lpString">- The string to scan.</param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="EntryNotFoundException"/>
	void ScanStringW(
		_In_z_ LPCWSTR lpString,
		_Out_ LPADDRESS lpReturn,
		_In_ BOOL bCaseSensitive = TRUE,
		_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
		_In_ SIZE_T uScanSize = CE_MAX_ADDR,
		_In_ DWORD dwAlignment = 1,
		_In_ SCAN_PROTECTION nScanProtection = SP_READ
	) {
		if (!XEScanStringW(lpString, bCaseSensitive, uStartAddress, uScanSize, dwAlignment, nScanProtection, lpReturn)) {
			switch ((DWORD)XEGetLastError()) {
			case XE_ERROR_PARAMETER_NULL:
				throw ParameterNullException("lpReturn");
			case XE_ERROR_PARAMETER_INVALID:
				throw ParameterInvalidException("nScanProtection");
			case XE_ERROR_SCAN_NOT_FOUND:
				throw EntryNotFoundException("Code pattern scan.");
			}
		}
	}

	/// <summary>
	/// Scans a UTF-16LE string.
	/// </summary>
	/// <param name="string">- The string to scan.</param>
	/// <param name="lpReturn">- A pointer to an ADDRESS object to receive the found address.</param>
	/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
	/// <param name="uStartAddress">- The address to start scanning at.</param>
	/// <param name="uScanSize">- Number of bytes to scan.</param>
	/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
	/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
	/// <param name="nScanProtection">- Defines which memory protections to scan for.
	/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="EntryNotFoundException"/>
	void ScanStringW(
		_In_ WString string,
		_Out_ LPADDRESS lpReturn,
		_In_ BOOL bCaseSensitive = TRUE,
		_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
		_In_ SIZE_T uScanSize = CE_MAX_ADDR,
		_In_ DWORD dwAlignment = 1,
		_In_ SCAN_PROTECTION nScanProtection = SP_READ
	) {
		if (!XEScanStringW(string.CStr(), bCaseSensitive, uStartAddress, uScanSize, dwAlignment, nScanProtection, lpReturn)) {
			switch ((DWORD)XEGetLastError()) {
			case XE_ERROR_PARAMETER_NULL:
				throw ParameterNullException("lpReturn");
			case XE_ERROR_PARAMETER_INVALID:
				throw ParameterInvalidException("nScanProtection");
			case XE_ERROR_SCAN_NOT_FOUND:
				throw EntryNotFoundException("Code pattern scan.");
			}
		}
	}

	/// <summary>
	/// A class for scanning a string in a different thread.
	/// </summary>
	class ScanStringAsyncW {
		typedef struct _SCAN_STRING_FLAGS_W {
			WCHAR szString[MAX_PATTERN_LENGTH];
			BOOL bCaseSensitive;
			ADDRESS uStartAddress;
			SIZE_T uScanSize;
			DWORD dwAlignment;
			SCAN_PROTECTION nScanProtection;
			LPADDRESS lpReturn;
		} SCAN_STRING_FLAGS_W, * LPSCAN_STRING_FLAGS_W;

		static DWORD APIENTRY ScanThread(_Inout_ LPSCAN_STRING_FLAGS_W lpFlags) {
			if (!XEScanStringW(
				lpFlags->szString,
				lpFlags->bCaseSensitive,
				lpFlags->uStartAddress,
				lpFlags->uScanSize,
				lpFlags->dwAlignment,
				lpFlags->nScanProtection,
				lpFlags->lpReturn
			)) return (DWORD)XEGetLastError();
		}

		SCAN_STRING_FLAGS_W flags;
		Thread<SCAN_STRING_FLAGS_W> thread;
		ADDRESS uReturn;
	public:
		/// <summary>
		/// Scans a UTF-16LE string.
		/// </summary>
		/// <param name="lpString">- The string to scan.</param>
		/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
		/// <param name="uStartAddress">- The address to start scanning at.</param>
		/// <param name="uScanSize">- Number of bytes to scan.</param>
		/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
		/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
		/// <param name="nScanProtection">- Defines which memory protections to scan for.
		/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
		ScanStringAsyncW(
			_In_z_ LPCWSTR lpString,
			_In_ BOOL bCaseSensitive = TRUE,
			_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
			_In_ SIZE_T uScanSize = CE_MAX_ADDR,
			_In_ DWORD dwAlignment = 1,
			_In_ SCAN_PROTECTION nScanProtection = SP_READ
		) {
			uReturn = 0;

			wcscpy_s(flags.szString, lpString);
			flags.bCaseSensitive = bCaseSensitive;
			flags.uStartAddress = uStartAddress;
			flags.uScanSize = uScanSize;
			flags.dwAlignment = dwAlignment;
			flags.nScanProtection = nScanProtection;
			flags.lpReturn = &uReturn;

			thread.SetFunctionPtr(ScanThread);
			thread.SetParameterPtr(&flags);
		}

		/// <summary>
		/// Scans an UTF-16LE string.
		/// </summary>
		/// <param name="string">- The string to scan.</param>
		/// <param name="bCaseSensitive">- Defines whether the scanner should care about casing or not.</param>
		/// <param name="uStartAddress">- The address to start scanning at.</param>
		/// <param name="uScanSize">- Number of bytes to scan.</param>
		/// <param name="dwAlignment">- Number of bytes to add for every iteration of the loop.
		/// <para>A higher number gives a faster scan but less chance of actually finding anything.</para><para>For a lower number it is vice versa.</para></param>
		/// <param name="nScanProtection">- Defines which memory protections to scan for.
		/// <para>For example, when you are scanning for op codes it is way more efficient</para><para>to only scan executable readonly memory as opposed to all memory.</para></param>
		ScanStringAsyncW(
			_In_ WString string,
			_In_ BOOL bCaseSensitive = TRUE,
			_In_ ADDRESS uStartAddress = CE_MIN_ADDR,
			_In_ SIZE_T uScanSize = CE_MAX_ADDR,
			_In_ DWORD dwAlignment = 1,
			_In_ SCAN_PROTECTION nScanProtection = SP_READ
		) {
			uReturn = 0;

			wcscpy_s(flags.szString, string.CStr());
			flags.bCaseSensitive = bCaseSensitive;
			flags.uStartAddress = uStartAddress;
			flags.uScanSize = uScanSize;
			flags.dwAlignment = dwAlignment;
			flags.nScanProtection = nScanProtection;
			flags.lpReturn = &uReturn;

			thread.SetFunctionPtr(ScanThread);
			thread.SetParameterPtr(&flags);
		}

		/// <summary>
		/// Starts the scan.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void StartScan() { thread.Start(); }

		/// <summary>
		/// Waits for the scan to finish.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void Wait() { thread.Wait(); }

		/// <summary>
		/// Gets the return value of the thread function when it has finished.
		/// <para>A generic exception is thrown when an attempt to call this function has been made before the thread has finished execution.</para>
		/// </summary>
		/// <returns>The thread return value.</returns>
		/// <exception cref="WinApiException"/>
		/// <exception cref="Exception"/>
		DWORD GetReturnValue() { return thread.GetReturnValue(); }

		/// <summary>
		/// Gets the return address. If the function has not finished executing this function will return 0.
		/// </summary>
		/// <returns>The found address.</returns>
		ADDRESS GetReturnAddress() { return uReturn; }
	};
}
#endif // __cplusplus

#endif // !__MEMAPI_SCAN_H__