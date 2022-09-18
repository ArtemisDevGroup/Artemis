#ifndef __FILEAPI_H__
#define __FILEAPI_H__

#include "Def.h"
#include "ErrorApi.h"

#include <Windows.h>

/// <summary>
/// Defines the access level to obtain when creating/opening this file.
/// </summary>
typedef enum _FILE_ACCESS {
	/// <summary>
	/// Obtains read permissions.
	/// </summary>
	FA_READ = FILE_READ_ATTRIBUTES | FILE_READ_DATA | FILE_READ_EA | STANDARD_RIGHTS_READ,

	/// <summary>
	/// Obtains write permissions.
	/// </summary>
	FA_WRITE = FILE_WRITE_ATTRIBUTES | FILE_WRITE_DATA | FILE_WRITE_EA | STANDARD_RIGHTS_WRITE,

	/// <summary>
	/// Obtains delete permissions.
	/// </summary>
	FA_DELETE = DELETE,

	/// <summary>
	/// Obtains read and write permissions.
	/// </summary>
	FA_READWRITE = FA_READ | FA_WRITE,
} FILE_ACCESS;

/// <summary>
/// Defines the share mode of the file.
/// </summary>
typedef enum _SHARE_MODE {
	/// <summary>
	/// Makes the file private and only accessible through the current handle until aftermentioned handle has been closed.
	/// </summary>
	SM_NONE = 0,

	/// <summary>
	/// Allows subsequent open operations to request delete access.
	/// </summary>
	SM_DELETE = FILE_SHARE_DELETE,

	/// <summary>
	/// Allows subsequent open operations to request read access.
	/// </summary>
	SM_READ = FILE_SHARE_READ,

	/// <summary>
	/// Allows subsequent open operations to request write access.
	/// </summary>
	SM_WRITE = FILE_SHARE_WRITE,

	/// <summary>
	/// Allows subsequent open operations to request read and write access.
	/// </summary>
	SM_READWRITE = FILE_SHARE_READ | FILE_SHARE_WRITE
} SHARE_MODE;

/// <summary>
/// Defines how the file should be created/opened.
/// </summary>
typedef enum _CREATION_DISPOSITION {
	/// <summary>
	/// <para>If the file exists:
	///		Overwrites the file with a new empty file.</para>
	/// <para>If the file doesn't exist:
	///		Creates a new file.</para>
	/// </summary>
	CD_CREATE_ALWAYS = CREATE_ALWAYS,

	/// <summary>
	/// <para>If the file exists:
	///		Fails with error code ERROR_FILE_EXISTS (80).</para>
	/// <para>If the file doesn't exist:
	///		Creates a new file.</para>
	/// </summary>
	CD_CREATE_NEW = CREATE_NEW,

	/// <summary>
	/// <para>If the file exists:
	///		Opens the file.</para>
	/// <para>If the file doesn't exist:
	///		Creates a new file.</para>
	/// </summary>
	CD_OPEN_ALWAYS = OPEN_ALWAYS,

	/// <summary>
	/// <para>If the file exists:
	///		Opens the file.</para>
	/// <para>If the file doesn't exist:
	///		Fails with error code ERROR_FILE_NOT_FOUND (2).</para>
	/// </summary>
	CD_OPEN_EXISTING = OPEN_EXISTING,

	/// <summary>
	/// <para>If the file exists:
	///		Opens the file and truncates it to a size of 0 bytes.</para>
	/// <para>If the file doesn't exist:
	///		Fails with error code ERROR_FILE_NOT_FOUND (2).</para>
	/// </summary>
	CD_TRUNCATE_EXISTING = TRUNCATE_EXISTING
} CREATION_DISPOSITION;

typedef enum _FILE_ATTRIBUTE {
	/// <summary>
	/// The file is intended to be archived.
	/// </summary>
	FT_ARCHIVE = FILE_ATTRIBUTE_ARCHIVE,

	/// <summary>
	/// The file is encrypted.
	/// </summary>
	FT_ENCRYPTED = FILE_ATTRIBUTE_ENCRYPTED,

	/// <summary>
	/// The file is hidden.
	/// </summary>
	FT_HIDDEN = FILE_ATTRIBUTE_HIDDEN,

	/// <summary>
	/// The file has no special attributes.
	/// </summary>
	FT_NORMAL = FILE_ATTRIBUTE_NORMAL,

	/// <summary>
	/// The file is readonly and cannot be changed or deleted.
	/// </summary>
	FT_READONLY = FILE_ATTRIBUTE_READONLY,

	/// <summary>
	/// The file is part of or exclusively used by an operating system.
	/// </summary>
	FT_SYSTEM = FILE_ATTRIBUTE_SYSTEM,

	/// <summary>
	/// The file is being used for temporary storage.
	/// </summary>
	FT_TEMPORARY = FILE_ATTRIBUTE_TEMPORARY
} FILE_ATTRIBUTE;

/// <summary>
/// Represents a file entry. Is used by the FileApi C functions.
/// </summary>
typedef struct _FILE_ENTRY {
	/// <summary>
	/// The path of the active file.
	/// </summary>
	CHAR szFilePath[MAX_PATH];

	/// <summary>
	/// A handle to the active file.
	/// </summary>
	HANDLE hFile;

	/// <summary>
	/// Represents the access mode of the file. See the FILE_ACCESS enum.
	/// </summary>
	FILE_ACCESS dwFileAccess;

	/// <summary>
	/// Represents the share mode of the file. See the SHARE_MODE enum.
	/// </summary>
	SHARE_MODE dwShareMode;
	
	/// <summary>
	/// Represents the creation disposition of the file. See the CREATION_DISPOSITION enum.
	/// </summary>
	CREATION_DISPOSITION dwCreationDisposition;

	/// <summary>
	/// Represents a file attribute. See the FILE_ATTRIBUTE enum.
	/// </summary>
	FILE_ATTRIBUTE dwFileAttribute;

	/// <summary>
	/// Shows whether the current object is active or not.
	/// </summary>
	BOOL bActive;
} FILE_ENTRY, * LPFILE_ENTRY;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Creates or opens a file and writes information to the FILE_ENTRY object pointed to by the lpFileEntry parameter.
	/// </summary>
	/// <param name="lpFileEntry">- A pointer to an empty FILE_ENTRY object.</param>
	/// <param name="lpFilePath">- A path to the location of the file.</param>
	/// <param name="dwFileAccess">- A constant representing the access rights to the file. See the FILE_ACCESS enum.</param>
	/// <param name="dwShareMode">- A constant representing the share mode of the file. See the SHARE_MODE enum.</param>
	/// <param name="dwCreationDisposition">- A constant representing how the file should be created/opened.</param>
	/// <param name="dwFileAttribute">- A constant representing a file attribute.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	/// <exception cref="ERROR_FILE_NOT_FOUND"/>
	/// <exception cref="ERROR_FILE_EXISTS"/>
	/// <exception cref="ERROR_ACCESS_DENIED"/>
	BOOL XECreateFile(
		_Out_ LPFILE_ENTRY lpFileEntry,
		_In_z_ LPCSTR lpFilePath,
		_In_ FILE_ACCESS dwFileAccess,
		_In_ SHARE_MODE dwShareMode,
		_In_ CREATION_DISPOSITION dwCreationDisposition,
		_In_ FILE_ATTRIBUTE dwFileAttribute
	);

	/// <summary>
	/// Reads from the file represented by the FILE_ENTRY object pointed to by the pointer.
	/// </summary>
	/// <param name="lpFileEntry">- A pointer to an initialized FILE_ENTRY object.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous parameter in bytes.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEReadFile(
		_In_ LPFILE_ENTRY lpFileEntry,
		_Out_writes_(dwSize) LPVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Writes to the file represented by the FILE_ENTRY object pointed to by the pointer.
	/// </summary>
	/// <param name="lpFileEntry">- A pointer to an initialized FILE_ENTRY object.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to read data from.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous parameter in bytes.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEWriteFile(
		_In_ LPFILE_ENTRY lpFileEntry,
		_In_ LPCVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Prints a format string to the file represented by the FILE_ENTRY object pointed to by the pointer.
	/// </summary>
	/// <param name="lpFileEntry">- A pointer to an initialized FILE_ENTRY object.</param>
	/// <param name="lpFormat">- A printf format string.</param>
	/// <param name="...">- Parameters for the format string.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEPrintFile(
		_In_ LPFILE_ENTRY lpFileEntry,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		...
	);

	/// <summary>
	/// Prints a format string to the file represented by the FILE_ENTRY object pointed to by the pointer.
	/// </summary>
	/// <param name="lpFileEntry">- A pointer to an initialized FILE_ENTRY object.</param>
	/// <param name="lpFormat">- A printf format string.</param>
	/// <param name="lpArgs">- Parameters for the format string.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEPrintFileV(
		_In_ LPFILE_ENTRY lpFileEntry,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		_In_ va_list lpArgs
	);

	/// <summary>
	/// Deletes the file represented by the FILE_ENTRY object pointed to by the pointer.
	/// </summary>
	/// <param name="lpFileEntry">- A pointer to an initialized FILE_ENTRY object.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEDeleteFile(
		_In_ LPFILE_ENTRY lpFileEntry
	);

	/// <summary>
	/// Closes an active FILE_ENTRY object.
	/// </summary>
	/// <param name="lpFileEntry">- A pointer to an initialized FILE_ENTRY object.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XECloseFile(
		_In_ LPFILE_ENTRY lpFileEntry
	);

#ifdef __cplusplus
}

namespace api {
	/// <summary>
	/// A class that represents a file and contains functions used for interacting with it.
	/// </summary>
	class File {
		FILE_ENTRY fe;

	public:
		inline File() : fe() {}

		/// <summary>
		/// Constructs a class object and creates or opens a file for IO operations.
		/// </summary>
		/// <param name="lpFilePath">- A path to the location of the file.</param>
		/// <param name="dwFileAccess">- A constant representing the access rights to the file. See the FILE_ACCESS enum.</param>
		/// <param name="dwShareMode">- A constant representing the share mode of the file. See the SHARE_MODE enum.</param>
		/// <param name="dwCreationDisposition">- A constant representing how the file should be created/opened.</param>
		/// <param name="dwFileAttribute">- A constant representing a file attribute.</param>
		/// <exception cref="ParamterNullException"/>
		/// <exception cref="WinApiException"/>
		inline File(
			_In_z_ LPCSTR lpFilePath,
			_In_ FILE_ACCESS dwFileAccess,
			_In_ SHARE_MODE dwShareMode,
			_In_ CREATION_DISPOSITION dwCreationDisposition,
			_In_ FILE_ATTRIBUTE dwFileAttribute
		) {
			if (!lpFilePath) throw ParameterNullException("lpFilePath");

			if (!XECreateFile(&fe, lpFilePath, dwFileAccess, dwShareMode, dwCreationDisposition, dwFileAttribute)) throw WinApiException("CreateFileA");
		}

		inline ~File() {
			if (!XECloseFile(&fe)) throw WinApiException("CloseHandle");
		}

		/// <summary>
		/// Reads from the file represented by the FILE_ENTRY object pointed to by the pointer.
		/// </summary>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
		/// <param name="dwSize">- The size of the buffer pointed to by the previous parameter in bytes.</param>
		/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
		/// <exception cref="WinApiException"/>
		inline void Read(
			_Out_writes_(dwSize) LPVOID lpBuffer,
			_In_ DWORD dwSize
		) {
			if (!XEReadFile(&fe, lpBuffer, dwSize)) throw WinApiException("ReadFile");
		}

		/// <summary>
		/// Writes to the file represented by the FILE_ENTRY object pointed to by the pointer.
		/// </summary>
		/// <param name="lpBuffer">- A pointer to a buffer to read data from.</param>
		/// <param name="dwSize">- The size of the buffer pointed to by the previous parameter in bytes.</param>
		/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
		/// <exception cref="WinApiException"/>
		inline void Write(
			_In_ LPCVOID lpBuffer,
			_In_ DWORD dwSize
		) {
			if (!XEWriteFile(&fe, lpBuffer, dwSize)) throw WinApiException("WriteFile");
		}

		/// <summary>
		/// Prints a format string to the file represented by the FILE_ENTRY object pointed to by the pointer.
		/// </summary>
		/// <param name="lpFormat">- A printf format string.</param>
		/// <param name="...">- Parameters for the format string.</param>
		/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
		/// <exception cref="WinApiException"/>
		inline void Print(
			_In_z_ _Printf_format_string_ LPCSTR lpFormat,
			...
		) {
			va_list va;
			va_start(va, lpFormat);
			BOOL b = XEPrintFileV(&fe, lpFormat, va);
			va_end(va);

			if (!b) throw WinApiException("WriteFile");
		}

		/// <summary>
		/// Deletes the file represented by the FILE_ENTRY object pointed to by the pointer.
		/// </summary>
		/// <param name="lpFileEntry">- A pointer to an initialized FILE_ENTRY object.</param>
		/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
		/// <exception cref="WinApiException"/>
		inline void Delete() {
			if (!XEDeleteFile(&fe)) throw WinApiException("DeleteFileA");
		}

		inline void Close() {
			if (!XECloseFile(&fe)) throw WinApiException("CloseHandle");
		}
	};
}
#endif // __cplusplus

#endif // !__FILEAPI_H__