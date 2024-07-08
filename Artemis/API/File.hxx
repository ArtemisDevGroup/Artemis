#ifndef ARTEMIS_API_FILE_HXX
#define ARTEMIS_API_FILE_HXX

#include "Definitions.hxx"

#include <Windows.h>	// DWORD, HANDLE
#include <memory>		// std::unique_ptr

namespace Artemis::API {
	struct safe_win32_handle_deleter {
		void operator()(HANDLE hObject) const;
	};

	using safe_win32_handle = std::unique_ptr<void, safe_win32_handle_deleter>;

	class directory_info {

	};

	enum class access_mode {
		read = FILE_GENERIC_READ,
		write = FILE_GENERIC_WRITE,
		readwrite = FILE_GENERIC_READ | FILE_GENERIC_WRITE
	};

	enum class open_mode {
		/// <summary>
		/// <para>Creates a new file, only if it does not already exist.</para>
		/// <para>
		/// - If the specified file exists, the function fails.
		/// </para>
		/// <para>
		/// - If the specified file does not exist and
		/// is a valid path to a writable location, a new file is created.
		/// </para>
		/// </summary>
		create_new = 1,

		/// <summary>
		/// <para>Creates a new file, always.</para>
		/// <para>
		/// - If the specified file exists and is writable,
		///   the file is truncated (has all contents erased).
		/// </para>
		/// <para>
		/// - If the specified file does not exist and is a valid path,
		///   a new file is created.
		/// </para>
		/// </summary>
		create_always = 2,

		/// <summary>
		/// <para>Opens a file or device, only if it exists.</para>
		/// <para>
		/// - If the specified file does not exist,
		///   the function fails.
		/// </para>
		/// <para>
		/// - If the specified file exists, the function succeeds.
		/// </para>
		/// </summary>
		open_existing = 3,

		/// <summary>
		/// <para>Opens a file, always.</para>
		/// <para>
		/// - If the specified file exists, the function succeeds.
		/// </para>
		/// <para>
		/// - If the specified file does not exist and is a valid path
		///   to a writable location, the function creates a file.
		/// </para>
		/// </summary>
		open_always = 4,

		/// <summary>
		/// <para>
		/// Opens a file and truncates it so that its size
		/// is zero bytes, only if it exists.
		/// </para>
		/// <para>
		/// - If the specified file does not exist, the function fails.
		/// </para>
		/// <para>
		/// - If the specified file exists and the file is opened
		///   with the write permission, the file is truncated and
		///   the function succeeds.
		/// </para>
		/// </summary>
		truncate_existing = 5
	};

	class file;

	class file_info {
		std::string _FilePath;

	public:
		file_info(std::string _FilePath);

		bool exists() const;

		file open(access_mode _AccessMode, open_mode _OpenMode) const;

		std::string name() const;
		std::string type() const;
		std::string path() const;
	};

	class file {
		safe_win32_handle hFile;

		DWORD dwFileSize;

	public:
		file();
		file(std::string _FilePath, access_mode _AccessMode, open_mode _OpenMode);
		file(const file&) = delete;
		file(file&&) = default;

		file& operator=(const file&) = delete;
		file& operator=(file&&) = default;
	};
}

#endif // !ARTEMIS_API_FILE_HXX