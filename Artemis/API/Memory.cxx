#include "pch.h"
#include "Memory.hxx"
#include "Error.hxx"

namespace Artemis::API {
	namespace RT {
		_Success_(return) ARTEMIS_API BOOL MemoryCopy_Unsafe(_In_ SIZE_T cb, _Out_writes_bytes_(cb) LPVOID lpDestination, _In_reads_bytes_(cb) LPCVOID lpSource) {
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

		_Success_(return) ARTEMIS_API BOOL MemoryCopy_SEH(_In_ SIZE_T cb, _Out_writes_bytes_(cb) LPVOID lpDestination, _In_reads_bytes_(cb) LPCVOID lpSource) {
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

	}

	namespace SEH {

	}
}