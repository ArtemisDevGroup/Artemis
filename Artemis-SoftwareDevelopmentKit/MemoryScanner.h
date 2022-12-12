//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __MEMORY_SCANNER_H__
#define __MEMORY_SCANNER_H__

#include "Definitions.h"
#include "MemoryDefinitions.h"
#include "Array.h"

namespace Artemis {
	/// <summary>
	/// A class for scanning memory for different kinds of data.
	/// </summary>
	class ARTEMIS_API MemoryScanner {
	public:
		/// <summary>
		/// Represents which protection level the memory to be scanned should have.
		/// </summary>
		enum ScanProtection {
			Read,						// Scans all readable memory.
			ReadWrite,					// Scans all read and writable memory.
			ReadOnly,					// Scans all readonly memory.
			Execute_Read,				// Scans all readable executable memory.
			Execute_ReadWrite,			// Scans all read and writable executable memory.
			Execute_ReadOnly,			// Scans all readonly executable memory.
			NoExecute_Read,				// Scans all readable non-executable memory.
			NoExecute_ReadWrite,		// Scans all read and writable non-executable memory.
			NoExecute_ReadOnly			// Scans all readonly non-executable memory.
		};
	private:
		HANDLE hThread;

		enum ScanType { Invalid, CodePattern, IdaPattern, StringA, StringW };

		typedef struct _SCAN_PARAMS {
			LPADDRESS lpAlloc;
			DWORD dwResultCount;
			HANDLE hProcess;

			ScanType Type;

			DWORD dwMaxResultCount;
			ADDRESS uStartAddress;
			SIZE_T uScanSize;
			DWORD dwAlignment;
			ScanProtection Protection;

			union {
				struct {
					CHAR szPattern[MAX_PATTERN_LENGTH];
					CHAR szMask[MAX_PATTERN_LENGTH];
				} CodePattern;
				struct {
					CHAR szPattern[MAX_STRING_LENGTH];
				} IdaPattern;
				struct {
					CHAR szString[MAX_STRING_LENGTH];
					BOOL bCaseSensitive;
				} StringA;
				struct {
					WCHAR szString[MAX_STRING_LENGTH];
					BOOL bCaseSensitive;
				} StringW;
			};
		} SCAN_PARAMS, * LPSCAN_PARAMS;

		SCAN_PARAMS params;

		static DWORD APIENTRY StartScan(LPSCAN_PARAMS);

	public:
		MemoryScanner(); // Null constructor.
		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <exception cref="Exception"/>
		MemoryScanner(const MemoryScanner&);
		~MemoryScanner(); // Destructor.

		/// <summary>
		/// Constructs a memory scanner object scanning for a code style pattern.
		/// </summary>
		/// <param name="lpPattern">- A pointer to the pattern.</param>
		/// <param name="lpMask">- A pointer to the mask. Must be null-terminated.</param>
		/// <exception cref="ParameterException"/>
		MemoryScanner(_In_ LPCSTR lpPattern, _In_z_ LPCSTR lpMask);

		/// <summary>
		/// Constructs a memory scanner object scanning for an IDA style pattern.
		/// </summary>
		/// <param name="lpPattern">- A pointer to the pattern. Must be null-terminated.</param>
		/// <exception cref="ParameterException"/>
		MemoryScanner(_In_z_ LPCSTR lpPattern);

		/// <summary>
		/// Constructs a memory scanner object scanning for a string.
		/// </summary>
		/// <param name="lpString">- The string to scan for.</param>
		/// <param name="bCaseSensitive">- A bool that determines wether to do a case sensitive scan or not.</param>
		/// <exception cref="ParameterException"/>
		MemoryScanner(_In_z_ LPCSTR lpString, _In_ BOOL bCaseSensitive);

		/// <summary>
		/// Constructs a memory scanner scanning for a string.
		/// </summary>
		/// <param name="lpString">- The string to scan for.</param>
		/// <param name="bCaseSensitive">- A bool that determines wether to do a case sensitive scan or not.</param>
		/// <exception cref="ParameterException"/>
		MemoryScanner(_In_z_ LPCWSTR lpString, _In_ BOOL bCaseSensitive);

		/// <summary>
		/// Sets the handle to use in the event that an external process' virtual memory is being scanned.
		/// </summary>
		/// <param name="hProcess">- A handle to the process.</param>
		void SetExternalHandle(_In_ HANDLE hProcess);

		/// <summary>
		/// Sets the maximum number of results to scan for.
		/// </summary>
		/// <param name="dwMaxResultCount">- The number of results to get.</param>
		void SetMaxResultCount(_In_ DWORD dwMaxResultCount = 1);

		/// <summary>
		/// Sets the address where the scanner starts scanning memory.
		/// </summary>
		/// <param name="uStartAddress">- The start address.</param>
		void SetStartAddress(_In_ ADDRESS uStartAddress = MIN_ADDRESS);

		/// <summary>
		/// Sets the number of bytes to scan before stopping.
		/// </summary>
		/// <param name="uScanSize">- The number of bytes to scan.</param>
		void SetScanSize(_In_ SIZE_T uScanSize = MAX_ADDRESS);

		/// <summary>
		/// Sets the scan alignment. (The number of bytes to jump every scan iteration.)
		/// </summary>
		/// <param name="dwAlignment">- The scan alignment.</param>
		void SetAlignment(_In_ DWORD dwAlignment = 1);

		/// <summary>
		/// Sets the protection level of the memory to scan. For example, if you want to only scan readonly memory, you can specify that here.
		/// </summary>
		/// <param name="Protection">- A member from the ScanProtection enum indicating the protection level.</param>
		void SetScanProtection(_In_ ScanProtection Protection = Read);

		/// <summary>
		/// Starts the scan in a new thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Start();

		/// <summary>
		/// Waits for the scan thread to finish.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Wait() const;

		/// <summary>
		/// Gets a list of the found addresses.
		/// </summary>
		/// <returns>The address list.</returns>
		_Check_return_ List<ADDRESS> GetResults() const;

		/// <summary>
		/// Gets the thread exit code.
		/// </summary>
		/// <returns>The thread exit code.</returns>
		/// <exception cref="WindowsApiException"/>
		_Check_return_ DWORD GetExitCode() const;
	};
}

#endif // !__MEMORY_SCANNER_H__