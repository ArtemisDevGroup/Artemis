#include "MemShApi.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

BOOL XECheckVecMask(_In_z_ LPCSTR lpMask, _In_ SIZE_T uCount) {
	CHAR n;
	BOOL bBreak = FALSE;

	do {
		if (strlen(lpMask) != uCount) break;

		for (SIZE_T i = 0; i < uCount; i++) {
			n = lpMask[i] - 0x30;
			if (n > uCount || n < 0) return FALSE;
		}

		return TRUE;
	} while (FALSE);
}

void substr(
	_Out_writes_z_(uSizeOfBuffer) LPSTR lpBuffer,
	_In_ SIZE_T uSizeOfBuffer,
	_In_z_ LPCSTR lpString,
	_In_ SIZE_T uIndex,
	_In_ SIZE_T uCount
) {
	if (uCount == -1) uCount = strlen(lpString) - uIndex;

	strncpy_s(lpBuffer, uSizeOfBuffer, &lpString[uIndex], uCount);
	lpBuffer[uCount] = '\0';
}

#define HEXC_TO_UINT(c) (IN_RANGE(c, 'A', 'F') ? c - '\x37' : (IN_RANGE(c, 'a', 'f') ? c - '\x57' : c - '\x30'));

BOOL XEParsePattern(
	_In_z_ LPCSTR lpComboPattern,
	_Out_writes_(uPatternSize) LPSTR lpPattern,
	_In_ SIZE_T uPatternSize,
	_Out_writes_z_(uMaskSize) LPSTR lpMask,
	_In_ SIZE_T uMaskSize
) {
	// Checking if the pointers for lpPattern or lpMask are nullptrs.
	if (!lpPattern || !lpMask) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	INT vIndex[MAX_PATTERN_LENGTH]; // Creates an array to contain the indexes of the spaces in the IDA-Style/Combo pattern.
	INT vIndexI = 0; // Contains the amount of indexes caught by the for loop.

	INT uStrLen = (INT)strlen(lpComboPattern);
	for (INT i = 0; i < uStrLen; i++) { // Loops through the combo pattern.
		if (lpComboPattern[i] == ' ') { // Stores the indexes of every space character in the string to an array for later use.
			vIndex[vIndexI] = i;
			vIndexI++;
		}
	}

	// Checks if any indexes were found and if they were not, returns an error code indicating that the pattern is invalid.
	if (!vIndexI) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	CHAR vBytes[MAX_PATTERN_LENGTH][3]; // An array to store the bytes in a string representation to later be used to extract the literal representation.
	INT vBytesI = 0; // Indicates the amount of items in the array.
	INT nLastIndex = vIndex[0]; // Stores the index of the last space character to be used in the next iteration.

	// Gets the very first byte of the pattern.
	substr(
		vBytes[0],
		sizeof(vBytes[0]),
		lpComboPattern,
		0, // Index 0 would be the first character of the string which is a part of the first byte.
		vIndex[0] // The first index would be used since indexes are 0 based, and sizes are not. This means that the index of a character is the amount of them before itself.
	);
	vBytesI++;

	for (INT i = 1; i < vIndexI; i++) {
		substr(
			vBytes[i],
			sizeof(vBytes[i]),
			lpComboPattern,
			nLastIndex + 1, // nLastIndex + 1 would be the first character after the previous space character, which would be the first character of the byte.
			vIndex[i] - nLastIndex - 1	// Gets the amount of characters of the current byte by
										// subtracting the index of the following space character
										// by the index of the previous space character and an additional 1
										// since subtracting the indexes gives you a value that only gives you the
										// difference between the indexes and not the amount of characters between them.
		);
		vBytesI++;
		nLastIndex = vIndex[i]; // Stores the current index to be used as the previous index in the next loop iteration.
	}

	substr(vBytes[vBytesI], sizeof(vBytes[vBytesI]), lpComboPattern, nLastIndex + 1, -1); // Gets the very last byte of the pattern.
	vBytesI++;

	// Checks if the buffers provided will be large enough for the operation to succeed.
	if (vBytesI >= uPatternSize || vBytesI >= uMaskSize) return XESetLastError(XE_ERROR_BUFFER_SMALL);

	BYTE b; // Pre-declares a byte for use below.

	for (INT i = 0; i < vBytesI; i++) {
		if (!strcmp(vBytes[i], "??") || (vBytes[i][0] == '?' && vBytes[i][1] == '\0')) { // Checks if the current byte is a wildcard.
			lpPattern[i] = '\xCC'; // Assigns a "default" value to the pattern as a placeholder.
			lpMask[i] = '?'; // Assigns a ? to the mask to indicate a wildcard.
			continue; // Runs next iteration.
		}

		if (vBytes[i][0] == '?' && vBytes[i][1] != '\0') { // Checks if the current byte is a low nibble.
			lpPattern[i] = HEXC_TO_UINT(vBytes[i][1]); // Gets the number as an integer from a character and assigns it to the pattern.
			lpMask[i] = 'l'; // Assigns an l to the mask to indicate a low nibble.
			continue; // Runs next iteration.
		}

		if (vBytes[i][1] == '?' && vBytes[i][0] != '\0') { // Checks if the current byte is a high nibble.
			lpPattern[i] = HEXC_TO_UINT(vBytes[i][0]); // Gets the number as an integer from a character and assigns it to the pattern.
			lpPattern[i] *= 0x10; // Multiplies that value by 0x10 so that the high nibble is in the left position as opposed to the right. (0x -> x0)
			lpMask[i] = 'h'; // Assigns an h to the mask to indicate a low nibble.
			continue; // Runs next iteration.
		}

		if (IS_HEX(vBytes[i][0]) && IS_HEX(vBytes[i][1])) { // Checks if the current byte is a regular byte.
			sscanf_s(vBytes[i], "%hhx", &b); // Converts the 2 char representation of the byte into a literal byte.
			lpPattern[i] = b; // Assigns said byte to the pattern.
			lpMask[i] = 'x'; // Assigns an x to the mask to indicate a regular byte.
		}

		// Returns an error if none of the above if statements resolve to true, indicating that the pattern has failed parsing due to it having invalid characters.
		return XESetLastError(XE_ERROR_PARAMETER_INVALID);
	}

	lpMask[vBytesI] = '\0'; // Null terminates the last byte of the mask.

	return TRUE;
}

BOOL XEIsProtectionValid(_In_ SCAN_PROTECTION nProtection) {
	switch (nProtection) {
	case SP_EXECUTE_READWRITE:
	case SP_EXECUTE_READONLY:
	case SP_EXECUTE_READ:
	case SP_NOEXECUTE_READWRITE:
	case SP_NOEXECUTE_READONLY:
	case SP_NOEXECUTE_READ:
	case SP_READWRITE:
	case SP_READONLY:
	case SP_READ:
		return TRUE;
	default:
		return FALSE;
	}
}
BOOL XEResolveProtection(_In_ SCAN_PROTECTION nDesiredProtection, _In_ DWORD dwCurrentProtection) {
	switch (nDesiredProtection) {
	case SP_EXECUTE_READWRITE:
		if (dwCurrentProtection == PAGE_EXECUTE_READWRITE) return TRUE;
		else return FALSE;
	case SP_EXECUTE_READONLY:
		if (dwCurrentProtection == PAGE_EXECUTE_READ) return TRUE;
		else return FALSE;
	case SP_EXECUTE_READ:
		if (dwCurrentProtection == PAGE_EXECUTE_READ || dwCurrentProtection == PAGE_EXECUTE_READWRITE) return TRUE;
		else return FALSE;
	case SP_NOEXECUTE_READWRITE:
		if (dwCurrentProtection == PAGE_READWRITE) return TRUE;
		else return FALSE;
	case SP_NOEXECUTE_READONLY:
		if (dwCurrentProtection == PAGE_READONLY) return TRUE;
		else return FALSE;
	case SP_NOEXECUTE_READ:
		if (dwCurrentProtection == PAGE_READONLY || dwCurrentProtection == PAGE_READWRITE) return TRUE;
		else return FALSE;
	case SP_READWRITE:
		if (dwCurrentProtection == PAGE_EXECUTE_READWRITE || dwCurrentProtection == PAGE_READWRITE) return TRUE;
		else return FALSE;
	case SP_READONLY:
		if (dwCurrentProtection == PAGE_EXECUTE_READ || dwCurrentProtection == PAGE_READONLY) return TRUE;
		else return FALSE;
	case SP_READ:
		if (dwCurrentProtection == PAGE_EXECUTE_READWRITE ||
			dwCurrentProtection == PAGE_READWRITE ||
			dwCurrentProtection == PAGE_EXECUTE_READ ||
			dwCurrentProtection == PAGE_READONLY) return TRUE;
		else return FALSE;
	default:
		return FALSE;
	}
}