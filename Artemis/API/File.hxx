#ifndef ARTEMIS_API_FILE_HXX
#define ARTEMIS_API_FILE_HXX

#include "Definitions.hxx"

#include <Windows.h>	// DWORD, HANDLE, FILE_GENERIC_READ, FILE_GENERIC_WRITE

#include <memory>		// std::unique_ptr
#include <string>		// std::string
#include <string_view>	// std::string_view
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

	enum class open_mode : DWORD {
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

	// https://learn.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
	enum class attribute : DWORD {
		readonly				= 1 << 0,
		hidden					= 1 << 1,
		system					= 1 << 2,
		directory				= 1 << 4,
		archive					= 1 << 5,
		device					= 1 << 6,
		normal					= 1 << 7,
		temporary				= 1 << 8,
		sparse_file				= 1 << 9,
		reparse_point			= 1 << 10,
		compressed				= 1 << 11,
		offline					= 1 << 12,
		not_content_indexed		= 1 << 13,
		encrypted				= 1 << 14,
		integrity_stream		= 1 << 15,
		virtual_				= 1 << 16,
		no_scrub_data			= 1 << 17,
		ea						= 1 << 18,
		pinned					= 1 << 19,
		unpinned				= 1 << 20,
		recall_on_data_access	= 1 << 22
	};

	struct attribute_collection {
		DWORD dwAttributes;

		ARTEMIS_API explicit attribute_collection(DWORD _Attributes) noexcept;
		
		template<std::input_iterator _It>
			requires(std::is_same_v<std::remove_const_t<typename std::iterator_traits<_It>::value_type>, attribute>)
		inline attribute_collection(_It _Begin, _It _End) : dwAttributes(0) {
			for (_It i = _Begin; i != _End; i = std::next(i))
				this->dwAttributes |= *i;
		}

		ARTEMIS_API attribute_collection(const std::vector<attribute>& _Attributes) noexcept;

		ARTEMIS_API bool is_present(attribute _Attribute) const noexcept;

		ARTEMIS_API void add(attribute _Attribute) noexcept;
		ARTEMIS_API void remove(attribute _Attribute) noexcept;
	};

	class file;

	class file_info {
		std::string_view _FilePath;
		std::string_view _FileName;
		std::string_view _FileType;

	public:
		ARTEMIS_API file_info(const std::string_view& _FilePath);

		ARTEMIS_API bool exists() const;

		ARTEMIS_API file open(access_mode _AccessMode, open_mode _OpenMode) const;

		ARTEMIS_API const std::string_view& name() const noexcept;
		ARTEMIS_API const std::string_view& type() const noexcept;
		ARTEMIS_API char drive_letter() const noexcept;
		ARTEMIS_API const std::string_view& qualified_path() const noexcept;
	};


	class file {
		std::string_view _FilePath;
		safe_win32_handle hFile;

	public:
		ARTEMIS_API file() noexcept;
		ARTEMIS_API file(const std::string_view& _FilePath, access_mode _AccessMode, open_mode _OpenMode);

		file(const file&) = delete;
		file(file&&) = default;

		file& operator=(const file&) = delete;
		file& operator=(file&&) = default;

		ARTEMIS_API void read(void* const _Buffer, size_t _Size) const;

		ARTEMIS_API void write(const void* const _Buffer, size_t _Size);

		ARTEMIS_API attribute_collection get_attributes() const;
		ARTEMIS_API bool has_attribute(attribute _Attribute) const;
		ARTEMIS_API void set_attributes(attribute_collection _Attributes);
	};
}

#endif // !ARTEMIS_API_FILE_HXX