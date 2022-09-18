#include "MemApi_Scan.h"
#include "MemApi_Virtual.h"

BOOL XEScanCodePattern(
	_In_ LPCSTR lpPattern,
	_In_z_ LPCSTR lpMask,
	_In_ ADDRESS uStartAddress,
	_In_ SIZE_T uScanSize,
	_In_ DWORD dwAlignment,
	_In_ SCAN_PROTECTION nScanProtection,
	_Out_ LPADDRESS lpReturn
) {
	if (!lpReturn) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!XEIsProtectionValid(nScanProtection)) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	BOOL b;
	CHAR c;
	SIZE_T uPatternLen = strlen(lpMask);

	MBI mbi = { NULL };
	mbi.RegionSize = 4096;

	for (ADDRESS uCurrentAddress = uStartAddress; uCurrentAddress < uStartAddress + uScanSize; uCurrentAddress += mbi.RegionSize) {
		if (!XEVirtualQuery(uCurrentAddress, &mbi)) continue;
		if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS) continue;
		if (!XEResolveProtection(nScanProtection, mbi.Protect)) continue;

		for (SIZE_T i = 0; i < mbi.RegionSize; i += dwAlignment) {
			b = TRUE;

			for (SIZE_T j = 0; j < uPatternLen; j++) {
				if (i + j >= mbi.RegionSize) {
					b = FALSE;
					break;
				}

				c = *(CHAR*)(uCurrentAddress + i + j);

				switch (lpMask[j]) {
				case '?': continue;
				case 'x':
					if (lpPattern[j] != c) break;
					else continue;
				case 'l':
					if (LO_NIBBLE(lpPattern[j]) != LO_NIBBLE(c)) break;
					else continue;
				case 'h':
					if (HI_NIBBLE(lpPattern[j]) != HI_NIBBLE(c)) break;
					else continue;
				}

				b = FALSE;
				break;
			}

			if (b) {
				*lpReturn = (uCurrentAddress + i);
				return XEResetLastError();
			}
		}
	}
	return XESetLastError(XE_ERROR_SCAN_NOT_FOUND);
}

BOOL XEScanIdaPattern(
	_In_z_ LPCSTR lpPattern,
	_In_ ADDRESS uStartAddress,
	_In_ SIZE_T uScanSize,
	_In_ DWORD dwAlignment,
	_In_ SCAN_PROTECTION nScanProtection,
	_Out_ LPADDRESS lpReturn
) {
	VIRTUAL_ALLOC v1, v2;
	if (!XEVirtualAlloc(
		&v1,
		0,
		MAX_PATTERN_LENGTH,
		AT_COMMIT_RESERVE | AT_TOPDOWN, MP_READWRITE
	)) return FALSE;

	if (!XEVirtualAlloc(
		&v2,
		0,
		MAX_PATTERN_LENGTH,
		AT_COMMIT_RESERVE | AT_TOPDOWN, MP_READWRITE
	)) return FALSE;

	if (!XEParsePattern(
		lpPattern,
		(LPSTR)v1.uAllocatedAddress,
		v1.dwAllocatedSize,
		(LPSTR)v2.uAllocatedAddress,
		v2.dwAllocatedSize
	)) return FALSE;

	BOOL bResult = XEScanCodePattern(
		(LPCSTR)v1.uAllocatedAddress,
		(LPCSTR)v2.uAllocatedAddress,
		uStartAddress,
		uScanSize,
		dwAlignment,
		nScanProtection,
		lpReturn
	);

	XEVirtualFree(&v1, FT_RELEASE);
	XEVirtualFree(&v2, FT_RELEASE);

	return bResult;
}

BOOL XEScanStringA(
	_In_z_ LPCSTR lpString,
	_In_ BOOL bCaseSensitive,
	_In_ ADDRESS uStartAddress,
	_In_ SIZE_T uScanSize,
	_In_ DWORD dwAlignment,
	_In_ SCAN_PROTECTION nScanProtection,
	_Out_ LPADDRESS lpReturn
) {
	if (!lpReturn) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!XEIsProtectionValid(nScanProtection)) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	BOOL b;
	SIZE_T uStrLen = strlen(lpString);

	MBI mbi = { NULL };
	mbi.RegionSize = 4096;

	for (ADDRESS uCurrentAddress = uStartAddress; uCurrentAddress < uStartAddress + uScanSize; uCurrentAddress += mbi.RegionSize) {
		if (!XEVirtualQuery(uCurrentAddress, &mbi)) continue;
		if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS) continue;
		if (!XEResolveProtection(nScanProtection, mbi.Protect)) continue;

		CHAR c;

		for (SIZE_T i = 0; i < mbi.RegionSize; i += dwAlignment) {
			b = TRUE;

			if (bCaseSensitive) {
				for (SIZE_T j = 0; j < uStrLen; j++) {
					if (i + j >= mbi.RegionSize) {
						b = FALSE;
						break;
					}

					if (lpString[j] != *(CHAR*)(uCurrentAddress + i + j)) {
						b = FALSE;
						break;
					}
				}
			}
			else {
				for (SIZE_T j = 0; j < uStrLen; j++) {
					if (i + j >= mbi.RegionSize) {
						b = FALSE;
						break;
					}

					c = *(CHAR*)(uCurrentAddress + i + j);

					if (IS_LOWERCASE(lpString[j])) {
						if (lpString[j] != c && lpString[j] != TO_UPPERCASE(c)) {
							b = FALSE;
							break;
						}
					}
					else if (IS_UPPERCASE(lpString[j])) {
						if (lpString[j] != c && lpString[j] != TO_LOWERCASE(c)) {
							b = FALSE;
							break;
						}
					}
					else {
						if (lpString[j] != c) {
							b = FALSE;
							break;
						}
					}
				}
			}

			if (b) {
				*lpReturn = (uCurrentAddress + i);
				return XEResetLastError();
			}
		}
	}
	return XESetLastError(XE_ERROR_SCAN_NOT_FOUND);
}

BOOL XEScanStringW(
	_In_z_ LPCWSTR lpString,
	_In_ BOOL bCaseSensitive,
	_In_ ADDRESS uStartAddress,
	_In_ SIZE_T uScanSize,
	_In_ DWORD dwAlignment,
	_In_ SCAN_PROTECTION nScanProtection,
	_Out_ LPADDRESS lpReturn
) {
	if (!lpReturn) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!XEIsProtectionValid(nScanProtection)) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	BOOL b;
	SIZE_T uStrLen = lstrlenW(lpString);

	MBI mbi = { NULL };
	mbi.RegionSize = 4096;

	for (ADDRESS uCurrentAddress = uStartAddress; uCurrentAddress < uStartAddress + uScanSize; uCurrentAddress += mbi.RegionSize) {
		if (!XEVirtualQuery(uCurrentAddress, &mbi)) continue;
		if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS) continue;
		if (!XEResolveProtection(nScanProtection, mbi.Protect)) continue;

		WCHAR c;

		for (SIZE_T i = 0; i < mbi.RegionSize; i += dwAlignment) {
			b = TRUE;

			if (bCaseSensitive) {
				for (SIZE_T j = 0; j < uStrLen; j++) {
					if (i + j >= mbi.RegionSize) {
						b = FALSE;
						break;
					}

					if (lpString[j] != *(WCHAR*)(uCurrentAddress + i + j)) {
						b = FALSE;
						break;
					}
				}
			}
			else {
				for (SIZE_T j = 0; j < uStrLen; j++) {
					if (i + j >= mbi.RegionSize) {
						b = FALSE;
						break;
					}

					c = *(WCHAR*)(uCurrentAddress + i + j);

					if (IS_LOWERCASE(lpString[j])) {
						if (lpString[j] != c && lpString[j] != TO_UPPERCASE(c)) {
							b = FALSE;
							break;
						}
					}
					else if (IS_UPPERCASE(lpString[j])) {
						if (lpString[j] != c && lpString[j] != TO_LOWERCASE(c)) {
							b = FALSE;
							break;
						}
					}
					else {
						if (lpString[j] != c) {
							b = FALSE;
							break;
						}
					}
				}
			}

			if (b) {
				*lpReturn = (uCurrentAddress + i);
				return XEResetLastError();
			}
		}
	}
	return XESetLastError(XE_ERROR_SCAN_NOT_FOUND);
}