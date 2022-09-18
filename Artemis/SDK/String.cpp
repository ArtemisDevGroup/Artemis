#include "String.h"
#include "SafeMemory.h"

void String::Alloc(DWORD dwCount) {
	DWORD dwPageCount = (dwCount / 64) + (dwCount % 64 > 0 ? 1 : 0);
	if (dwAllocSize / 64 >= dwPageCount) return;

	LPSTR lpOld = lpStr;
	DWORD dwOldSize = dwAllocSize;

	dwAllocSize = dwPageCount * 64;
	lpStr = (LPSTR)calloc(dwAllocSize, sizeof(CHAR));

	if (!lpStr) throw api::MemoryFullException();

	if (lpOld) {
		strncpy_s(lpStr, dwAllocSize, lpOld, dwOldSize);
		free(lpOld);
	}
}

String::String() : lpStr(nullptr), dwAllocSize(NULL) {}

String::String(const String& str) {
	DWORD dwCount = str.Size() + 1;
	DWORD dwPageCount = (dwCount / 64) + (dwCount % 64 > 0 ? 1 : 0);
	dwAllocSize = dwPageCount * 64;

	lpStr = (LPSTR)calloc(dwAllocSize, sizeof(CHAR));
	if (!lpStr) throw api::MemoryFullException();

	strcpy_s(lpStr, dwAllocSize, str.CStr());
}

String::String(_In_ std::string str) {
	Alloc(str.size() + 1);
	strcpy_s(lpStr, dwAllocSize, str.c_str());
}

String::String(_In_z_ LPCSTR lpString) {
	Alloc(strlen(lpString) + 1);
	strcpy_s(lpStr, dwAllocSize, lpString);
}

String::~String() { free(lpStr); }

LPCSTR String::CStr() const { return lpStr; }

std::string String::StdStr() const { 
	std::string ret = "";
	for (INT i = 0; i < Size(); i++) {
		ret += lpStr[i];
	}

	return ret;
}

SIZE_T String::Size() const {
	return lpStr ? strlen(lpStr) : 0;
}

void String::Clear() { free(lpStr); }

String String::SubString(_In_ INT nIndex, _In_ INT nCount) {
	if (nCount + nIndex > Size()) throw api::ParameterInvalidException("nIndex && nCount");
	
	LPSTR buf = (LPSTR)calloc(nCount + 1, sizeof(CHAR));
	if (!buf) throw api::MemoryFullException();

	strncpy_s(buf, nCount + 1, &lpStr[nIndex], nCount);
	buf[nCount] = '\0';

	String ret(buf);

	free(buf);
	return ret;
}

void String::Insert(_In_ INT nIndex, _In_ CHAR c) {
	Alloc(Size() + 1 + 1);
	if (Size() <= nIndex || nIndex <= 0) throw api::ParameterInvalidException("nIndex");

	for (DWORD i = Size() + 1; i > nIndex; i--) {
		lpStr[i] = lpStr[i - 1];
	}

	lpStr[nIndex] = c;
}

void String::Insert(_In_ INT nIndex, _In_z_ LPCSTR lpString) {
	SIZE_T nSize = strlen(lpString);
	Alloc(Size() + nSize + 1);

	for (DWORD i = Size() + nSize; i > nIndex; i--) {
		lpStr[i] = lpStr[i - nSize];
	}

	for (INT i = 0; i < nSize; i++) {
		lpStr[nIndex + i] = lpString[i];
	}
}

void String::Insert(_In_ INT nIndex, _In_ String str) {
	SIZE_T nSize = str.Size();
	Alloc(Size() + nSize + 1);

	for (DWORD i = Size() + nSize; i > nIndex; i--) {
		lpStr[i] = lpStr[i - nSize];
	}

	for (INT i = 0; i < nSize; i++) {
		lpStr[nIndex + i] = str[i];
	}
}

void String::Remove(_In_ INT nIndex, _In_ INT nCount) {
	if (Size() - nIndex <= nCount) throw api::ParameterInvalidException("nCount");

	strcpy_s(&lpStr[nIndex], dwAllocSize - nIndex - 1, &lpStr[nIndex + nCount]);
}

INT String::Find(_In_ CHAR c) {
	for (SIZE_T i = 0; i < Size(); i++) {
		if (lpStr[i] == c) return i;
	}
	return -1;
}

INT String::Find(_In_z_ LPCSTR lpString) {
	SIZE_T nSize = Size();

	BOOL bFound = TRUE;
	for (SIZE_T i = 0; i < nSize; i++) {
		bFound = TRUE;

		for (SIZE_T j = 0; j < strlen(lpString); j++) {
			if (i + j > nSize) return -1;

			if (lpStr[i + j] != lpString[j]) {
				bFound = FALSE;
				break;
			}
		}

		if (bFound) return i;
	}

	return -1;
}

INT String::Find(_In_ String str) {
	SIZE_T nSize = Size();

	BOOL bFound = TRUE;
	for (SIZE_T i = 0; i < nSize; i++) {
		bFound = TRUE;

		for (SIZE_T j = 0; j < str.Size(); j++) {
			if (i + j > nSize) return -1;

			if (lpStr[i + j] != str[j]) {
				bFound = FALSE;
				break;
			}
		}

		if (bFound) return i;
	}

	return -1;
}

CHAR& String::operator[](_In_ INT nIndex) {
	if (nIndex < 0 || nIndex > dwAllocSize - 1) throw api::IndexOutOfRangeException(nIndex, dwAllocSize - 1);
	return lpStr[nIndex];
}

void WString::Alloc(DWORD dwCount) {
	DWORD dwPageCount = (dwCount * 2 / 64) + (dwCount * 2 % 64 > 0 ? 1 : 0);
	if (dwAllocSize / 64 >= dwPageCount) return;

	LPWSTR lpOld = lpStr;
	DWORD dwOldSize = dwAllocSize;

	dwAllocSize = dwPageCount * 64;
	lpStr = (LPWSTR)calloc(dwAllocSize, sizeof(WCHAR));

	if (!lpStr) throw api::MemoryFullException();

	if (lpOld) {
		wcsncpy_s(lpStr, dwAllocSize / 2, lpOld, dwOldSize);
		free(lpOld);
	}
}

WString::WString() : lpStr(nullptr), dwAllocSize(NULL) {}

WString::WString(const WString& str) {
	DWORD dwCount = str.Size() + 1;
	DWORD dwPageCount = (dwCount * 2 / 64) + (dwCount * 2 % 64 > 0 ? 1 : 0);
	dwAllocSize = dwPageCount * 64;

	lpStr = (LPWSTR)calloc(dwAllocSize, sizeof(CHAR));
	if (!lpStr) throw api::MemoryFullException();

	wcscpy_s(lpStr, dwAllocSize / 2, str.CStr());
}

WString::WString(_In_ std::wstring str) {
	Alloc(str.size() + 1);
	wcscpy_s(lpStr, dwAllocSize, str.c_str());
}

WString::WString(_In_z_ LPCWSTR lpString) {
	Alloc(wcslen(lpString) + 1);
	wcscpy_s(lpStr, dwAllocSize, lpString);
}

WString::~WString() { free(lpStr); }

LPCWSTR WString::CStr() const { return lpStr; }

std::wstring WString::StdStr() const {
	std::wstring ret = L"";
	for (INT i = 0; i < Size(); i++) {
		ret += lpStr[i];
	}

	return ret;
}

SIZE_T WString::Size() const {
	return lpStr ? wcslen(lpStr) : 0;
}

void WString::Clear() { free(lpStr); }

WString WString::SubString(_In_ INT nIndex, _In_ INT nCount) {
	if (nCount + nIndex > Size()) throw api::ParameterInvalidException("nIndex && nCount");

	LPWSTR buf = (LPWSTR)calloc(nCount + 1, sizeof(WCHAR));
	if (!buf) throw api::MemoryFullException();

	wcsncpy_s(buf, nCount + 1, &lpStr[nIndex], nCount);
	buf[nCount] = '\0';

	WString ret(buf);

	free(buf);
	return ret;
}

void WString::Insert(_In_ INT nIndex, _In_ WCHAR wc) {
	Alloc(Size() + 1 + 1);
	if (Size() <= nIndex || nIndex <= 0) throw api::ParameterInvalidException("nIndex");

	for (DWORD i = Size() + 1; i > nIndex; i--) {
		lpStr[i] = lpStr[i - 1];
	}

	lpStr[nIndex] = wc;
}

void WString::Insert(_In_ INT nIndex, _In_z_ LPCWSTR lpString) {
	SIZE_T nSize = wcslen(lpString);
	Alloc(Size() + nSize + 1);

	for (DWORD i = Size() + nSize; i > nIndex; i--) {
		lpStr[i] = lpStr[i - nSize];
	}

	for (INT i = 0; i < nSize; i++) {
		lpStr[nIndex + i] = lpString[i];
	}
}

void WString::Insert(_In_ INT nIndex, _In_ WString str) {
	SIZE_T nSize = str.Size();
	Alloc(Size() + nSize + 1);

	for (DWORD i = Size() + nSize; i > nIndex; i--) {
		lpStr[i] = lpStr[i - nSize];
	}

	for (INT i = 0; i < nSize; i++) {
		lpStr[nIndex + i] = str[i];
	}
}

void WString::Remove(_In_ INT nIndex, _In_ INT nCount) {
	if (Size() - nIndex <= nCount) throw api::ParameterInvalidException("nCount");

	wcscpy_s(&lpStr[nIndex], ((dwAllocSize / 2) - nIndex - 1) * 2, &lpStr[nIndex + nCount]);
}

INT WString::Find(_In_ WCHAR c) {
	for (SIZE_T i = 0; i < Size(); i++) {
		if (lpStr[i] == c) return i;
	}
	return -1;
}

INT WString::Find(_In_z_ LPCWSTR lpString) {
	SIZE_T nSize = Size();

	BOOL bFound = TRUE;
	for (SIZE_T i = 0; i < nSize; i++) {
		bFound = TRUE;

		for (SIZE_T j = 0; j < wcslen(lpString); j++) {
			if (i + j > nSize) return -1;

			if (lpStr[i + j] != lpString[j]) {
				bFound = FALSE;
				break;
			}
		}

		if (bFound) return i;
	}

	return -1;
}

INT WString::Find(_In_ WString str) {
	SIZE_T nSize = Size();

	BOOL bFound = TRUE;
	for (SIZE_T i = 0; i < nSize; i++) {
		bFound = TRUE;

		for (SIZE_T j = 0; j < str.Size(); j++) {
			if (i + j > nSize) return -1;

			if (lpStr[i + j] != str[j]) {
				bFound = FALSE;
				break;
			}
		}

		if (bFound) return i;
	}

	return -1;
}

WCHAR& WString::operator[](_In_ INT nIndex) {
	if (nIndex < 0 || nIndex > dwAllocSize - 1) throw api::IndexOutOfRangeException(nIndex, dwAllocSize - 1);
	return lpStr[nIndex];
}