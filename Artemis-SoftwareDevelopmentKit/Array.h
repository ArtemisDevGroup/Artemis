//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @iA-blip and @Sigma0014.									|
// @iA-blip: https://github.com/iA-blip													|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <string.h>
#include <initializer_list>

#include "Definitions.h"
#include "Exceptions.h"
#include "Interfaces.h"

namespace Artemis {
	/// <summary>
	/// A class for dynamically allocating and interacting with an element list.
	/// </summary>
	/// <typeparam name="T">- The element type.</typeparam>
	template<typename T>
	class List : public IDisposable {
	private:
		T* lpAlloc;
		INT nCount;

		inline void Allocate(_In_ INT nCount) {
			if (lpAlloc) {
				T* lpOldMem = lpAlloc;
				int nOldSize = this->nCount;

				this->nCount = nCount;
				lpAlloc = new T[nCount];

				size_t uSize = nOldSize < nCount ? nOldSize : nCount;
				uSize *= sizeof(T);

				memcpy(lpAlloc, lpOldMem, uSize);

				delete[] lpOldMem;
			}
			else {
				this->nCount = nCount;
				lpAlloc = new T[nCount];
			}
		}

	public:
		inline List() : lpAlloc(nullptr), nCount(0) {}

		inline List(const List<T>& cpy) : lpAlloc(nullptr) {
			Allocate(cpy.nCount);
			memcpy(lpAlloc, cpy.lpAlloc, nCount * sizeof(T));
		}

		/// <summary>
		/// Constructs a List object and copies memory from the array pointed to.
		/// </summary>
		/// <param name="lpArray">- A pointer to a previously allocated array of items.</param>
		/// <param name="nCount">- The number of elements to copy from the previous array.</param>
		inline List(
			_In_reads_(nCount) T* lpArray,
			_In_ INT nCount
		) : lpAlloc(nullptr) {
			Allocate(nCount);
			memcpy(lpAlloc, lpArray, nCount * sizeof(T));
		}

		/// <summary>
		/// Constructs a List object and copies memory from the array referenced.
		/// </summary>
		/// <typeparam name="nCount">- The number of elements to copy from the previous array.</typeparam>
		/// <param name="szArray">- A reference to a previously allocated array of items.</param>
		template<INT nCount>
		inline List(_In_reads_(nCount) T(&szArray)[nCount]) : lpAlloc(nullptr) {
			Allocate(nCount);
			memcpy(lpAlloc, szArray, nCount * sizeof(T));
		}

		/// <summary>
		/// Constructs a List object and copies memory from the provided initializer list.
		/// </summary>
		/// <param name="InitList">- The initializer list to copy from.</param>
		inline List(_In_ std::initializer_list<T> InitList) : lpAlloc(nullptr) {
			Allocate(InitList.size());
			memcpy(lpAlloc, InitList.begin(), InitList.size() * sizeof(T));
		}

		/// <summary>
		/// Adds a single element to the list.
		/// </summary>
		/// <param name="Element">- The element to add.</param>
		/// <param name="nIndex">- The index to add at. -1 means appending to the end of the list.</param>
		/// <exception cref="IndexOutOfRangeException"/>
		inline void Add(
			_In_ T Element,
			_In_ INT nIndex = -1
		) {
			if (nIndex > -1 && nIndex >= this->nCount) throw IndexOutOfRangeException(nIndex, this->nCount - 1);

			Allocate(nCount + 1);

			if (nIndex <= -1)
				lpAlloc[nCount - 1] = Element;
			else {
				for (int i = nCount - 1; i >= nIndex; i--)
					lpAlloc[i] = lpAlloc[i - 1];

				lpAlloc[nIndex] = Element;
			}
		}

		/// <summary>
		/// Adds an array of elements to the list.
		/// </summary>
		/// <param name="lpElements">- A pointer to the array of elements.</param>
		/// <param name="nCount">- The number of elements in the array.</param>
		/// <param name="nIndex">- The index to add at. -1 means appending to the end of the list.</param>
		/// <exception cref="IndexOutOfRangeException"/>
		inline void Add(
			_In_reads_(nCount) T* lpElements,
			_In_ INT nCount,
			_In_ INT nIndex
		) {
			if (nIndex >= this->nCount) throw IndexOutOfRangeException(nIndex, this->nCount - 1);

			Allocate(this->nCount + nCount);

			if (nIndex <= -1)
				for (int i = 0; i < nCount; i++)
					lpAlloc[(this->nCount - nCount) + i] = lpElements[i];
			else {
				for (int i = this->nCount - 1; i >= nIndex; i--)
					lpAlloc[i] = lpAlloc[i - nCount];

				for (int i = nIndex; i < nIndex + nCount; i++)
					lpAlloc[i] = lpElements[i - nIndex];
			}
		}

		/// <summary>
		/// Adds an array of elements to the list.
		/// </summary>
		/// <typeparam name="nCount">- The number of elements in the array.</typeparam>
		/// <param name="szArray">- A reference to the array of elements.</param>
		/// <param name="nIndex">- The index to add at. -1 means appending to the end of the list.</param>
		/// <exception cref="IndexOutOfRangeException"/>
		template<INT nCount>
		inline void Add(
			_In_reads_(nCount) T(&szArray)[nCount],
			_In_ INT nIndex
		) {
			if (nIndex >= this->nCount) throw IndexOutOfRangeException(nIndex, this->nCount - 1);

			Allocate(this->nCount + nCount);

			if (nIndex <= -1)
				for (int i = 0; i < nCount; i++)
					lpAlloc[(this->nCount - nCount) + i] = szArray[i];
			else {
				for (int i = this->nCount - 1; i >= nIndex; i--)
					lpAlloc[i] = lpAlloc[i - nCount];

				for (int i = nIndex; i < nIndex + nCount; i++)
					lpAlloc[i] = szArray[i - nIndex];
			}
		}

		/// <summary>
		/// Adds an intializer list with elements to the array.
		/// </summary>
		/// <param name="InitList">- The list of elements to add.</param>
		/// <param name="nIndex">- The index to add at. -1 means appending to the end of the list.</param>
		/// <exception cref="IndexOutOfRangeException"/>
		inline void Add(
			_In_ std::initializer_list<T> InitList,
			_In_ INT nIndex = -1
		) {
			if (nIndex >= this->nCount) throw IndexOutOfRangeException(nIndex, this->nCount - 1);

			Allocate(this->nCount + InitList.size());

			if (nIndex <= -1)
				for (int i = 0; i < InitList.size(); i++)
					lpAlloc[(this->nCount - InitList.size()) + i] = InitList.begin()[i];
			else {
				for (int i = this->nCount - 1; i >= nIndex; i--)
					lpAlloc[i] = lpAlloc[i - InitList.size()];

				for (int i = nIndex; i < nIndex + InitList.size(); i++)
					lpAlloc[i] = InitList.begin()[i - nIndex];
			}
		}

		/// <summary>
		/// Removes a number of elements from the list.
		/// </summary>
		/// <param name="nCount">- The number of elements to remove.</param>
		/// <param name="nIndex">- The index to remove at. -1 removes from the end of the list. nCount HAS to be 1 in that instance.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="IndexOutOfRangeException"/>
		inline void Remove(
			_In_ INT nCount = 1,
			_In_ INT nIndex = -1
		) {
			if (nCount < 1) throw ParameterException("nCount");
			if (nIndex <= -1 && nCount != 1) throw IndexOutOfRangeException(this->nCount - 1, this->nCount - nCount);
			if (nIndex + nCount > this->nCount) throw IndexOutOfRangeException(nIndex, this->nCount - nCount);

			if (nIndex <= -1) {
				Allocate(this->nCount - nCount);
			}
			else {
				for (int i = 0; i < nCount; i++) {
					for (int i = nIndex; i < this->nCount - 1; i++) {
						lpAlloc[i] = lpAlloc[i + 1];
					}
				}

				Allocate(this->nCount - nCount);
			}
		}

		/// <summary>
		/// Frees the memory allocated for the list. Essenitally clearing the list.
		/// </summary>
		inline void Release() {
			if (lpAlloc) {
				delete[] lpAlloc;
				lpAlloc = nullptr;
				nCount = 0;
			}
		}

		/// <summary>
		/// Gets the number of elements in the list.
		/// </summary>
		/// <returns>The number of elements in the list.</returns>
		inline _Check_return_ INT GetCount() const { return this->nCount; }

		/// <summary>
		/// Gets a pointer to the list.
		/// </summary>
		/// <returns>A pointer to the list.</returns>
		inline _Check_return_ _Ret_maybenull_ T* GetPtr() const { return lpAlloc; }

		inline ~List() { Release(); }

		inline T& operator[](_In_ INT nIndex) { return lpAlloc[nIndex]; }
		inline T operator[](_In_ INT nIndex) const { return lpAlloc[nIndex]; }
	};
}

#endif // !__ARRAY_H__