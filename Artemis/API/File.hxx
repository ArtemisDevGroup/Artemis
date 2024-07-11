#ifndef ARTEMIS_API_FILE_HXX
#define ARTEMIS_API_FILE_HXX

#include "Definitions.hxx"

#include <Windows.h>	// DWORD, HANDLE, FILE_GENERIC_READ, FILE_GENERIC_WRITE

#include <memory>		// std::unique_ptr
#include <string>		// std::string
#include <iterator>		// std::random_access_iterator
#include <vector>		// std::vector

namespace Artemis::API {
	struct safe_win32_handle_deleter {
		ARTEMIS_API void operator()(HANDLE hObject) const;
	};

	using safe_win32_handle = std::unique_ptr<void, safe_win32_handle_deleter>;

	class directory_info {

	};

	enum class access_mode : DWORD {
		read = FILE_GENERIC_READ,
		write = FILE_GENERIC_WRITE,
		readwrite = FILE_GENERIC_READ | FILE_GENERIC_WRITE
	};

	enum class open_mode : DWORD{
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

	enum class attribute : DWORD {

	};

	struct attribute_collection {
		DWORD dwAttributes;

		ARTEMIS_API attribute_collection(DWORD _Attributes);
		
		template<std::input_iterator _It>
			requires(std::is_same_v<_It::value_type, attribute>)
		inline attribute_collection(_It _Begin, _It _End) {

		}

		inline attribute_collection(const std::vector<attribute>& _Attributes) {
			
		}

		bool is_present(attribute _Attribute) const;

		void add(attribute _Attribute);
		void remove(attribute _Attribute);
	};

	class file;

	class file_info {
		std::string _FilePath;
		std::string _FileName;
		std::string _FileType;

	public:
		ARTEMIS_API file_info(std::string _FilePath);

		ARTEMIS_API bool exists() const;

		ARTEMIS_API file open(access_mode _AccessMode, open_mode _OpenMode) const;

		ARTEMIS_API const char* const name() const noexcept;
		ARTEMIS_API const char* const type() const noexcept;
		ARTEMIS_API char drive_letter() const noexcept;
		ARTEMIS_API const char* const qualified_path() const noexcept;
	};


	class file {
		safe_win32_handle hFile;

	public:
		ARTEMIS_API file() noexcept;
		ARTEMIS_API file(const std::string& _FilePath, access_mode _AccessMode, open_mode _OpenMode);

		file(const file&) = delete;
		file(file&&) = default;

		file& operator=(const file&) = delete;
		file& operator=(file&&) = default;

		void read() const;
		void write();

		attribute_collection get_attributes() const;
		bool has_attribute(attribute _Attribute) const;
		void set_attributes(attribute_collection _Attributes);
	};
}

#endif // !ARTEMIS_API_FILE_HXX