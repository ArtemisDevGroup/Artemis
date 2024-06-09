#include "pch.h"
#include "Memory.hxx"
#include "Error.hxx"

namespace Artemis::API {
	namespace RT {
		_Success_(return) ARTEMIS_API BOOL MemoryCopy_Unsafe(
			_In_ SIZE_T cb,
			_Out_writes_bytes_(cb) LPVOID lpDestination,
			_In_reads_bytes_(cb) LPCVOID lpSource
		) {
			if (!cb) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull, ParameterError::Create(AA_PEA(cb)));
				return FALSE;
			}

			if (!lpDestination) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull, ParameterError::Create(AA_PEA(lpDestination)));
				return FALSE;
			}

			if (!lpSource) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull, ParameterError::Create(AA_PEA(lpSource)));
				return FALSE;
			}

			LPBYTE lpDst = (LPBYTE)lpDestination;
			LPCBYTE lpSrc = (LPCBYTE)lpSource;

			for (SIZE_T i = 0; i < cb; i++)
				lpDst[i] = lpSrc[i];

			return TRUE;
		}

		_Success_(return) ARTEMIS_API BOOL MemoryCopy_SEH(
			_In_ SIZE_T cb,
			_Out_writes_bytes_(cb) LPVOID lpDestination,
			_In_reads_bytes_(cb) LPCVOID lpSource
		) {
			if (!cb) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull, ParameterError::Create(AA_PEA(cb)));
				return FALSE;
			}

			if (!lpDestination) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull, ParameterError::Create(AA_PEA(lpDestination)));
				return FALSE;
			}

			if (!lpSource) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull, ParameterError::Create(AA_PEA(lpSource)));
				return FALSE;
			}

			LPBYTE lpDst = (LPBYTE)lpDestination;
			LPCBYTE lpSrc = (LPCBYTE)lpSource;

			SIZE_T i = 0;

			__try {
				for (; i < cb; i++)
					lpDst[i] = lpSrc[i];
			}
			__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::MemoryAccessViolation, MemoryAccessViolationError::Create(lpDst + i));
				return FALSE;
			}

			return TRUE;
		}
	}

	namespace Impl {
		_Success_(return) BOOL ReadString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uCount) LPSTR lpReturn,
			_In_ SIZE_T uCount
		) {
			if (!lpfnMemoryCopy(uCount * sizeof(CHAR), lpReturn, (LPCVOID)uAddress))
				return FALSE;

			if (strnlen(lpReturn, uCount) == uCount)
				lpReturn[uCount - 1] = '\0';

			return TRUE;
		}

		_Success_(return) BOOL ReadString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uMaxCount) std::string* const lpReturn,
			_In_ SIZE_T uMaxCount
		) {
			LPSTR lpString = new CHAR[uMaxCount];

			if (!ReadString(lpfnMemoryCopy, uAddress, lpString, uMaxCount)) {
				delete[] lpString;
				return FALSE;
			}

			std::string ret(lpString);
			delete[] lpString;

			lpReturn->swap(ret);
			return TRUE;
		}

		std::string ReadString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_In_ SIZE_T uMaxCount
		) {
			std::string ret;
			if (!ReadString(lpfnMemoryCopy, uAddress, &ret, uMaxCount))
				return FALSE;
			return ret;
		}

		_Success_(return) BOOL ReadWideString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uCount) LPWSTR lpReturn,
			_In_ SIZE_T uCount
		) {
			if (!lpfnMemoryCopy(uCount * sizeof(WCHAR), lpReturn, (LPCVOID)uAddress))
				return FALSE;

			if (wcsnlen(lpReturn, uCount) == uCount)
				lpReturn[uCount - 1] = L'\000';

			return TRUE;
		}

		_Success_(return) BOOL ReadWideString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uMaxCount) std::wstring* const lpReturn,
			_In_ SIZE_T uMaxCount
		) {
			LPWSTR lpString = new WCHAR[uMaxCount];

			if (!ReadWideString(lpfnMemoryCopy, uAddress, lpString, uMaxCount)) {
				delete[] lpString;
				return FALSE;
			}

			std::wstring ret(lpString);
			delete[] lpString;

			lpReturn->swap(ret);
			return TRUE;
		}

		std::wstring ReadWideString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_In_ SIZE_T uMaxCount
		) {
			std::wstring ret;
			if (!ReadWideString(lpfnMemoryCopy, uAddress, &ret, uMaxCount))
				return FALSE;
			return ret;
		}
	}

	namespace SEH {

	}
}