#include "pch.h"
#include "Error.hxx"

namespace Artemis::API {
	static ErrorInfo g_LastErrorInfo;

	static LPCSTR ResolveErrorMessage(_In_ ErrorCode dwErrorCode) {
		return nullptr;
	}

	ExtendedError ExtendedError::Create(_In_ DWORD dwErrorCode, _In_z_ LPCSTR lpErrorMessage) {
		ExtendedError ee;
		ee.dwErrorCode = dwErrorCode;
		strcpy_s(ee.szErrorMessage, lpErrorMessage);
		return ee;
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

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode) noexcept {
		strcpy_s(g_LastErrorInfo.szFunction, lpFunction);
		g_LastErrorInfo.dwErrorCode = dwErrorCode;
		strcpy_s(g_LastErrorInfo.szErrorMessage, ResolveErrorMessage(dwErrorCode));

		g_LastErrorInfo.ExtendedErrorType = ExtendedErrorType::None;
	}

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const ExtendedError& CustomExtendedError) {
		strcpy_s(g_LastErrorInfo.szFunction, lpFunction);
		g_LastErrorInfo.dwErrorCode = dwErrorCode;
		strcpy_s(g_LastErrorInfo.szErrorMessage, ResolveErrorMessage(dwErrorCode));

		g_LastErrorInfo.ExtendedErrorType = ExtendedErrorType::Custom;
		g_LastErrorInfo.CustomExtendedError = CustomExtendedError;
	}

	ARTEMIS_API VOID SetLastArtemisError(_In_z_ LPCSTR lpFunction, _In_ ErrorCode dwErrorCode, const WindowsError& WindowsError) {
		strcpy_s(g_LastErrorInfo.szFunction, lpFunction);
		g_LastErrorInfo.dwErrorCode = dwErrorCode;
		strcpy_s(g_LastErrorInfo.szErrorMessage, ResolveErrorMessage(dwErrorCode));

		g_LastErrorInfo.ExtendedErrorType = ExtendedErrorType::Windows;
		g_LastErrorInfo.WindowsExtendedError = WindowsError;
	}

	ARTEMIS_API ErrorInfo GetLastArtemisError() noexcept {
		return g_LastErrorInfo;
	}
}