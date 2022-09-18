#ifndef __ERRORAPI_H__
#define __ERRORAPI_H__

#include "Def.h"
#include "MemShApi.h"

#define XE_ERROR_SUCCESS				0x00
#define XE_ERROR_BUFFER_SMALL			0x01
#define XE_ERROR_WINAPI_FAIL			0x02
#define XE_ERROR_PARAMETER_INVALID		0x03
#define XE_ERROR_PARAMETER_NULL			0x04
#define XE_ERROR_MEMORY_FULL			0x05
#define XE_ERROR_FUNCTION_FAIL			0x06
#define XE_ERROR_READ_ACCESS_VIOLATION	0x07
#define XE_ERROR_WRITE_ACCESS_VIOLATION 0x08
#define XE_ERROR_SCAN_NOT_FOUND			0x09
#define XE_ERROR_ENTRY_NOT_FOUND		0x0A
#define XE_ERROR_HOOK_UNSUPPORTED		0x0B
#define XE_ERROR_HOOK_ALREADY_CREATED	0x0C
#define XE_ERROR_HOOK_NOT_EXECUTABLE	0x0D
#define XE_ERROR_HOOK_NOT_INIT			0x0E
#define XE_ERROR_HOOK_NOT_CREATED		0x0F
#define XE_ERROR_INDEX_OUT_OF_RANGE		0x10

/// <summary>
/// Represents an error that has occured.
/// </summary>
typedef struct _ERROR_ENTRY {
	/// <summary>
	/// The library error code.
	/// </summary>
	DWORD dwLibCode;

	/// <summary>
	/// The library error message.
	/// </summary>
	CHAR szLibMessage[256];

	/// <summary>
	/// The WinApi error code.
	/// </summary>
	DWORD dwWinCode;

	/// <summary>
	/// The WinApi error message.
	/// </summary>
	CHAR szWinMessage[256];
} ERROR_ENTRY, * LPERROR_ENTRY;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Retrieves the error codes of the last occuring error.
	/// </summary>
	/// <returns>
	/// <para>A 64-bit word.</para>
	/// <para>The first 32-bits represent the library error code.</para>
	/// <para>The last 32-bits represent the WinApi error code.</para>
	/// </returns>
	QWORD XEGetLastError();

	/// <summary>
	/// Sets the last error code.
	/// </summary>
	/// <param name="dwLibCode">- Error code to set.</param>
	/// <returns>Always FALSE.</returns>
	BOOL XESetLastError(_In_ DWORD dwLibCode);

	/// <summary>
	/// Sets the last error code to XE_ERROR_SUCCESS.
	/// </summary>
	/// <returns>Always TRUE.</returns>
	BOOL XEResetLastError();

	/// <summary>
	/// Retrieves an error entry containing extended error information of the last occuring error.
	/// </summary>
	/// <returns>The latest error entry.</returns>
	ERROR_ENTRY XEGetLastErrorEntry();

	/// <summary>
	/// Retrieves the error message representing the error code passed.
	/// </summary>
	/// <param name="dwLibCode">- The error code to retrieve a message from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the error message.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous parameter in bytes.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	BOOL XEGetErrorMessage(
		_In_ DWORD dwLibCode,
		_Out_writes_z_(dwSize) LPSTR lpBuffer,
		_In_ DWORD dwSize
	);

#ifdef __cplusplus
}

#include <Windows.h>

namespace api {
	/// <summary>
	/// The exception base class. All exception classes inherit from this base class. It is only used in specific cases where a custom message is needed.
	/// </summary>
	class Exception {
		CHAR szMessage[256];
	public:
		Exception() {
			szMessage[0] = '\0';
		}

		/// <summary>
		/// Constructs an Exception and fetches the message from the XEGetErrorMessage function using the provided code.
		/// </summary>
		/// <param name="dwCode">- The exception code to use to fetch the related exception information.</param>
		Exception(DWORD dwCode) {
			XEGetErrorMessage(dwCode, szMessage, sizeof(szMessage));
		}

		/// <summary>
		/// Constructs an Exception and sets the message to the message provided.
		/// </summary>
		/// <param name="lpMessage">- A pointer to the message string.</param>
		Exception(LPCSTR lpMessage) {
			strcpy_s(szMessage, lpMessage);
		}

		/// <summary>
		/// Gets the message for this Exception instance.
		/// </summary>
		/// <returns>The error message.</returns>
		LPCSTR Message() const { return szMessage; }
	};

	/// <summary>
	/// Thrown when a buffer is too small for the operation.
	/// </summary>
	class BufferSizeException : public Exception {
		CHAR szParamName[64];
		DWORD dwExpectedSize;
	public:
		/// <summary>
		/// Constructs a buffer size Exception.
		/// </summary>
		/// <param name="lpParamName">- The name of the buffer that is too small.</param>
		/// <param name="dwExpectedSize">- The required buffer size to complete the operation.</param>
		BufferSizeException(LPCSTR lpParamName, DWORD dwExpectedSize) : Exception(XE_ERROR_BUFFER_SMALL), dwExpectedSize(dwExpectedSize) {
			strcpy_s(szParamName, lpParamName);
		}

		/// <summary>
		/// Gets the expected buffer size for this instance.
		/// </summary>
		/// <returns>The expected buffer size.</returns>
		DWORD ExpectedSize() const { return dwExpectedSize; }

		/// <summary>
		/// Gets the buffer name for this instance.
		/// </summary>
		/// <returns>The buffer name.</returns>
		LPCSTR ParamName() const { return szParamName; };
	};

	/// <summary>
	/// The generic parameter exception base class.
	/// </summary>
	class ParameterException : public Exception {
		CHAR szParamName[64];
	public:
		/// <summary>
		/// Constructs a parameter Exception regarding the provided parameter.
		/// </summary>
		/// <param name="lpParamName">- The name of the regarded parameter.</param>
		/// <param name="dwCode">- The specific exception code for the exception instance.</param>
		ParameterException(LPCSTR lpParamName, DWORD dwCode) : Exception(dwCode) {
			strcpy_s(szParamName, lpParamName);
		}

		/// <summary>
		/// Gets the name of the regarded parameter for this instance.
		/// </summary>
		/// <returns>The name of the parameter.</returns>
		LPCSTR ParamName() const { return szParamName; }
	};

	/// <summary>
	/// Thrown when a required parameter is set to null.
	/// </summary>
	class ParameterNullException : public ParameterException {
	public:
		/// <summary>
		/// Constructs a parameter null Exception regarding the provided parameter.
		/// </summary>
		/// <param name="lpParamName">- The name of the regarded parameter.</param>
		ParameterNullException(LPCSTR lpParamName) : ParameterException(lpParamName, XE_ERROR_PARAMETER_NULL) {}
	};

	/// <summary>
	/// Thrown when a required parameter has an invalid value.
	/// </summary>
	class ParameterInvalidException : public ParameterException {
	public:
		/// <summary>
		/// Constructs a parameter invalid Exception regarding the provided parameter.
		/// </summary>
		/// <param name="lpParamName">- The name of the regarded parameter.</param>
		ParameterInvalidException(LPCSTR lpParamName) : ParameterException(lpParamName, XE_ERROR_PARAMETER_INVALID) {}
	};

	/// <summary>
	/// Thrown when a WinApi function has failed.
	/// </summary>
	class WinApiException : public Exception {
		CHAR szFunction[64];
		DWORD dwCode;
		CHAR szWinApiMessage[256];
	public:
		/// <summary>
		/// Constructs a WinApi Exception regarding the provided function.
		/// </summary>
		/// <param name="lpFunction">- The name of the regarded function.</param>
		WinApiException(LPCSTR lpFunction) : Exception(XE_ERROR_WINAPI_FAIL) {
			strcpy_s(szFunction, lpFunction);
			dwCode = GetLastError();
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, dwCode, LANG_SYSTEM_DEFAULT, szWinApiMessage, sizeof(szWinApiMessage), nullptr);
		}

		/// <summary>
		/// Gets the name of the WinApi function related to this instance.
		/// </summary>
		/// <returns>The name of the function.</returns>
		LPCSTR Function() const { return szFunction; }

		/// <summary>
		/// Gets the WinApi error code related to this instance.
		/// </summary>
		/// <returns>The WinApi code.</returns>
		DWORD WinCode() const { return dwCode; }

		/// <summary>
		/// Gets the WinApi error message related to this instance.
		/// </summary>
		/// <returns>The WinApi message.</returns>
		LPCSTR WinMessage() const { return szWinApiMessage; }
	};

	/// <summary>
	/// Thrown when memory cannot be allocated.
	/// </summary>
	class MemoryFullException : public Exception {
	public:
		/// <summary>
		/// Constructs a memory full Exception.
		/// </summary>
		MemoryFullException() : Exception(XE_ERROR_MEMORY_FULL) {}
	};

	/// <summary>
	/// Thrown when a function called through a prototype fails.
	/// </summary>
	class FunctionFailException : public Exception {
	public:
		/// <summary>
		/// Constructs a function failed Exception.
		/// </summary>
		FunctionFailException() : Exception(XE_ERROR_FUNCTION_FAIL) {}
	};

	/// <summary>
	/// Thrown when a memory read access violation is raised.
	/// </summary>
	class ReadAccessViolationException : public Exception {
		ADDRESS addr;
	public:
		/// <summary>
		/// Constructs a read access violation Exception regarding provided memory address.
		/// </summary>
		/// <param name="uAddress">- The regarded address.</param>
		ReadAccessViolationException(ADDRESS uAddress) : Exception(XE_ERROR_READ_ACCESS_VIOLATION), addr(uAddress) {}

		/// <summary>
		/// Gets the address where the read access violation happened.
		/// </summary>
		/// <returns>The regarded address.</returns>
		ADDRESS Address() const { return addr; }
	};

	/// <summary>
	/// Thrown when a memory write access violation is raised.
	/// </summary>
	class WriteAccessViolationException : public Exception {
		ADDRESS addr;
	public:
		/// <summary>
		/// Constructs a write access violation Exception regarding provided memory address.
		/// </summary>
		/// <param name="uAddress">- The regarded address.</param>
		WriteAccessViolationException(ADDRESS uAddress) : Exception(XE_ERROR_WRITE_ACCESS_VIOLATION), addr(uAddress) {}

		/// <summary>
		/// Gets the address where the write access violation happened.
		/// </summary>
		/// <returns>The regarded address.</returns>
		ADDRESS Address() const { return addr; }
	};

	/// <summary>
	/// Thrown when a scanner does not end up finding an entry.
	/// </summary>
	class ScanNotFoundException : public Exception {
	public:
		/// <summary>
		/// Constructs a scan not found Exception.
		/// </summary>
		ScanNotFoundException() : Exception(XE_ERROR_SCAN_NOT_FOUND) {}
	};

	/// <summary>
	/// Thrown when an entry of some sort does not end up being found.
	/// </summary>
	class EntryNotFoundException : public Exception {
		CHAR szEntryName[128];
	public:
		/// <summary>
		/// Constructs a entry not found Exception regarding the provided entry.
		/// </summary>
		/// <param name="lpEntryName">- The name of the regarded entry.</param>
		EntryNotFoundException(LPCSTR lpEntryName) : Exception(XE_ERROR_ENTRY_NOT_FOUND) {
			strcpy_s(szEntryName, lpEntryName);
		}

		/// <summary>
		/// Gets the name of the entry regarding this instance.
		/// </summary>
		/// <returns>The name of the entry.</returns>
		LPCSTR EntryName() const { return szEntryName; }
	};

	/// <summary>
	/// The generic hook exception base class.
	/// </summary>
	class HookException : public Exception {
	public:
		/// <summary>
		/// Constructs a hook Exception.
		/// </summary>
		/// <param name="dwCode">- The specific exception code for the exception instance.</param>
		HookException(DWORD dwCode) : Exception(dwCode) {}
	};

	/// <summary>
	/// Thrown when an attempt has been made to hook an unsupported function.
	/// </summary>
	class HookUnsupportedException : public HookException {
	public:
		/// <summary>
		/// Constructs a hook unsupported Exception.
		/// </summary>
		HookUnsupportedException() : HookException(XE_ERROR_HOOK_UNSUPPORTED) {}
	};

	/// <summary>
	/// Thrown when an attempt has been made to hook an already hooked function.
	/// </summary>
	class HookAlreadyCreatedException : public HookException {
	public:
		/// <summary>
		/// Constructs a hook already created Exception.
		/// </summary>
		HookAlreadyCreatedException() : HookException(XE_ERROR_HOOK_ALREADY_CREATED) {}
	};

	/// <summary>
	/// Thrown when an attempt has been made to hook a function without execution rights.
	/// </summary>
	class HookNotExecutableException : public HookException {
	public:
		/// <summary>
		/// Constructs a hook not executable Exception.
		/// </summary>
		HookNotExecutableException() : HookException(XE_ERROR_HOOK_NOT_EXECUTABLE) {}
	};

	/// <summary>
	/// Thrown when the hooking library is not initialized.
	/// </summary>
	class HookNotInitializedException : public HookException {
	public:
		/// <summary>
		/// Constructs a hook not initialized exception.
		/// </summary>
		HookNotInitializedException() : HookException(XE_ERROR_HOOK_NOT_INIT) {}
	};

	/// <summary>
	/// Thrown when an attempt has been made to interact with a hook that has not yet been created.
	/// </summary>
	class HookNotCreatedException : public HookException {
	public:
		/// <summary>
		/// Constructs a hook not created exception.
		/// </summary>
		HookNotCreatedException() : HookException(XE_ERROR_HOOK_NOT_CREATED) {}
	};

	/// <summary>
	/// Thrown when an index was out of range.
	/// </summary>
	class IndexOutOfRangeException : public Exception {
		INT nIndex;
		INT nMaxIndex;
	public:
		/// <summary>
		/// Constructs an index out of range exception.
		/// </summary>
		/// <param name="nIndex">- The current index.</param>
		/// <param name="nMaxIndex">- The maximum index.</param>
		IndexOutOfRangeException(INT nIndex, INT nMaxIndex) : Exception(XE_ERROR_INDEX_OUT_OF_RANGE), nIndex(nIndex), nMaxIndex(nMaxIndex) {}

		/// <summary>
		/// Gets the index associated with this instance.
		/// </summary>
		/// <returns>The associated index.</returns>
		INT Index() const { return nIndex; }

		/// <summary>
		/// Gets the maximum index associated with this instance.
		/// </summary>
		/// <returns>The associated index.</returns>
		INT MaxIndex() const { return nMaxIndex; }
	};
}
#endif // __cplusplus

#endif // !__ERRORAPI_H__