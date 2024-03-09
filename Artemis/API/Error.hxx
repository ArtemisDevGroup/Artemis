#ifndef ARTEMIS_API_ERROR_HXX
#define ARTEMIS_API_ERROR_HXX

#include "Definitions.hxx"

#include <Windows.h>

#include <typeinfo>
#include <memory>

#ifndef MAX_ERROR_MESSAGE
#define MAX_ERROR_MESSAGE 256
#endif // !MAX_ERROR_MESSAGE

#ifndef MAX_FUNCTION_NAME
#define MAX_FUNCTION_NAME 128
#endif // !MAX_FUNCTION_NAME

namespace Artemis::API {
	enum class ErrorCode : DWORD {
		Unknown = 0xFFFFFFFF,
		Success,
		ParameterNull,
		ParameterInvalid,
		Windows,
		MemoryAccessViolation
	};

	enum class ExtendedErrorType {
		None,
		Custom,
		Parameter,
		Windows,
		Memory,
		Errno
	};

	struct ARTEMIS_API ExtendedError {
		DWORD dwErrorCode;
		CHAR szErrorMessage[MAX_ERROR_MESSAGE];

		static ExtendedError Create(_In_ DWORD dwErrorCode, _In_z_ LPCSTR lpErrorMessage);
	};

#define ARTEMIS_API_PARAMETER_ERROR_ARGS(x) #x, typeid(x), sizeof(x), &x
#define AA_PEA(x) ARTEMIS_API_PARAMETER_ERROR_ARGS(x)

	struct ARTEMIS_API ParameterError {
		CHAR szParameterName[64];
		struct {
			CHAR szTypeName[64];
			INT nTypeSize;
		} TypeInfo;
		std::shared_ptr<BYTE> lpParameterValue;

		static ParameterError Create(_In_z_ LPCSTR lpParameterName, _In_ const std::type_info& TypeInfo, _In_ INT nTypeSize, _In_reads_(nTypeSize) LPCVOID lpParameterValue);

		template<typename T>
		static ParameterError Create(_In_z_ LPCSTR lpParameterName, _In_ const T& Parameter) {
			Create(lpParameterName, typeid(T), sizeof(T), &Parameter);
		}
	};

	struct ARTEMIS_API WindowsError {
		CHAR szWindowsFunction[MAX_FUNCTION_NAME];
		DWORD dwWindowsErrorCode;
		CHAR szWindowsErrorMessage[MAX_ERROR_MESSAGE];
		CHAR szAdditionalInformation[MAX_ERROR_MESSAGE];

		static WindowsError Create(_In_z_ LPCSTR lpFunction, _In_ DWORD dwErrorCode, _In_opt_z_ LPCSTR lpAdditionalInformation);
	};

	struct ARTEMIS_API MemoryAccessViolationError {
		LPVOID lpAddress;
		MEMORY_BASIC_INFORMATION FaultyPageInformation;

		static MemoryAccessViolationError Create(_In_ LPVOID lpAddress);
	};
	
	struct ErrorInfo {
		ErrorCode dwErrorCode;
		CHAR szErrorMessage[MAX_ERROR_MESSAGE];
		CHAR szFunction[MAX_FUNCTION_NAME];

		ExtendedErrorType ExtendedErrorType;
		struct {
			union {
				ExtendedError Custom;
				WindowsError Windows;
				MemoryAccessViolationError Memory;
			};
			ParameterError Parameter; // Non-trivial, cannot be part of the union.
		} ExtendedErrorInfo;
	};

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode);

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const ExtendedError& CustomExtendedError);

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const ParameterError& ParameterError);

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const WindowsError& WindowsError);

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const MemoryAccessViolationError& MemoryError);

	ARTEMIS_API const ErrorInfo& GetLastArtemisError() noexcept;
}

#endif // !ARTEMIS_API_ERROR_HXX