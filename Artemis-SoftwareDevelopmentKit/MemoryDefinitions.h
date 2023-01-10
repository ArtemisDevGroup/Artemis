//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#ifndef __MEMORY_SHARED_DEFINITIONS_H__
#define __MEMORY_SHARED_DEFINITIONS_H__

#include "Definitions.h"

#include <initializer_list>

#define MAX_POINTER_LENGTH				64							// The maximum number of pointer offsets.
#define MAX_ASSEMBLY_PATCH_SIZE			64							// The maximum number of bytes in an assembly patch.
#define MIN_ADDRESS						0x10000						// The lowest application address.
#define MAX_ADDRESS						0x7FFFFFFFFFFF				// The highest application address.
#define MAX_SCAN_RESULT_COUNT			256							// The maximum number of results in a memory scan.
#define MAX_PATTERN_LENGTH				64							// The maximum pattern character length.
#define MAX_STRING_LENGTH				256							// The maximum string length.

#undef ADDRESS
#undef LPADDRESS
#undef LPCADDRESS

namespace Artemis {

#ifdef _WIN64
	typedef unsigned __int64		ADDRESS,		* LPADDRESS;		// Represents a memory address.
#else
	typedef unsigned __int32		ADDRESS,		* LPADDRESS;		// Represents a memory address.
#endif
	typedef const ADDRESS*			LPCADDRESS;							// Represents a memory address.

	typedef UINT					BASE_OFFSET,	* LPBASE_OFFSET;	// Represents an offset off of the base address.
	typedef const BASE_OFFSET*		LPCBASE_OFFSET;						// Represents an offset off of the base address.

	typedef ADDRESS					POINTER_OFFSET, * LPPOINTER_OFFSET; // Represents a pointer offset.
	typedef const POINTER_OFFSET* LPCPOINTER_OFFSET;					// Represents a pointer offset.

	typedef MEMORY_BASIC_INFORMATION MBI,			* LPMBI;			// An alias to the MEMORY_BASIC_INFORMATION structure.

	/// <summary>
	/// A class representing a list of arguments.
	/// </summary>
	/// <typeparam name="T">- The type of arguments.</typeparam>
	template<typename T>
	class ArgumentList {
		T* lpBuffer;
		INT nCount;

	public:
		/// <summary>
		/// Creates an object using an initializer list.
		/// </summary>
		/// <param name="list">- A list of the arguments.</param>
		ArgumentList(_In_ std::initializer_list<T> list) {
			nCount = list.size();
			lpBuffer = new T[nCount];
			memcpy(lpBuffer, list.begin(), nCount * sizeof(T));
		}

		/// <summary>
		/// Creates an object using a stack array.
		/// </summary>
		/// <typeparam name="nCount">- The number of arguments.</typeparam>
		/// <param name="lpBuffer">- A pointer to the buffer containing the arguments.</param>
		template<int nCount>
		ArgumentList(T(&lpBuffer)[nCount]) {
			this->nCount = nCount;
			this->lpBuffer = new T[nCount];
			memcpy(this->lpBuffer, lpBuffer, nCount * sizeof(T));
		}

		/// <summary>
		/// Creates an object using a heap array.
		/// </summary>
		/// <param name="lpBuffer">- A pointer to the buffer containing the arguments.</param>
		/// <param name="nCount">- The number of arguments.</param>
		ArgumentList(T* lpBuffer, int nCount) {
			this->nCount = nCount;
			this->lpBuffer = new T[nCount];
			memcpy(this->lpBuffer, lpBuffer, nCount * sizeof(T));
		}

		ArgumentList(const ArgumentList<T>& cpy) {
			nCount = cpy.nCount;
			lpBuffer = new T[nCount];
			memcpy(lpBuffer, cpy.lpBuffer, nCount * sizeof(T));
		}

		~ArgumentList() {
			if (lpBuffer) {
				delete[] lpBuffer;
				lpBuffer = nullptr;
			}
		}

		operator T* () { return lpBuffer; }
		operator const T* () const { return lpBuffer; }

		T& operator[](INT nIndex) { return lpBuffer[nIndex]; }
		T operator[](INT nIndex) const { return lpBuffer[nIndex]; }

		/// <summary>
		/// Gets the number of arguments that are in the list.
		/// </summary>
		/// <returns>The number of arguments.</returns>
		INT GetCount() const { return nCount; }
	};

	/// <summary>
	/// A class representing a set of pointer offsets.
	/// </summary>
	/// <typeparam name="ReturnType">- The return type of the pointer.</typeparam>
	template<typename ReturnType>
	class Pointer : public ArgumentList<ADDRESS> { public: using ArgumentList::ArgumentList; };

	/// <summary>
	/// A class representing a base offset aswell as a set of pointer offsets.
	/// </summary>
	/// <typeparam name="ReturnType">- The return type of the pointer.</typeparam>
	template<typename ReturnType>
	class BasePointer : public ArgumentList<ADDRESS> {
		BASE_OFFSET uOffset;

	public:
		/// <summary>
		/// Creates an object using an initializer list.
		/// </summary>
		/// <param name="uBaseOffset">- The base offset of the pointer.</param>
		/// <param name="list">- A list containing the pointer offsets.</param>
		BasePointer(_In_ BASE_OFFSET uBaseOffset, _In_ std::initializer_list<ADDRESS> list) : ArgumentList(list), uOffset(uBaseOffset) {}

		/// <summary>
		/// Constructs an object using a stack array.
		/// </summary>
		/// <typeparam name="nCount">- The number of pointer offsets.</typeparam>
		/// <param name="uBaseOffset">- The base offset of the pointer.</param>
		/// <param name="lpBuffer">- A pointer to a buffer containing the pointer offsets.</param>
		template<int nCount>
		BasePointer(_In_ BASE_OFFSET uBaseOffset, _In_ ADDRESS(&lpBuffer)[nCount]) : ArgumentList(lpBuffer, nCount), uOffset(uBaseOffset) {}

		/// <summary>
		/// Constructs an object using a heap array.
		/// </summary>
		/// <param name="uBaseOffset">- The base offset of the pointer.</param>
		/// <param name="lpBuffer">- A pointer to a buffer containing the pointer offsets.</param>
		/// <param name="nCount">- The number of pointer offsets.</param>
		BasePointer(_In_ BASE_OFFSET uBaseOffset, _In_ ADDRESS* lpBuffer, _In_ int nCount) : ArgumentList(lpBuffer, nCount), uOffset(uBaseOffset) {}

		/// <summary>
		/// Gets the base offset of the pointer.
		/// </summary>
		/// <returns>The base offset.</returns>
		BASE_OFFSET GetOffset() const { return uOffset; }
	};

	/// <summary>
	/// A class representing a patch in assembly code.
	/// </summary>
	class AssemblyPatch {
		LPBYTE lpEnable;
		LPBYTE lpDisable;
		INT nCount;

	public:
		/// <summary>
		/// Creates an AssemblyPatch object. 
		/// </summary>
		/// <param name="enable">- The bytes representing the enable section of the assembly code.</param>
		/// <param name="disable">- The bytes representing the disable section of the assembly code.</param>
		inline AssemblyPatch(_In_ const ArgumentList<BYTE>& enable, _In_ const ArgumentList<BYTE>& disable) {
			if (enable.GetCount() != disable.GetCount()) {
				lpEnable = nullptr;
				lpDisable = nullptr;
				nCount = 0;
				return;
			}

			nCount = enable.GetCount();
			lpEnable = new BYTE[nCount];
			lpDisable = new BYTE[nCount];

			memcpy(lpEnable, enable, nCount);
			memcpy(lpDisable, disable, nCount);
		}

		inline AssemblyPatch(const AssemblyPatch& cpy) {
			nCount = cpy.nCount;

			lpEnable = new BYTE[nCount];
			lpDisable = new BYTE[nCount];

			memcpy(lpEnable, cpy.lpEnable, nCount);
			memcpy(lpDisable, cpy.lpDisable, nCount);
		}

		inline ~AssemblyPatch() {
			if (lpEnable) {
				delete[] lpEnable;
				lpEnable = nullptr;
			}

			if (lpDisable) {
				delete[] lpDisable;
				lpDisable = nullptr;
			}
		}

		/// <summary>
		/// Gets a pointer to the enable code.
		/// </summary>
		/// <returns>The enable code.</returns>
		const BYTE* GetEnableCode() const { return lpEnable; }

		/// <summary>
		/// Gets a pointer to the disable code.
		/// </summary>
		/// <returns>The disable code.</returns>
		const BYTE* GetDisableCode() const { return lpDisable; }

		/// <summary>
		/// Gets the size of the code arrays.
		/// </summary>
		/// <returns>The size of the code arrays.</returns>
		const INT GetCount() const { return nCount; }
	};

	/// <summary>
	/// A class representing a patch in assembly code at a certain base address.
	/// </summary>
	class BaseAssemblyPatch : public AssemblyPatch {
		BASE_OFFSET uBaseOffset;

	public:
		/// <summary>
		/// Constructs a BaseAssemblyPatch object.
		/// </summary>
		/// <param name="uBaseOffset">- The base offset to write the code to.</param>
		/// <param name="enable">- The bytes representing the enable section of the assembly code.</param>
		/// <param name="disable">- The bytes representing the disable section of the assembly code.</param>
		inline BaseAssemblyPatch(_In_ BASE_OFFSET uBaseOffset, _In_ const ArgumentList<BYTE>& enable, _In_ const ArgumentList<BYTE>& disable) : AssemblyPatch(enable, disable), uBaseOffset(uBaseOffset) {}

		/// <summary>
		/// Gets the base offset.
		/// </summary>
		/// <returns>The base offset.</returns>
		BASE_OFFSET GetOffset() const { return uBaseOffset; }
	};
}

#endif // !__MEMORY_SHARED_DEFINITIONS_H__