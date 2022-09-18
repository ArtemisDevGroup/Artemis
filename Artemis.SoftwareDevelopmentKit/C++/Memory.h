#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "../H/Definitions.h"
#include "MemoryDefinitions.h"
#include "Exceptions.h"

#undef GetModuleHandle

/// <summary>
/// A class for scanning memory for different kinds of data.
/// </summary>
class MemoryScanner {
public:
	/// <summary>
	/// Represents which protection level the memory to be scanned should have.
	/// </summary>
	enum ScanProtection {
		Read, // Scans all readable memory.
		ReadWrite, // Scans all read and writable memory.
		ReadOnly, // Scans all readonly memory.
		Execute_Read, // Scans all readable executable memory.
		Execute_ReadWrite, // Scans all read and writable executable memory.
		Execute_ReadOnly, // Scans all readonly executable memory.
		NoExecute_Read, // Scans all readable non-executable memory.
		NoExecute_ReadWrite, // Scans all read and writable non-executable memory.
		NoExecute_ReadOnly // Scans all readonly non-executable memory.
	};
private:
	HANDLE hThread;

	enum ScanType { Invalid, CodePattern, IdaPattern, StringA, StringW };

	typedef struct _SCAN_PARAMS {
		LPADDRESS lpAlloc;
		DWORD dwResultCount;
		HANDLE hProcess;

		ScanType Type;

		DWORD dwMaxResultCount;
		ADDRESS uStartAddress;
		SIZE_T uScanSize;
		DWORD dwAlignment;
		ScanProtection Protection;

		union {
			struct {
				CHAR szPattern[MAX_PATTERN_LENGTH];
				CHAR szMask[MAX_PATTERN_LENGTH];
			} CodePattern;
			struct {
				CHAR szPattern[MAX_STRING_LENGTH];
			} IdaPattern;
			struct {
				CHAR szString[MAX_STRING_LENGTH];
				BOOL bCaseSensitive;
			} StringA;
			struct {
				WCHAR szString[MAX_STRING_LENGTH];
				BOOL bCaseSensitive;
			} StringW;
		};
	} SCAN_PARAMS, * LPSCAN_PARAMS;
	
	SCAN_PARAMS params;

	static DWORD APIENTRY StartScan(LPSCAN_PARAMS);

public:
	MemoryScanner(); // Null constructor.
	MemoryScanner(const MemoryScanner&); // Copy constructor.
	~MemoryScanner(); // Destructor.

	/// <summary>
	/// Constructs a memory scanner object scanning for a code style pattern.
	/// </summary>
	/// <param name="lpPattern">- A pointer to the pattern.</param>
	/// <param name="lpMask">- A pointer to the mask. Must be null-terminated.</param>
	/// <exception cref="ParameterException"/>
	MemoryScanner(_In_ LPCSTR lpPattern, _In_z_ LPCSTR lpMask);

	/// <summary>
	/// Constructs a memory scanner object scanning for an IDA style pattern.
	/// </summary>
	/// <param name="lpPattern">- A pointer to the pattern. Must be null-terminated.</param>
	/// <exception cref="ParameterException"/>
	MemoryScanner(_In_z_ LPCSTR lpPattern);

	/// <summary>
	/// Constructs a memory scanner object scanning for a string.
	/// </summary>
	/// <param name="lpString">- The string to scan for.</param>
	/// <param name="bCaseSensitive">- A bool that determines wether to do a case sensitive scan or not.</param>
	/// <exception cref="ParameterException"/>
	MemoryScanner(_In_z_ LPCSTR lpString, _In_ BOOL bCaseSensitive);

	/// <summary>
	/// Constructs a memory scanner scanning for a string.
	/// </summary>
	/// <param name="lpString">- The string to scan for.</param>
	/// <param name="bCaseSensitive">- A bool that determines wether to do a case sensitive scan or not.</param>
	/// <exception cref="ParameterException"/>
	MemoryScanner(_In_z_ LPCWSTR lpString, _In_ BOOL bCaseSensitive);

	/// <summary>
	/// Sets the handle to use in the event that an external process' virtual memory is being scanned.
	/// </summary>
	/// <param name="hProcess">- A handle to the process.</param>
	void SetExternalHandle(_In_ HANDLE hProcess);

	/// <summary>
	/// Sets the maximum number of results to scan for.
	/// </summary>
	/// <param name="dwMaxResultCount">- The number of results to get.</param>
	void SetMaxResultCount(_In_ DWORD dwMaxResultCount = 1);

	/// <summary>
	/// Sets the address where the scanner starts scanning memory.
	/// </summary>
	/// <param name="uStartAddress">- The start address.</param>
	void SetStartAddress(_In_ ADDRESS uStartAddress = MIN_ADDRESS);

	/// <summary>
	/// Sets the number of bytes to scan before stopping.
	/// </summary>
	/// <param name="uScanSize">- The number of bytes to scan.</param>
	void SetScanSize(_In_ SIZE_T uScanSize = MAX_ADDRESS);

	/// <summary>
	/// Sets the scan alignment. (The number of bytes to jump every scan iteration.)
	/// </summary>
	/// <param name="dwAlignment">- The scan alignment.</param>
	void SetAlignment(_In_ DWORD dwAlignment = 1);

	/// <summary>
	/// Sets the protection level of the memory to scan. For example, if you want to only scan readonly memory, you can specify that here.
	/// </summary>
	/// <param name="Protection">- A member from the ScanProtection enum indicating the protection level.</param>
	void SetScanProtection(_In_ ScanProtection Protection = Read);

	/// <summary>
	/// Starts the scan in a new thread.
	/// </summary>
	/// <exception cref="WindowsApiException"/>
	void Start();

	/// <summary>
	/// Waits for the scan thread to finish.
	/// </summary>
	void Wait();

	/// <summary>
	/// Gets the list of addresses found.
	/// </summary>
	/// <returns></returns>
	LPADDRESS GetResult();

	/// <summary>
	/// Gets the number of addresses found.
	/// </summary>
	/// <returns></returns>
	DWORD GetResultCount();

	/// <summary>
	/// Gets the thread exit code.
	/// </summary>
	/// <returns></returns>
	DWORD GetExitCode();
};

/// <summary>
/// Represents a memory page protection constant.
/// </summary>
enum class MemoryProtection {
	/// <summary>
	/// Executable readonly memory.
	/// </summary>
	Execute_ReadOnly = PAGE_EXECUTE_READ,

	/// <summary>
	/// Executable read and writable memory.
	/// </summary>
	Execute_ReadWrite = PAGE_EXECUTE_READWRITE,

	/// <summary>
	/// Non-executable readonly memory.
	/// </summary>
	ReadOnly = PAGE_READONLY,

	/// <summary>
	/// Non-executable read and writable memory.
	/// </summary>
	ReadWrite = PAGE_READWRITE
};

/// <summary>
/// Constants that determine how memory should be allocated.
/// </summary>
enum class AllocationType {
	/// <summary>
	/// Commits the memory in the region. This means it makes the memory usable. Cannot be used if the memory is not reserved.
	/// </summary>
	Commit = MEM_COMMIT,

	/// <summary>
	/// Reserves the memory region. This means the memory is on standby and cannot be allocated for something else.
	/// </summary>
	Reserve = MEM_RESERVE,

	/// <summary>
	/// Allocates the highest memory page possible. This means it'll give you a super high address.
	/// </summary>
	TopDown = MEM_TOP_DOWN
};

/// <summary>
/// Constants that determine the way a memory allocation should be freed.
/// </summary>
enum class FreeType {
	/// <summary>
	/// Decommits the memory and puts it in a reserved state.
	/// </summary>
	Decommit = MEM_DECOMMIT,

	/// <summary>
	/// Releases the memory to be used by the process.
	/// </summary>
	Release = MEM_RELEASE
};

inline AllocationType operator|(AllocationType l, AllocationType r) { return (AllocationType)((DWORD)l | (DWORD)r); }

/// <summary>
/// A class for creating and freeing virtual memory allocations.
/// </summary>
class VirtualAllocation {
public:
	HANDLE hProcess;
	ADDRESS uAddress;
	SIZE_T uSize;
	AllocationType Type;
	MemoryProtection Protection;
	ADDRESS uAllocatedAddress;

public:
	VirtualAllocation();
	~VirtualAllocation();

	/// <summary>
	/// Constructs a VirtualAllocation object based on the provided parameters.
	/// </summary>
	/// <param name="uAddress">- The address to allocate at.</param>
	/// <param name="uSize">- The number of bytes to allocate.</param>
	/// <param name="Type">- The type of allocation to make.</param>
	/// <param name="Protection">- The memory protection of the allocation.</param>
	VirtualAllocation(
		_In_ ADDRESS uAddress,
		_In_ SIZE_T uSize,
		_In_ AllocationType Type,
		_In_ MemoryProtection Protection
	);

	/// <summary>
	/// Sets the handle to use in the event that an external process' virtual memory is being allocated.
	/// </summary>
	/// <param name="hProcess">- A handle to the process.</param>
	void SetExternalHandle(_In_ HANDLE hProcess);

	/// <summary>
	/// Allocates a memory region based on the parameters set in the constructor.
	/// </summary>
	/// <exception cref="WindowsApiException"/>
	void Allocate();

	/// <summary>
	/// Frees previously allocated memory.
	/// </summary>
	/// <param name="Type">- The method to use when freeing the allocation.</param>
	/// <exception cref="WindowsApiException"/>
	void Free(FreeType Type = FreeType::Release);

	/// <summary>
	/// Gets the real address of the allocated memory region.
	/// </summary>
	/// <returns>- The address of the region.</returns>
	ADDRESS GetAllocatedAddress();
};

/// <summary>
/// Á class for hooking functions.
/// </summary>
class Hook {
public:
	/// <summary>
	/// Initializes the hooking library.
	/// </summary>
	static void InitializeLibrary();
	/// <summary>
	/// Uninitializes the hooking library.
	/// </summary>
	static void UninitializeLibrary();

	/// <summary>
	/// Creates a hook on the API function provided.
	/// </summary>
	/// <param name="lpModule">- The module where the function is contained.</param>
	/// <param name="lpFunction">- The name of the function to fetch from the export table.</param>
	/// <param name="lpDetour">- The function to replace with.</param>
	/// <param name="lpOriginal">- A pointer to call for the original function.</param>
	Hook(
		_In_z_ LPCSTR lpModule,
		_In_z_ LPCSTR lpFunction,
		_In_ LPCVOID lpDetour,
		_Out_opt_ LPVOID lpOriginal = nullptr
	);

	/// <summary>
	/// Creates a hook on the target function.
	/// </summary>
	/// <param name="lpTarget">- The target function.</param>
	/// <param name="lpDetour">- The function to replace with.</param>
	/// <param name="lpOriginal">- A pointer to call for the original function.</param>
	Hook(
		_In_ LPCVOID lpTarget,
		_In_ LPCVOID lpDetour,
		_Out_opt_ LPVOID lpOriginal = nullptr
	);
	
	/// <summary>
	/// Creates a hook on the function at the target address.
	/// </summary>
	/// <param name="uTarget">- The address of the function.</param>
	/// <param name="lpDetour">- The function to replace with.</param>
	/// <param name="lpOriginal">- A pointer to call for the original function.</param>
	Hook(
		_In_ ADDRESS uTarget,
		_In_ LPCVOID lpDetour,
		_Out_opt_ LPVOID lpOriginal = nullptr
	);

	/// <summary>
	/// Enables the hook.
	/// </summary>
	void Enable();

	/// <summary>
	/// Disables the hook.
	/// </summary>
	void Disable();

	/// <summary>
	/// Frees the hook instance by removing the hook.
	/// </summary>
	void Free();
};

/// <summary>
/// A class for injecting dynamic link libraries into a process.
/// </summary>
class DllInjection {
public:
	enum Architecture { X86, X64 };
	enum PathRelativity { Absolute, ToCaller, ToTarget };
private:
	HANDLE hProcess;
	BOOL bCloseHandle;
	Architecture arch;
	PathRelativity pr;
	CHAR szPath[MAX_PATH];
public:
	/// <summary>
	/// Creates a DllInjection instance.
	/// </summary>
	/// <param name="lpTargetProcess">- The name of the target process.</param>
	/// <param name="lpDllPath">- The path of the dll to inject.</param>
	/// <param name="arch">- The architecture of the dll to inject.</param>
	/// <param name="pr">- The relativity of the path specified as "lpDllPath".</param>
	DllInjection(
		_In_z_ LPCSTR lpTargetProcess,
		_In_z_ LPCSTR lpDllPath,
		_In_ Architecture arch = X64,
		_In_ PathRelativity pr = ToCaller
	);

	/// <summary>
	/// Creates a DllInjection instance.
	/// </summary>
	/// <param name="dwTargetProcessId">- The unique identifier of the target process.</param>
	/// <param name="lpDllPath">- The path of the dll to inject.</param>
	/// <param name="arch">- The architecture of the dll to inject.</param>
	/// <param name="pr">- The relativity of the path specified as "lpDllPath".</param>
	DllInjection(
		_In_ DWORD dwTargetProcessId,
		_In_z_ LPCSTR lpDllPath,
		_In_ Architecture arch = X64,
		_In_ PathRelativity pr = ToCaller
	);

	/// <summary>
	/// Creates a DllInjection instance.
	/// </summary>
	/// <param name="hTargetProcess">- A handle to the target process. Must have the following rights:
	/// <para>PROCESS_QUERY_INFORMATION</para>
	/// <para>PROCESS_VM_OPERATION</para>
	/// <para>PROCESS_VM_READ</para>
	/// <para>PROCESS_VM_WRITE</para>
	/// <para>PROCESS_CREATE_THREAD</para>
	/// </param>
	/// <param name="lpDllPath">- The path of the dll to inject.</param>
	/// <param name="arch">- The architecture of the dll to inject.</param>
	/// <param name="pr">- The relativity of the path specified as "lpDllPath".</param>
	DllInjection(
		_In_ HANDLE hTargetProcess,
		_In_z_ LPCSTR lpDllPath,
		_In_ Architecture arch = X64,
		_In_ PathRelativity pr = ToCaller
	);

	~DllInjection();

	/// <summary>
	/// Injects the library.
	/// </summary>
	void Inject();
};

/// <summary>
/// A class for interacting with the local or a remote process' virtual memory region.
/// </summary>
class Memory {
public:
	/// <summary>
	/// The object target type.
	/// </summary>
	enum Type {
		/// <summary>
		/// Indicates that the memory region targetted is the one of the local process.
		/// </summary>
		Internal,

		/// <summary>
		/// Indicates that the memory region targetted is the one of a remote process.
		/// </summary>
		External
	};
private:
	Type TargetType;

	CHAR szProcessName[MAX_NAME];
	CHAR szProcessPath[MAX_PATH];
	CHAR szModuleName[MAX_NAME];
	CHAR szModulePath[MAX_PATH];

	HANDLE hProcess;
	HMODULE hModule;
	DWORD dwProcessId;
	ADDRESS uModuleBaseAddress;
	DWORD dwModuleSize;

	void _Read(
		_In_ ADDRESS uAddress,
		_Out_ LPVOID lpBuffer,
		_In_ DWORD dwSize
	);

	void _Write(
		_In_ ADDRESS uAddress,
		_In_ LPCVOID lpBuffer,
		_In_ DWORD dwSize
	);

public:
	Memory();

	/// <summary>
	/// Constructs a Memory object targetting a module in the local process' virtual memory region.
	/// </summary>
	/// <param name="lpModuleName">- The name of the module to base off of with it's extension. This parameter is optional, nullptr will target the main module of the process.</param>
	Memory(_In_opt_ LPCSTR lpModuleName);

	/// <summary>
	/// Constructs a Memory object targetting a module in a remote process' virtual memory region.
	/// </summary>
	/// <param name="lpProcessName">- The name of the process along with it's extension.</param>
	/// <param name="lpModuleName">- The name of the module to base off of with it's extension. This parameter is optional, nullptr will target the main module of the process.</param>
	Memory(_In_ LPCSTR lpProcessName, _In_opt_ LPCSTR lpModuleName);

	~Memory();

	/// <summary>
	/// Gets the name of the target process along with it's file extension.
	/// </summary>
	/// <returns>The name of the process.</returns>
	LPCSTR GetProcessName() const;

	/// <summary>
	/// Gets the full path of the target process.
	/// </summary>
	/// <returns>The path of the process.</returns>
	LPCSTR GetProcessPath() const;

	/// <summary>
	/// Gets the name of the target module along with itäs file extension.
	/// </summary>
	/// <returns>The name of the module.</returns>
	LPCSTR GetModuleName() const;

	/// <summary>
	/// Gets the path of the target module.
	/// </summary>
	/// <returns>The path of the module.</returns>
	LPCSTR GetModulePath() const;

	/// <summary>
	/// Gets the target process' type.
	/// </summary>
	/// <returns>The process type.</returns>
	Type GetTargetType() const;

	/// <summary>
	/// Gets a handle to the target process.
	/// </summary>
	/// <returns>A process handle.</returns>
	HANDLE GetProcessHandle() const;

	/// <summary>
	/// Gets the unique system identifier of the target process.
	/// </summary>
	/// <returns>>The process id.</returns>
	DWORD GetProcessId() const;

	/// <summary>
	/// Gets a handle to the target module.
	/// </summary>
	/// <returns>A module handle.</returns>
	HMODULE GetModuleHandle() const;

	/// <summary>
	/// Gets the base address of the target module's executable allocation.
	/// </summary>
	/// <returns>The module base address.</returns>
	ADDRESS GetModuleBase() const;

	/// <summary>
	/// Gets the size of the target module's executable allocation in bytes.
	/// </summary>
	/// <returns>The module size.</returns>
	DWORD GetModuleSize() const;

	/// <summary>
	/// Reads memory from an address.
	/// </summary>
	/// <typeparam name="T">- The datatype to read.</typeparam>
	/// <param name="uAddress">- The address to read.</param>
	/// <returns>The read value.</returns>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T>
	inline T Read(_In_ ADDRESS uAddress) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);

		T n;
		_Read(uAddress, &n, sizeof(T));

		if (!b) ExceptionContext::ResetContext();
		return n;
	}

	/// <summary>
	/// Reads an array from an address.
	/// </summary>
	/// <typeparam name="T">- The datatype to read.</typeparam>
	/// <typeparam name="uSize">- The number of elements to read.</typeparam>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A pointer to the buffer to receive the read data.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T, UINT uSize>
	inline void ReadArray(
		_In_ ADDRESS uAddress,
		_Out_ T(&lpBuffer)[uSize]
	) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		_Read(uAddress, lpBuffer, uSize * sizeof(T));
		if (!b) ExceptionContext::ResetContext();
	}

	/// <summary>
	/// Reads an array from an address.
	/// </summary>
	/// <typeparam name="T">- The datatype to read.</typeparam>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
	/// <param name="uSize">- The size of the buffer in bytes.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T>
	inline void ReadArray(
		_In_ ADDRESS uAddress,
		_Out_ T* lpBuffer,
		_In_ UINT uSize
	) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		_Read(uAddress, lpBuffer, uSize);
		if (!b) ExceptionContext::ResetContext();
	}

	/// <summary>
	/// Reads the address from the end of a pointer.
	/// </summary>
	/// <param name="uAddress">- The pointer base address.</param>
	/// <param name="lpPointer">- A pointer to an object containing the pointer.</param>
	/// <returns>The address at the end of the pointer.</returns>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	ADDRESS ReadPtrAddress(
		_In_ ADDRESS uAddress,
		_In_ LPCPOINTER lpPointer
	);

	/// <summary>
	/// Reads the address from the end of a pointer.
	/// </summary>
	/// <param name="lpPointer">- A pointer to an object containing the base offset and the pointer.</param>
	/// <returns>The address at the end of the pointer.</returns>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	ADDRESS ReadPtrAddress(_In_ LPCBASE_POINTER lpPointer);

	/// <summary>
	/// Reads data from a pointer.
	/// </summary>
	/// <typeparam name="T">- The datatype to read.</typeparam>
	/// <param name="uAddress">- The pointer base address.</param>
	/// <param name="lpPointer">- A pointer to an object containing the pointer.</param>
	/// <returns>The read data.</returns>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T>
	inline T ReadPtr(
		_In_ ADDRESS uAddress,
		_In_ LPCPOINTER lpPointer
	) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		T ret = Read<T>(ReadPtrAddress(uAddress, lpPointer));
		if (!b) ExceptionContext::ResetContext();
		return ret;
	}

	/// <summary>
	/// Reads data from a pointer.
	/// </summary>
	/// <typeparam name="T">- The datatype to read.</typeparam>
	/// <param name="lpPointer">- A pointer to an object containing the base offset and the pointer.</param>
	/// <returns>The read data.</returns>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T>
	inline T ReadPtr(_In_ LPCBASE_POINTER lpPointer) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		T ret = Read<T>(ReadPtrAddress(lpPointer));
		if (!b) ExceptionContext::ResetContext();
		return ret;
	}

	/// <summary>
	/// Reads a string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
	/// <param name="uCount">- The number of characters to read.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	void ReadStringA(
		_In_ ADDRESS uAddress,
		_Out_ LPSTR lpBuffer,
		_In_ UINT uCount
	);

	/// <summary>
	/// Reads a string from an address.
	/// </summary>
	/// <typeparam name="uCount">- The number of characters to read.</typeparam>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<UINT uCount>
	inline void ReadStringA(
		_In_ ADDRESS uAddress,
		_Out_ CHAR(&lpBuffer)[uCount]
	) {
		ReadStringA(uAddress, lpBuffer, uCount);
	}

	/// <summary>
	/// Reads a string from an address.
	/// </summary>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
	/// <param name="uCount">- The number of characters to read.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	void ReadStringW(
		_In_ ADDRESS uAddress,
		_Out_writes_z_(uCount) LPWSTR lpBuffer,
		_In_ UINT uCount
	);

	/// <summary>
	/// Reads a string from an address.
	/// </summary>
	/// <typeparam name="uCount">- The number of characters to read.</typeparam>
	/// <param name="uAddress">- The address to read.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<UINT uCount>
	inline void ReadStringW(
		_In_ ADDRESS uAddress,
		_Out_writes_z_(uCount) WCHAR(&lpBuffer)[uCount]
	) {
		ReadStringW(uAddress, lpBuffer, uCount);
	}

	/// <summary>
	/// Writes data to an address.
	/// </summary>
	/// <typeparam name="T">- The datatype to write.</typeparam>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="Data">- The data to write.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T>
	inline void Write(
		_In_ ADDRESS uAddress,
		_In_ const T Data
	) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		_Write(uAddress, &Data, sizeof(T));
		if (!b) ExceptionContext::ResetContext();
	}

	/// <summary>
	/// Writes an array of data to an address.
	/// </summary>
	/// <typeparam name="T">- The datatype to write.</typeparam>
	/// <typeparam name="uSize">- The number of elements to write.</typeparam>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T, UINT uSize>
	inline void WriteArray(
		_In_ ADDRESS uAddress,
		_In_ const T(&lpBuffer)[uSize]
	) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		_Write(uAddress, lpBuffer, uSize * sizeof(T));
		if (!b) ExceptionContext::ResetContext();
	}

	/// <summary>
	/// Writes an array of data to an address.
	/// </summary>
	/// <typeparam name="T">- The datatype to write.</typeparam>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="uSize">- The size of the buffer in bytes.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T>
	inline void WriteArray(
		_In_ ADDRESS uAddress,
		_In_ const T* lpBuffer,
		_In_ UINT uSize
	) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		_Write(uAddress, lpBuffer, uSize);
		if (!b) ExceptionContext::ResetContext();
	}

	/// <summary>
	/// Writes data to a pointer.
	/// </summary>
	/// <typeparam name="T">- The datatype to write.</typeparam>
	/// <param name="uAddress">- The pointer base address.</param>
	/// <param name="lpPointer">- A pointer to an object containing the pointer.</param>
	/// <param name="Data">- The data to write.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T>
	inline void WritePtr(
		_In_ ADDRESS uAddress,
		_In_ LPCPOINTER lpPointer,
		_In_ const T Data
	) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		Write<T>(ReadPtrAddress(uAddress, lpPointer), Data);
		if (!b) ExceptionContext::ResetContext();
	}

	/// <summary>
	/// Writes data to a pointer.
	/// </summary>
	/// <typeparam name="T">- The datatype to write.</typeparam>
	/// <param name="lpPointer">- A pointer to an object containing the base offset and the pointer.</param>
	/// <param name="Data">- The data to write.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	template<typename T>
	inline void WritePtr(
		_In_ LPCBASE_POINTER lpPointer,
		_In_ const T Data
	) {
		BOOL b = ExceptionContext::HasContext();
		if (!b) ExceptionContext::SetContext(__FUNCTION__);
		Write<T>(ReadPtrAddress(lpPointer), Data);
		if (!b) ExceptionContext::ResetContext();
	}

	/// <summary>
	/// Writes a string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="lpString">- The string to write.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	void WriteStringA(
		_In_ ADDRESS uAddress,
		_In_z_ LPCSTR lpString
	);

	/// <summary>
	/// Writes a string to an address.
	/// </summary>
	/// <param name="uAddress">- The address to write to.</param>
	/// <param name="lpString">- The string to write.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	void WriteStringW(
		_In_ ADDRESS uAddress,
		_In_z_ LPCWSTR lpString
	);

	/// <summary>
	/// Defines which operation to run.
	/// </summary>
	enum AssemblyAction {
		/// <summary>
		/// Writes the enable code.
		/// </summary>
		Enable,

		/// <summary>
		/// Writes the disable code.
		/// </summary>
		Disable
	};

	/// <summary>
	/// Patches assembly code.
	/// </summary>
	/// <param name="uAddress">- The address to patch.</param>
	/// <param name="lpPatch">- A pointer to an object containing the assembly code to patch.</param>
	/// <param name="Action">- The action to take.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	void AssemblyPatch(
		ADDRESS uAddress,
		LPCASM_PATCH lpPatch,
		AssemblyAction Action
	);

	/// <summary>
	/// Patches assembly code.
	/// </summary>
	/// <param name="lpPatch">- A pointer to an object containing the assembly code to patch aswell as the base offset.</param>
	/// <param name="Action">- The action to take.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="Exception"/>
	/// <exception cref="MemoryAccessViolationException (Internal)"/>
	/// <exception cref="WindowsApiException (External)"/>
	/// <exception cref="InstanceInvalidException"/>
	void AssemblyPatch(
		LPCBASE_ASM_PATCH lpPatch,
		AssemblyAction Action
	);

	/// <summary>
	/// Creates a memory scanner associated to the current target process. Scans a code style pattern.
	/// </summary>
	/// <param name="lpPattern">- The pattern to scan.</param>
	/// <param name="lpMask">- The associated pattern mask.</param>
	/// <param name="bScanModule">- Defines wether the scanner should only scan the process allocation or not.</param>
	/// <returns>An initialized MemoryScanner object.</returns>
	/// <exception cref="ParameterException"/>
	MemoryScanner CreateScanner(
		_In_ LPCSTR lpPattern,
		_In_z_ LPCSTR lpMask,
		_In_ BOOL bScanModule = TRUE
	);

	/// <summary>
	/// Creates a memory scanner associared with the current target process. Scans an IDA style pattern.
	/// </summary>
	/// <param name="lpPattern">- The pattern to scan.</param>
	/// <param name="bScanModule">- Defines wether the scanner should only scan the process allocation or not.</param>
	/// <returns>An initialized MemoryScanner object.</returns>
	/// <exception cref="ParameterException"/>
	MemoryScanner CreateScanner(
		_In_z_ LPCSTR lpPattern,
		_In_ BOOL bScanModule = TRUE
	);

	/// <summary>
	/// Creates a memory scanner associated with the current target process. Scans a string.
	/// </summary>
	/// <param name="lpString">- The string to scan.</param>
	/// <param name="bCaseSensitive"><- Defines wether the scan should be case sensitive or not.s/param>
	/// <param name="bScanModule">- Defines wether the scanner should only scan the process allocation or not.</param>
	/// <returns>An initialized MemoryScanner object.</returns>
	/// <exception cref="ParameterException"/>
	MemoryScanner CreateScanner(
		_In_z_ LPCSTR lpString,
		_In_ BOOL bCaseSensitive,
		_In_ BOOL bScanModule = TRUE
	);

	/// <summary>
	/// Creates a memory scanner associated with the current target process. Scans a string.
	/// </summary>
	/// <param name="lpString">- The string to scan.</param>
	/// <param name="bCaseSensitive"><- Defines wether the scan should be case sensitive or not.s/param>
	/// <param name="bScanModule">- Defines wether the scanner should only scan the process allocation or not.</param>
	/// <returns>An initialized MemoryScanner object.</returns>
	/// <exception cref="ParameterException"/>
	MemoryScanner CreateScanner(
		_In_z_ LPCWSTR lpString,
		_In_ BOOL bCaseSensitive,
		_In_ BOOL bScanModule = TRUE
	);

	/// <summary>
	/// Creates a virtual allocation object with the provided parameters.
	/// </summary>
	/// <param name="uAddress">- The address to allocate, if set to 0, the function itself will determine where to allocate.</param>
	/// <param name="uSize">- The number of bytes to allocate.</param>
	/// <param name="Type">- The allocation type/method.</param>
	/// <param name="Protection">- The allocation memory page protection.</param>
	/// <returns>An initialized VirtualAllocation object.</returns>
	VirtualAllocation CreateVirtualAllocation(
		_In_opt_ ADDRESS uAddress = 0,
		_In_ SIZE_T uSize = 1024,
		_In_ AllocationType Type = AllocationType::Commit | AllocationType::Reserve,
		_In_ MemoryProtection Protection = MemoryProtection::ReadWrite
	);

	/// <summary>
	/// Changes the protection of one or more memory pages.
	/// </summary>
	/// <param name="uAddress">- The address to change the protection of.</param>
	/// <param name="uSize">- The number of bytes to change the protection of.</param>
	/// <param name="NewProtection">- The new protection of the page.</param>
	/// <param name="OldProtection">- A pointer to a buffer to receive the previous memory protection constant.</param>
	/// <exception cref="WindowsApiException"/>
	void VirtualProtect(
		_In_ ADDRESS uAddress,
		_In_ SIZE_T uSize,
		_In_ MemoryProtection NewProtection,
		_Out_opt_ MemoryProtection* OldProtection = nullptr
	);

	/// <summary>
	/// Queries information about a memory page.
	/// </summary>
	/// <param name="uAddress">- The address of the memory page.</param>
	/// <param name="lpMbi">- A pointer to a buffer to receive information about the memory page.</param>
	/// <exception cref="ParameterException"/>
	/// <exception cref="WindowsApiException"/>
	void VirtualQuery(
		_In_ ADDRESS uAddress,
		_Out_ LPMBI lpMbi
	);

	/// <summary>
	/// Creates a hook on the API function provided.
	/// </summary>
	/// <param name="lpFunction">- The module where the function is contained.</param>
	/// <param name="lpDetour">- The name of the function to fetch from the export table.</param>
	/// <param name="lpOriginal">- A pointer to call for the original function.</param>
	/// <returns>An initialized hook object.</returns>
	/// <exception cref="NotImplementedException (Type == External)"/>
	Hook CreateHook(
		_In_z_ LPCSTR lpFunction,
		_In_ LPCVOID lpDetour,
		_Out_opt_ LPVOID lpOriginal = nullptr
	);

	/// <summary>
	/// Creates a hook on the target function.
	/// </summary>
	/// <param name="lpTarget">- The target function.</param>
	/// <param name="lpDetour">- The function to replace with.</param>
	/// <param name="lpOriginal">- A pointer to call for the original function.</param>
	/// <returns>An initialized hook object.</returns>
	/// <exception cref="NotImplementedException (Type == External)"/>
	Hook CreateHook(
		_In_ LPCVOID lpTarget,
		_In_ LPCVOID lpDetour,
		_Out_opt_ LPVOID lpOriginal = nullptr
	);

	/// <summary>
	/// Creates a hook on the function at the target address.
	/// </summary>
	/// <param name="uTarget">- The address of the function.</param>
	/// <param name="lpDetour">- The function to replace with.</param>
	/// <param name="lpOriginal">- A pointer to call for the original function.</param>
	/// <returns>An initialized hook object.</returns>
	/// <exception cref="NotImplementedException (Type == External)"/>
	Hook CreateHook(
		_In_ ADDRESS uTarget,
		_In_ LPCVOID lpDetour,
		_Out_opt_ LPVOID lpOriginal = nullptr
	);
};

#endif // !__MEMORY_H__