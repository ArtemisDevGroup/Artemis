#include "pch.h"
#include "File.hxx"
#include "Error.hxx"

namespace Artemis::API {
	void safe_win32_handle_deleter::operator()(HANDLE hObject) const {
		CloseHandle(hObject);
	}

	file_info::file_info(std::string _FilePath) {
		if (_FilePath[1] == ':' && _FilePath[2] == '\\') {
			this->_FilePath = _FilePath;
			return;
		}

		__stack_record();

		CHAR szBuffer[MAX_PATH];

		DWORD dwSize = GetModuleFileNameA(
			nullptr,
			szBuffer,
			sizeof(szBuffer)
		);
		
		if (!dwSize)
			throw win32_exception("K32GetModuleFileNameExA");

		for (dwSize -= 1; dwSize >= 0 && szBuffer[dwSize] != '\\'; dwSize--)
			szBuffer[dwSize] = '\0';

		lstrcatA(szBuffer, _FilePath.c_str());

		this->_FilePath = std::string(szBuffer);

		__stack_escape();
	}

	bool file_info::exists() const {
		__stack_record();

		BOOL bReturn = PathFileExistsA(this->_FilePath.c_str());

		if (!bReturn) {
			DWORD dwLastError = GetLastError();
			if (dwLastError != ERROR_FILE_NOT_FOUND)
				throw win32_exception(dwLastError, "PathFileExistsA");
		}

		__stack_escape();
		return bool(bReturn);
	}

	file file_info::open(access_mode _AccessMode, open_mode _OpenMode) const {
		__stack_record();

		file ret;
		__stack_rethrow(ret = std::move(file(this->_FilePath, _AccessMode, _OpenMode)));

		__stack_escape();
		return ret;
	}

	std::string file_info::name() const {
		__stack_record();



		__stack_escape();
	}
}