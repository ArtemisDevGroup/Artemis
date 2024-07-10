#include "pch.h"
#include "File.hxx"
#include "Error.hxx"

namespace Artemis::API {
	const std::regex c_AbsolutePathRegex("^[A-Z]:\\\\.*\\..+$");
	const std::regex c_RelativePathRegex("^(?![A-Z]:\\\\).*\\..+$");
	const std::regex c_FetchingRegex("^.*\\\\(.*)\\.(.+)$");

	void safe_win32_handle_deleter::operator()(HANDLE hObject) const {
		CloseHandle(hObject);
	}

	file_info::file_info(std::string _FilePath) {
		__stack_record();

		if (std::regex_match(_FilePath, c_RelativePathRegex)) {
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
		}
		else if (std::regex_match(_FilePath, c_AbsolutePathRegex)) {
			this->_FilePath = _FilePath;
		}
		else throw argument_exception("Path has invalid format.", "_FilePath");

		std::smatch matches;
		assert(std::regex_match(this->_FilePath, matches, c_FetchingRegex));
		
		this->_FileName = matches[0];
		this->_FilePath = matches[1];

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

	const char* const file_info::name() const noexcept { return this->_FileName.c_str(); }

	const char* const file_info::type() const noexcept { return this->_FileType.c_str(); }

	char file_info::drive_letter() const noexcept { return this->_FilePath[0]; }

	const char* const file_info::qualified_path() const noexcept { return this->_FilePath.c_str(); }
}