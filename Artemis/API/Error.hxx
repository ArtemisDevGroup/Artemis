#ifndef ARTEMIS_API_ERROR_HXX
#define ARTEMIS_API_ERROR_HXX

#include "Definitions.hxx"

#include <Windows.h>

#ifndef MAX_ERROR_MESSAGE
#define MAX_ERROR_MESSAGE 256
#endif // !MAX_ERROR_MESSAGE

#ifndef MAX_FUNCTION_NAME
#define MAX_FUNCTION_NAME 64
#endif // !MAX_FUNCTION_NAME

namespace Artemis::API {
	enum class ErrorCode : DWORD {

	};

	enum class ExtendedErrorType {
		None,
		Custom,
		Windows,
		Errno
	};

	struct ARTEMIS_API ExtendedError {
		DWORD dwErrorCode;
		CHAR szErrorMessage[MAX_ERROR_MESSAGE];

		static ExtendedError Create(_In_ DWORD dwErrorCode, _In_z_ LPCSTR lpErrorMessage);
	};

	struct ARTEMIS_API WindowsError {
		CHAR szWindowsFunction[MAX_FUNCTION_NAME];
		DWORD dwWindowsErrorCode;
		CHAR szWindowsErrorMessage[MAX_ERROR_MESSAGE];
		CHAR szAdditionalInformation[MAX_ERROR_MESSAGE];

		static WindowsError Create(_In_z_ LPCSTR lpFunction, _In_ DWORD dwErrorCode, _In_opt_z_ LPCSTR lpAdditionalInformation);
	};
	
	struct ErrorInfo {
		ErrorCode dwErrorCode;
		CHAR szErrorMessage[MAX_ERROR_MESSAGE];
		CHAR szFunction[MAX_FUNCTION_NAME];

		ExtendedErrorType ExtendedErrorType;
		union {
			ExtendedError CustomExtendedError;
			WindowsError WindowsExtendedError;
		};
	};

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode) noexcept;

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const ExtendedError& CustomExtendedError);

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const WindowsError& WindowsError);

	ARTEMIS_API ErrorInfo GetLastArtemisError() noexcept;
}

#endif // !ARTEMIS_API_ERROR_HXX