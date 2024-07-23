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

	attribute_collection::attribute_collection(DWORD _Attributes) noexcept : dwAttributes(_Attributes) {}

	attribute_collection::attribute_collection(const std::vector<attribute>& _Attributes) noexcept : attribute_collection(_Attributes.begin(), _Attributes.end()) {}

	bool attribute_collection::is_present(attribute _Attribute) const noexcept { return bool(this->dwAttributes & (DWORD)_Attribute); }

	void attribute_collection::add(attribute _Attribute) noexcept { this->dwAttributes |= (DWORD)_Attribute; }

	void attribute_collection::remove(attribute _Attribute) noexcept { this->dwAttributes ^= (DWORD)_Attribute; }

	file_info::file_info(const std::string_view& _FilePath) {
		__stack_record();

		if (std::regex_match(_FilePath.data(), c_RelativePathRegex)) {
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

			lstrcatA(szBuffer, _FilePath.data());

			this->_FilePath = std::string(szBuffer);
		}
		else if (std::regex_match(_FilePath.data(), c_AbsolutePathRegex)) {
			this->_FilePath = _FilePath;
		}
		else throw argument_exception("Path has invalid format.", "_FilePath");

		std::cmatch matches;
		assert(std::regex_match(this->_FilePath.data(), matches, c_FetchingRegex));
		
		this->_FileName = matches[0].str();
		this->_FilePath = matches[1].str();

		__stack_escape();
	}

	bool file_info::exists() const {
		__stack_record();

		BOOL bReturn = PathFileExistsA(this->_FilePath.data());

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

	const std::string_view& file_info::name() const noexcept { return this->_FileName; }

	const std::string_view& file_info::type() const noexcept { return this->_FileType; }

	char file_info::drive_letter() const noexcept { return this->_FilePath[0]; }

	const std::string_view& file_info::qualified_path() const noexcept { return this->_FilePath; }

	file::file() noexcept : hFile(nullptr), _FilePath("") {}

	file::file(const std::string_view& _FilePath, access_mode _AccessMode, open_mode _OpenMode) : _FilePath(_FilePath) {
		__stack_record();

		HANDLE hFile = CreateFileA(
			this->_FilePath.data(),
			(DWORD)_AccessMode,
			0,
			nullptr,
			(DWORD)_OpenMode,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (hFile == INVALID_HANDLE_VALUE)
			throw win32_exception("CreateFileA");

		this->hFile = safe_win32_handle(hFile);

		__stack_escape();
	}

	void file::read(void* const _Buffer, size_t _Size) const {
		__stack_record();

		if (!ReadFile(
			this->hFile.get(),
			_Buffer,
			_Size,
			nullptr,
			nullptr
		)) throw win32_exception("ReadFile");

		__stack_escape();
	}

	void file::write(const void* const _Buffer, size_t _Size) {
		__stack_record();

		if (!WriteFile(
			this->hFile.get(),
			_Buffer,
			_Size,
			nullptr,
			nullptr
		)) throw win32_exception("WriteFile");

		__stack_escape();
	}

	attribute_collection file::get_attributes() const {
		__stack_record();

		DWORD dwAttributes = GetFileAttributesA(this->_FilePath.data());

		if (dwAttributes == INVALID_FILE_ATTRIBUTES)
			throw win32_exception("GetFileAttributesA");

		__stack_escape();
		return attribute_collection(dwAttributes);
	}

	bool file::has_attribute(attribute _Attribute) const {
		__stack_record();

		attribute_collection attributes(0);
		__stack_rethrow(attributes = this->get_attributes());

		__stack_escape();
		return attributes.is_present(_Attribute);
	}

	void file::set_attributes(attribute_collection _Attributes) {
		__stack_record();

		if (!SetFileAttributesA(
			this->_FilePath.data(),
			_Attributes.dwAttributes
		)) throw win32_exception("SetFileAttributesA");

		__stack_escape();
	}
}