#include "pch.h"
#include "Error.hxx"

namespace Artemis::API {
	static ErrorInfo g_LastErrorInfo;

	static LPCSTR ResolveErrorMessage(_In_ ErrorCode dwErrorCode) {
		switch (dwErrorCode) {
		case ErrorCode::Unknown:
			return "An unknown error has occured.";
		case ErrorCode::Success:
			return "An operation was successful.";
		case ErrorCode::ParameterNull:
			return "A parameter is null.";
		case ErrorCode::ParameterInvalid:
			return "A parameter is invalid.";
		case ErrorCode::Windows:
			return "A Windows API function has failed.";
		default:
			return "Invalid error code.";
		}
	}

	ExtendedError ExtendedError::Create(_In_ DWORD dwErrorCode, _In_z_ LPCSTR lpErrorMessage) {
		ExtendedError ee;
		ee.dwErrorCode = dwErrorCode;
		strcpy_s(ee.szErrorMessage, lpErrorMessage);
		return ee;
	}

	ParameterError ParameterError::Create(_In_z_ LPCSTR lpParameterName, _In_ const std::type_info& TypeInfo, _In_ INT nTypeSize, _In_reads_(nTypeSize) LPCVOID lpParameterValue) {
		ParameterError pe;
		strcpy_s(pe.szParameterName, lpParameterName);
		strcpy_s(pe.TypeInfo.szTypeName, TypeInfo.name());
		pe.TypeInfo.nTypeSize = nTypeSize;
		pe.lpParameterValue = std::make_shared<BYTE>(nTypeSize);
		memcpy_s(pe.lpParameterValue.get(), nTypeSize, lpParameterValue, nTypeSize);
		return pe;
	}

	WindowsError WindowsError::Create(_In_z_ LPCSTR lpFunction, _In_ DWORD dwErrorCode, _In_opt_z_ LPCSTR lpAdditionalInformation) {
		WindowsError we;
		strcpy_s(we.szWindowsFunction, lpFunction);
		we.dwWindowsErrorCode = dwErrorCode;

		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			dwErrorCode,
			LANG_SYSTEM_DEFAULT,
			we.szWindowsErrorMessage,
			sizeof(we.szWindowsErrorMessage),
			nullptr
		);

		if (lpAdditionalInformation)
			strcpy_s(we.szAdditionalInformation, lpAdditionalInformation);
		else
			we.szAdditionalInformation[0] = '\0';
		return we;
	}

	MemoryAccessViolationError MemoryAccessViolationError::Create(_In_ LPVOID lpAddress) {
		MemoryAccessViolationError mave;
		mave.lpAddress = lpAddress;
		
		VirtualQuery(lpAddress, &mave.FaultyPageInformation, sizeof(mave.FaultyPageInformation));
		return mave;
	}

	static VOID SetLastArtemisErrorImpl(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode) {
		strcpy_s(g_LastErrorInfo.szFunction, lpFunction);
		g_LastErrorInfo.dwErrorCode = dwErrorCode;
		strcpy_s(g_LastErrorInfo.szErrorMessage, ResolveErrorMessage(dwErrorCode));
	}

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode) {
		SetLastArtemisErrorImpl(lpFunction, dwErrorCode);

		g_LastErrorInfo.ExtendedErrorType = ExtendedErrorType::None;
	}

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const ExtendedError& CustomExtendedError) {
		SetLastArtemisErrorImpl(lpFunction, dwErrorCode);

		g_LastErrorInfo.ExtendedErrorType = ExtendedErrorType::Custom;
		g_LastErrorInfo.ExtendedErrorInfo.Custom = CustomExtendedError;
	}

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const ParameterError& ParameterError) {
		SetLastArtemisErrorImpl(lpFunction, dwErrorCode);

		g_LastErrorInfo.ExtendedErrorType = ExtendedErrorType::Parameter;
		g_LastErrorInfo.ExtendedErrorInfo.Parameter = ParameterError;
	}

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const WindowsError& WindowsError) {
		SetLastArtemisErrorImpl(lpFunction, dwErrorCode);

		g_LastErrorInfo.ExtendedErrorType = ExtendedErrorType::Windows;
		g_LastErrorInfo.ExtendedErrorInfo.Windows = WindowsError;
	}

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const MemoryAccessViolationError& MemoryError) {
		SetLastArtemisErrorImpl(lpFunction, dwErrorCode);

		g_LastErrorInfo.ExtendedErrorType = ExtendedErrorType::Memory;
		g_LastErrorInfo.ExtendedErrorInfo.Memory = MemoryError;
	}

	ARTEMIS_API const ErrorInfo& GetLastArtemisError() noexcept {
		return g_LastErrorInfo;
	}
}