//------------------------------------------------------------------------>
// MIT License
// 
// Copyright (c) 2023 Artemis Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------>
// Aurora: https://github.com/ArtemisDevGroup/Aurora
// This file was authored by:
// @Astrea0014: https://github.com/Astrea0014
//------------------------------------------------------------------------>

#ifndef __AURORA_FILE_SYSTEM_H__
#define __AURORA_FILE_SYSTEM_H__

#include "Definitions.h"
#include "Exceptions.h"

#include <Windows.h>

#undef CreateDirectory
#undef CreateFile
#undef GetCompressedFileSize

namespace Aurora {
	/// <summary>
	/// Defines how a file creation operation shall treat existing files.
	/// </summary>
	enum class CreationDisposition : A_DWORD {
		CreateNew = CREATE_NEW,					// Creates a new file, only if one does not exist.
		CreateAlways = CREATE_ALWAYS,			// Always creates a new file.
		OpenExisting = OPEN_EXISTING,			// Opens a file, only if it exists.
		OpenAlways = OPEN_ALWAYS,				// Opens a file if it exists, otherwise creates a new file.
		TruncateExisting = TRUNCATE_EXISTING	// Creates a new file, only if one already exists.
	};

	/// <summary>
	/// Defines the access rights of a file instance.
	/// </summary>
	struct FileAccessFlags {
		A_DWORD dwValue;

		enum Enumeration : A_DWORD {
			Read = FILE_GENERIC_READ,		// Allows read operations on the file.
			Write = FILE_GENERIC_WRITE,		// Allows write operations on the file.
			Execute = FILE_GENERIC_EXECUTE	// Allows the file to be executed by the process.
		};

		constexpr FileAccessFlags() noexcept : dwValue(0) {}
		constexpr FileAccessFlags(A_DWORD dwValue) noexcept : dwValue(dwValue) {}

		operator A_DWORD& () noexcept { return dwValue; }
		constexpr operator const A_DWORD& () const noexcept { return dwValue; }
	};

	/// <summary>
	/// Defines which access rights other instances can request when they open the same file.
	/// </summary>
	struct FileShareModeFlags {
		A_DWORD dwValue;

		enum Enumeration : A_DWORD {
			None = 0,
			Read = FILE_SHARE_READ,
			Write = FILE_SHARE_WRITE,
			Delete = FILE_SHARE_DELETE,
			All = Read | Write | Delete
		};

		constexpr FileShareModeFlags() noexcept : dwValue(0) {}
		constexpr FileShareModeFlags(A_DWORD dwValue) noexcept : dwValue(dwValue) {}

		operator A_DWORD& () noexcept { return dwValue; }
		constexpr operator const A_DWORD& () const noexcept { return dwValue; }
	};

	/// <summary>
	/// Defines the attributes of the file.
	/// </summary>
	struct FileAttributeFlags {
		A_DWORD dwValue;

		enum Enumeration : A_DWORD {
			Readonly = FILE_ATTRIBUTE_READONLY,							// A file that is read-only. Applications can read the file, but cannot write to it or delete it. This attribute is not honored on directories.
			Hidden = FILE_ATTRIBUTE_HIDDEN,								// The file or directory is hidden. It is not included in an ordinary directory listing.
			System = FILE_ATTRIBUTE_SYSTEM,								// A file or directory that the operating system uses a part of, or uses exclusively.
			Directory = FILE_ATTRIBUTE_DIRECTORY,						// The handle that identifies a directory.
			Archive = FILE_ATTRIBUTE_ARCHIVE,							// A file or directory that is an archive file or directory. Applications typically use this attribute to mark files for backup or removal.
			Device = FILE_ATTRIBUTE_DEVICE,								// This value is reserved for system use.
			Normal = FILE_ATTRIBUTE_NORMAL,								// A file that does not have other attributes set. This attribute is valid only when used alone.
			/// <summary>
			/// A file that is being used for temporary storage.
			/// File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application deletes a temporary file after the handle is closed.
			/// In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed.
			/// </summary>
			Temporary = FILE_ATTRIBUTE_TEMPORARY,
			SparseFile = FILE_ATTRIBUTE_SPARSE_FILE,					// A file that is a sparse file.
			ReparsePoint = FILE_ATTRIBUTE_REPARSE_POINT,				// A file or directory that has an associated reparse point, or a file that is a symbolic link.
			Compressed = FILE_ATTRIBUTE_COMPRESSED,						// A file or directory that is compressed. For a file, all of the data in the file is compressed. For a directory, compression is the default for newly created files and subdirectories.
			/// <summary>
			/// The data of a file is not available immediately. This attribute indicates that the file data is physically moved to offline storage.
			/// This attribute is used by Remote Storage, which is the hierarchical storage management software. Applications should not arbitrarily change this attribute.
			/// </summary>
			Offline = FILE_ATTRIBUTE_OFFLINE,
			NoContextIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,		// The file or directory is not to be indexed by the content indexing service.
			Encrypted = FILE_ATTRIBUTE_ENCRYPTED,						// A file or directory that is encrypted. For a file, all data streams in the file are encrypted. For a directory, encryption is the default for newly created files and subdirectories.
			/// <summary>
			/// The directory or user data stream is configured with integrity (only supported on ReFS volumes). It is not included in an ordinary directory listing.
			/// The integrity setting persists with the file if it's renamed. If a file is copied the destination file will have integrity set if either the source file or destination directory have integrity set.
			/// </summary>
			IntegrityStream = FILE_ATTRIBUTE_INTEGRITY_STREAM,
			Virtual = FILE_ATTRIBUTE_VIRTUAL,							// This value is reserved for system use.
			/// <summary>
			/// The user data stream not to be read by the background data integrity scanner (AKA scrubber). When set on a directory it only provides inheritance.
			/// This flag is only supported on Storage Spaces and ReFS volumes. It is not included in an ordinary directory listing.
			/// </summary>
			NoScrubData = FILE_ATTRIBUTE_NO_SCRUB_DATA,
			ExtendedAttributes = FILE_ATTRIBUTE_EA,						// A file or directory with extended attributes.
			/// <summary>
			/// This attribute indicates user intent that the file or directory should be kept fully present locally even when not being actively accessed.
			/// This attribute is for use with hierarchical storage management software.
			/// </summary>
			Pinned = FILE_ATTRIBUTE_PINNED,
			/// <summary>
			/// This attribute indicates that the file or directory should not be kept fully present locally except when being actively accessed.
			/// This attribute is for use with hierarchical storage management software.
			/// </summary>
			Unpinned = FILE_ATTRIBUTE_UNPINNED,
			/// <summary>
			/// This attribute only appears in directory enumeration classes (FILE_DIRECTORY_INFORMATION, FILE_BOTH_DIR_INFORMATION, etc.).
			/// When this attribute is set, it means that the file or directory has no physical representation on the local system; the item is virtual.
			/// Opening the item will be more expensive than normal, e.g. it will cause at least some of it to be fetched from a remote store.
			/// </summary>
			RecallOnOpen = FILE_ATTRIBUTE_RECALL_ON_OPEN,
			/// <summary>
			/// When this attribute is set, it means that the file or directory is not fully present locally.
			/// For a file that means that not all of its data is on local storage (e.g. it may be sparse with some data still in remote storage).
			/// For a directory it means that some of the directory contents are being virtualized from another location.
			/// Reading the file / enumerating the directory will be more expensive than normal, e.g. it will cause at least some of the file/directory content to be fetched from a remote store.
			/// Only kernel-mode callers can set this bit.
			/// </summary>
			RecallOnDataAccess = FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS
		};

		constexpr FileAttributeFlags() noexcept : dwValue(0) {}
		constexpr FileAttributeFlags(A_DWORD dwValue) noexcept : dwValue(dwValue) {}

		/// <summary>
		/// Checks if an attribute flag is set.
		/// </summary>
		/// <param name="dwAttribute">- The attribute to check.</param>
		/// <returns>True if the attribute is set, otherwise false.</returns>
		AURORA_NDWR_GET("CheckAttribute") constexpr A_BOOL CheckAttribute(_In_ const FileAttributeFlags& dwAttribute) const noexcept { return !!(dwValue & dwAttribute.dwValue); }

		operator A_DWORD& () noexcept { return dwValue; }
		constexpr operator const A_DWORD& () const noexcept { return dwValue; }
	};

	/// <summary>
	/// Defines custom file behavior.
	/// </summary>
	struct FileFlags {
		A_DWORD dwValue;

		enum Enumeration : A_DWORD {
			/// <summary>
			/// No custom file behavior.
			/// </summary>
			None = 0,
			/// <summary>
			/// The file is being opened or created for a backup or restore operation.
			/// The system ensures that the calling process overrides file security checks when the process has SE_BACKUP_NAME and SE_RESTORE_NAME privileges.
			/// You must set this flag to obtain a handle to a directory. A directory handle can be passed to some functions instead of a file handle.
			/// </summary>
			BackupSemantics = FILE_FLAG_BACKUP_SEMANTICS,
			/// <summary>
			/// The file is to be deleted immediately after all of its handles are closed, which includes the specified handle and any other open or duplicated handles.
			/// If there are existing open handles to a file, the call fails unless they were all opened with the FILE_SHARE_DELETE share mode.
			/// Subsequent open requests for the file fail, unless the FILE_SHARE_DELETE share mode is specified.
			/// </summary>
			DeleteOnClose = FILE_FLAG_DELETE_ON_CLOSE,
			/// <summary>
			/// The file or device is being opened with no system caching for data reads and writes. This flag does not affect hard disk caching or memory mapped files.
			/// </summary>
			NoBuffering = FILE_FLAG_NO_BUFFERING,
			/// <summary>
			/// Access is intended to be random. The system can use this as a hint to optimize file caching.
			/// This flag has no effect if the file system does not support cached I/O and FILE_FLAG_NO_BUFFERING.
			/// </summary>
			RandomAccess = FILE_FLAG_RANDOM_ACCESS,
			/// <summary>
			/// Access is intended to be sequential from beginning to end. The system can use this as a hint to optimize file caching.
			/// This flag should not be used if read-behind (that is, reverse scans) will be used.
			/// This flag has no effect if the file system does not support cached I/O and FILE_FLAG_NO_BUFFERING.
			/// </summary>
			SequentialScan = FILE_FLAG_SEQUENTIAL_SCAN,
			/// <summary>
			/// Write operations will not go through any intermediate cache, they will go directly to disk.
			/// </summary>
			WriteThrough = FILE_FLAG_WRITE_THROUGH
		};

		constexpr FileFlags() noexcept : dwValue(0) {}
		constexpr FileFlags(A_DWORD dwValue) : dwValue(dwValue) {}

		operator A_DWORD& () noexcept { return dwValue; }
		constexpr operator const A_DWORD& () const noexcept { return dwValue; }
	};

	/// <summary>
	/// Defines what the file pointer offset shall be relative to.
	/// </summary>
	enum class FilePointerMoveMethod : A_DWORD {
		Begin = FILE_BEGIN,			// The beginning of the file. 
		Current = FILE_CURRENT,		// The current file pointer position.
		End = FILE_END				// The end of the file.
	};

	/// <summary>
	/// Represents a local file.
	/// </summary>
	class AURORA_API File {
	protected:
		HANDLE hFile;

	private:
		A_CHAR szFilePath[MAX_PATH];
		A_CHAR szFileName[MAX_NAME];

	public:
		constexpr File() noexcept;

		/// <summary>
		/// Creates or opens a file.
		/// </summary>
		/// <param name="lpFileName">- The name of the file. This can be either relative to the host process or absolute.</param>
		/// <param name="dwCreationDisposition">- The creation disposition. See CreationDisposition for more.</param>
		/// <param name="dwDesiredAccess">- The desired access mode to the file. See FileAccessFlags for more.</param>
		/// <param name="dwAttributes">- The file attributes. See FileAttributeFlags for more.</param>
		/// <param name="dwFlags">- The file flags. See FileFlags for more.</param>
		/// <param name="dwShareMode">- The instance share mode. See FileShareModeFlags for more.</param>
		/// <param name="refTemplateFile">- A reference to a File instance to use as a template for the created file.</param>
		/// <param name="lpSecurityAttributes">- A pointer to a security attributes structure containing security information for the newly created directory. This parameter is optional.</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ParameterInvalidException"/>
		File(
			_In_z_ A_LPCSTR lpFileName,
			_In_ CreationDisposition dwCreationDisposition,
			_In_ FileAccessFlags dwDesiredAccess = FileAccessFlags::Read | FileAccessFlags::Write,
			_In_ FileAttributeFlags dwAttributes = FileAttributeFlags::Normal,
			_In_ FileFlags dwFlags = FileFlags::None,
			_In_ FileShareModeFlags dwShareMode = FileShareModeFlags::All,
			_In_opt_ const File& refTemplateFile = File(),
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr
		);

		File(const File&);

		~File();

		/// <summary>
		/// Gets an absolute path to the file.
		/// </summary>
		/// <returns>The full file path.</returns>
		AURORA_NDWR_GET("GetFilePath") _Ret_z_ constexpr A_LPCSTR GetFilePath() const noexcept;

		/// <summary>
		/// Gets the name of the file.
		/// </summary>
		/// <returns>The file name.</returns>
		AURORA_NDWR_GET("GetFileName") _Ret_z_ constexpr A_LPCSTR GetFileName() const noexcept;

		/// <summary>
		/// Gets the current file attribute flags.
		/// </summary>
		/// <returns>The file attributes.</returns>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("GetAttributes") FileAttributeFlags GetAttributes() const;

		/// <summary>
		/// Sets the current file attribute flags.
		/// </summary>
		/// <param name="dwAttributes">- The attributes to set.</param>
		/// <exception cref="WindowsApiException"/>
		A_VOID SetAttributes(_In_ FileAttributeFlags dwAttributes) const;

		/// <summary>
		/// Gets the current file pointer position relative to the beginning of the file.
		/// </summary>
		/// <returns>The current file pointer.</returns>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("GetFilePointer") A_U64 GetFilePointer() const;

		/// <summary>
		/// Sets the current file pointer position relative to the move method.
		/// </summary>
		/// <param name="dwMoveMethod">- The move method. See FilePointerMoveMethod for more.</param>
		/// <param name="uOffset">- The offset to set the file pointer to.</param>
		/// <exception cref="WindowsApiException"/>
		A_VOID SetFilePointer(_In_ FilePointerMoveMethod dwMoveMethod, _In_ A_U64 uOffset) const;

		/// <summary>
		/// Reads the requested number of bytes from the file pointer.
		/// </summary>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
		/// <param name="dwSize">- The size of the buffer.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		A_VOID Read(
			_Out_writes_bytes_(dwSize) A_LPVOID lpBuffer,
			_In_ A_DWORD dwSize
		) const;

		/// <summary>
		/// Writes the requested number of bytes to the file pointer.
		/// </summary>
		/// <param name="lpBuffer">- A pointer to a buffer that contains the data to write.</param>
		/// <param name="dwSize">- The size of the buffer.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		A_VOID Write(
			_In_reads_bytes_(dwSize) A_LPCVOID lpBuffer,
			_In_ A_DWORD dwSize
		) const;

		/// <summary>
		/// Gets the size of the file in bytes.
		/// </summary>
		/// <returns>The file size.</returns>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("GetFileSize") A_U64 GetFileSize() const;

		/// <summary>
		/// Gets the compressed size of the file in bytes.
		/// </summary>
		/// <returns>The compressed file size.</returns>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("GetCompressedFileSize") A_U64 GetCompressedFileSize() const;

		/// <summary>
		/// Flushes the current file buffers forcing the contents of the buffers to be written to disk.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		A_VOID Flush() const;
	};

	/// <summary>
	/// Represents a local text based file.
	/// </summary>
	class AURORA_API TextFile : public File {
		FILE* lpFile;

	public:
		constexpr TextFile() noexcept;
		
		/// <summary>
		/// Creates or opens a text file.
		/// </summary>
		/// <param name="lpFileName">- The name of the file. This can be either relative to the host process or absolute.</param>
		/// <param name="dwCreationDisposition">- The creation disposition. See CreationDisposition for more.</param>
		/// <param name="dwDesiredAccess">- The desired access mode to the file. See FileAccessFlags for more.</param>
		/// <param name="dwAttributes">- The file attributes. See FileAttributeFlags for more.</param>
		/// <param name="dwFlags">- The file flags. See FileFlags for more.</param>
		/// <param name="dwShareMode">- The instance share mode. See FileShareModeFlags for more.</param>
		/// <param name="refTemplateFile">- A reference to a File instance to use as a template for the created file.</param>
		/// <param name="lpSecurityAttributes">- A pointer to a security attributes structure containing security information for the newly created directory. This parameter is optional.</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ParameterInvalidException"/>
		TextFile(
			_In_z_ A_LPCSTR lpFileName,
			_In_ CreationDisposition dwCreationDisposition,
			_In_ FileAccessFlags dwDesiredAccess = FileAccessFlags::Read | FileAccessFlags::Write,
			_In_ FileAttributeFlags dwAttributes = FileAttributeFlags::Normal,
			_In_ FileFlags dwFlags = FileFlags::None,
			_In_ FileShareModeFlags dwShareMode = FileShareModeFlags::All,
			_In_opt_ const File& refTemplateFile = File(),
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr
		);

		~TextFile();

		/// <summary>
		/// Reads the format from the current file pointer.
		/// </summary>
		/// <param name="lpFormat">- The format to read.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		/// <exception cref="ErrnoException"/>
		A_VOID ReadFormatV(_In_z_ _Scanf_format_string_ A_LPCSTR lpFormat, _In_ va_list lpArgs) const;

		/// <summary>
		/// Reads the format from the current file pointer.
		/// </summary>
		/// <param name="lpFormat">- The format to read.</param>
		/// <exception cref="ErrnoException"/>
		A_VOID ReadFormat(_In_z_ _Scanf_format_string_ A_LPCSTR lpFormat, ...) const;

		/// <summary>
		/// Writes the format to the current file pointer.
		/// </summary>
		/// <param name="lpFormat">- The format to write.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		/// <exception cref="ErrnoException"/>
		A_VOID WriteFormatV(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, _In_ va_list lpArgs) const;

		/// <summary>
		/// Writes the format to the current file pointer.
		/// </summary>
		/// <param name="lpFormat">- The format to write.</param>
		/// <exception cref="ErrnoException"/>
		A_VOID WriteFormat(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, ...) const;
	};

	/// <summary>
	/// Represents a local directory.
	/// </summary>
	class AURORA_API Directory {
		A_CHAR szDirectoryPath[MAX_PATH];
		A_CHAR szDirectoryName[MAX_NAME];

	public:
		/// <summary>
		/// Opens an existing directory.
		/// </summary>
		/// <param name="lpDirectoryName">- The name of the directory to open.</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ParameterInvalidException"/>
		Directory(_In_z_ A_LPCSTR lpDirectoryName);

		/// <summary>
		/// Gets an absolute path to the directory.
		/// </summary>
		/// <returns>The full directory path.</returns>
		AURORA_NDWR_GET("GetDirectoryPath") _Ret_z_ constexpr A_LPCSTR GetDirectoryPath() const noexcept;

		/// <summary>
		/// Gets the name of the directory.
		/// </summary>
		/// <returns>The directory name.</returns>
		AURORA_NDWR_GET("GetDirectoryName") _Ret_z_ constexpr A_LPCSTR GetDirectoryName() const noexcept;
		
		/// <summary>
		/// Gets the current directory attribute flags.
		/// </summary>
		/// <returns>The directory attributes.</returns>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("GetAttributes") FileAttributeFlags GetAttributes() const;

		/// <summary>
		/// Sets the current directory attribute flags.
		/// </summary>
		/// <param name="dwAttributes">- The attributes to set.</param>
		/// <exception cref="WindowsApiException"/>
		A_VOID SetAttributes(_In_ FileAttributeFlags dwAttributes) const;

		/// <summary>
		/// Creates or opens a sub-directory in the current directory.
		/// </summary>
		/// <param name="lpDirectoryName">- The name of the directory to create.</param>
		/// <param name="bThrowOnExists">- Determines wether the function should throw or complete successfully if the directory already exists. Defaults to false.</param>
		/// <param name="lpSecurityAttributes">- A pointer to a security attributes structure containing security information for the newly created directory. This parameter is optional.</param>
		/// <returns>The newly created directory.</returns>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ParameterInvalidException"/>
		AURORA_NDWR_CREATE("CreateDirectory") Directory CreateDirectory(
			_In_z_ A_LPCSTR lpDirectoryName,
			_In_ A_BOOL bThrowOnExists = false,
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr
		) const;

		/// <summary>
		/// Creates a file in the current directory.
		/// </summary>
		/// <param name="lpFileName">- The name of the file to create.</param>
		/// <param name="dwCreationDisposition">- The disposition of the creation.</param>
		/// <param name="dwDesiredAccess">- The desired access mode to the file.</param>
		/// <param name="dwAttributes">- The attributes of the file.</param>
		/// <param name="dwFlags">- Any additional file flags.</param>
		/// <param name="dwShareMode">- The share mode of the file.</param>
		/// <param name="refTemplateFile">- A reference to a file object to use as the template of the file. Only applicable if the file is created.</param>
		/// <param name="lpSecurityAttributes">- A pointer to a security attributes structure containing security information for the newly created file. This parameter is optional.</param>
		/// <returns>The newly created file.</returns>
		/// <exceptions cref="WindowsApiException"/>
		/// <exception cref="ParameterInvalidException"/>
		AURORA_NDWR_CREATE("CreateFile") File CreateFile(
			_In_z_ A_LPCSTR lpFileName,
			_In_ CreationDisposition dwCreationDisposition,
			_In_ FileAccessFlags dwDesiredAccess = FileAccessFlags::Read | FileAccessFlags::Write,
			_In_ FileAttributeFlags dwAttributes = FileAttributeFlags::Normal,
			_In_ FileFlags dwFlags = FileFlags::None,
			_In_ FileShareModeFlags dwShareMode = FileShareModeFlags::All,
			_In_opt_ const File& refTemplateFile = File(),
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr
		) const;
	};

	/// <summary>
	/// Creates a directory at the specified location.
	/// </summary>
	/// <param name="lpDirectoryName">- A full path or a path relative to the current process to create the directory at.</param>
	/// <param name="bThrowOnExists">- Determines wether the function should throw or complete successfully if the directory already exists. Defaults to false.</param>
	/// <param name="lpSecurityAttributes">- A pointer to a security attributes structure containing security information for the newly created directory. This parameter is optional.</param>
	/// <returns>The newly created directory.</returns>
	/// <exception cref="WindowsApiException"/>'
	/// <exception cref="ParameterInvalidException"/>
	AURORA_NDWR_CREATE("CreateDirectory") AURORA_API Directory CreateDirectory(
		_In_z_ A_LPCSTR lpDirectoryName,
		_In_ A_BOOL bThrowOnExists = false,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr
	);

	/// <summary>
	/// Represents a local root directory. (C:\, D:\ etc...)
	/// </summary>
	class AURORA_API RootDirectory : public Directory {
		A_CHAR szDrivePath[4];

		_Ret_z_ A_LPCSTR SetDrivePath(_In_ A_CHAR cDriveLetter);

	public:
		/// <summary>
		/// Constructs a RootDirectory.
		/// </summary>
		/// <param name="cDriveLetter">- The letter of the drive to open. The character MUST be uppercase, otherwise this function will fail.</param>
		/// <exception cref="ParameterInvalidException"/>
		RootDirectory(_In_range_('C', 'Z') A_CHAR cDriveLetter);

		/// <summary>
		/// Gets the total number of bytes on the disk.
		/// </summary>
		/// <returns>The total disk space.</returns>
		AURORA_NDWR_GET("GetTotalDiskSpace") A_U64 GetTotalDiskSpace() const;

		/// <summary>
		/// Gets the total number of free bytes on the disk.
		/// </summary>
		/// <returns>The total free disk space.</returns>
		AURORA_NDWR_GET("GetFreeDiskSpace") A_U64 GetFreeDiskSpace() const;
	};
}

#endif // !__AURORA_FILE_SYSTEM_H__