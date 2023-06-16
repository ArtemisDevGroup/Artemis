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

#ifndef __AURORA_ARRAY_H__
#define __AURORA_ARRAY_H__

#include <initializer_list>
#include <stdlib.h>

#include "Definitions.h"
#include "Exceptions.h"

namespace Aurora {
	/// <summary>
	/// <para>Represents a result after the comparison of two values.</para>
	/// <para>This is used as a return value for the compare routine of the collection classes.</para>
	/// <para>See guides or the API reference for usage examples.</para>
	/// </summary>
	enum class CompareResult : A_I32 {
		/// <summary>
		/// Element 1 is less than element 2.
		/// </summary>
		LessThan = -1,

		/// <summary>
		/// Element 1 and element 2 are equal.
		/// </summary>
		Equal = 0,

		/// <summary>
		/// Element 1 is more than element 2.
		/// </summary>
		MoreThan = 1
	};

	/// <summary>
	/// A class for pairing two pieces of data.
	/// </summary>
	/// <typeparam name="Type1">- The type of the first object to store.</typeparam>
	/// <typeparam name="Type2">- The type of the second object to store.</typeparam>
	template<NonVoidType Type1, NonVoidType Type2>
	class Pair {
		Type1 Field1;
		Type2 Field2;

	public:
		constexpr Pair() : Field1(), Field2() {}

		/// <summary>
		/// Constructs a pair of two pieces of data.
		/// </summary>
		/// <param name="First">- The first piece of data of the pair.</param>
		/// <param name="Second">- The second piece of data of the pair.</param>
		constexpr Pair(_In_ const Type1& First, _In_ const Type2& Second) : Field1(First), Field2(Second) {}

		/// <summary>
		/// Gets a constant reference to the first object in the pair.
		/// </summary>
		/// <returns>A reference to the first object.</returns>
		AURORA_NDWR_GET("GetFirst") constexpr const Type1& GetFirst() const noexcept { return Field1; }

		/// <summary>
		/// Gets a reference to the first object in the pair.
		/// </summary>
		/// <returns>A reference to the first object.</returns>
		AURORA_NDWR_GET("GetFirst") constexpr Type1& GetFirst() noexcept { return Field1; }

		/// <summary>
		/// Gets a constant reference to the second object in the pair.
		/// </summary>
		/// <returns>A reference to the second object.</returns>
		AURORA_NDWR_GET("GetSecond") constexpr const Type2& GetSecond() const noexcept { return Field2; }

		/// <summary>
		/// Gets a constant reference to the second object in the pair.
		/// </summary>
		/// <returns>A reference to the second object.</returns>
		AURORA_NDWR_GET("GetSecond") constexpr Type2& GetSecond() noexcept { return Field2; }
	};

	/// <summary>
	/// A class for storing sequences of data.
	/// </summary>
	/// <typeparam name="ElementType">- The type of the elements to store.</typeparam>
	/// <typeparam name="nElementCount">- The number of elements to store.</typeparam>
	template<NonVoidType ElementType, A_I32 nElementCount>
	class Array {
	public:
		/// <summary>
		/// The core array storing the values.
		/// </summary>
		ElementType szArray[nElementCount];

		inline operator _Ret_writes_(nElementCount) ElementType* () noexcept { return szArray; }
		constexpr operator _Ret_writes_(nElementCount) const ElementType* () const noexcept { return szArray; }

		inline ElementType& operator[](A_I32 nIndex) { return szArray[nIndex]; }
		constexpr const ElementType operator[](A_I32 nIndex) const { return szArray[nIndex]; }

		/// <summary>
		/// Gets the size of the array and returns it.
		/// </summary>
		/// <returns>The size of the array.</returns>
		AURORA_NDWR_GET("size") constexpr const A_I32 size() const noexcept { return nElementCount; }

		/// <summary>
		/// Gets the begin iterator.
		/// </summary>
		/// <returns>The begin iterator.</returns>
		AURORA_NDWR_GET("begin") inline _Ret_writes_(nElementCount) ElementType* begin() noexcept { return szArray; }

		/// <summary>
		/// Gets the begin const iterator.
		/// </summary>
		/// <returns>The begin const iterator.</returns>
		AURORA_NDWR_GET("begin") constexpr _Ret_writes_(nElementCount) const ElementType* begin() const noexcept { return szArray; }

		/// <summary>
		/// Ges the end iterator.
		/// </summary>
		/// <returns>The end iterator.</returns>
		AURORA_NDWR_GET("end") inline ElementType* end() noexcept { return &szArray[nElementCount]; }

		/// <summary>
		/// Gets the end const iterator.
		/// </summary>
		/// <returns>The end const iterator.</returns>
		AURORA_NDWR_GET("end") constexpr const ElementType* end() const noexcept { return &szArray[nElementCount]; }

		/// <summary>
		/// A function template for a function used to compare elements in this array and sort them accordingly.
		/// </summary>
		using CompareFunction = Function<CompareResult(const ElementType& refElement1, const ElementType& refElement2)>;

		/// <summary>
		/// Sorts the array according to the routine provided.
		/// </summary>
		/// <param name="lpfnCompareFunction">
		/// - A function pointer or lambda expression of a routine comparing two values from the array at a time and returning a value from the CompareResult enumeration.
		/// </param>
		inline A_VOID Sort(_In_ CompareFunction lpfnCompareFunction) { qsort_s((void*)szArray, nElementCount, sizeof(ElementType), [](void* pContext, const void* pElem1, const void* pElem2) -> int { return (int)((*(CompareFunction*)pContext)(*(ElementType*)pElem1, *(ElementType*)pElem2)); }, &lpfnCompareFunction); }
	};

	/// <summary>
	/// A class for dynamically storing lists of objects.
	/// </summary>
	/// <typeparam name="ElementType">- The type of the elements in the list.</typeparam>
	template<NonVoidType ElementType>
	class List {
		ElementType* pArray;
		A_I32 nCount;
		A_I32 nMaxCount;

		static constexpr inline A_I32 c_nPageSize = 64;

		AURORA_NDWR_PURE("ntob") static constexpr A_I32 ntob(A_I32 n) noexcept { return n * sizeof(ElementType); }
		AURORA_NDWR_PURE("bton") static constexpr A_I32 bton(A_I32 b) noexcept { return b / sizeof(ElementType); }

		AURORA_NDWR_PURE("GetRealAllocCount") static constexpr A_I32 GetRealAllocCount(A_I32 nCount) noexcept {
			return bton(c_nPageSize * (ntob(nCount) % c_nPageSize ? (ntob(nCount) / c_nPageSize + 1) : (ntob(nCount) / c_nPageSize)));
		}

		inline A_VOID Allocate(_In_ A_I32 nCount) {
			if (nCount <= nMaxCount) return;

			A_I32 nAllocCount = GetRealAllocCount(nCount);

			if (pArray) {
				ElementType* pOldArray = pArray;
				A_I32 nOldMaxCount = nMaxCount;

				pArray = new ElementType[nAllocCount];
				nMaxCount = nAllocCount;

#pragma warning(push)
#pragma warning(disable:6386) // Disabling "buffer overrun" warning due to pArray never being overran, it gets checked at line 'this' - 12.
				for (A_I32 i = 0; i < this->nCount; i++) pArray[i] = pOldArray[i];
#pragma warning(pop)
			}
			else {
				pArray = new ElementType[nAllocCount];
				nMaxCount = nAllocCount;
				this->nCount = 0;
			}
		}

		inline A_VOID Move(_In_ A_I32 nFrom, _In_ A_I32 nTo) {
			AuroraContextStart();

			if (nFrom > nTo) {
				for (A_I32 i = 0; i < this->nCount - nFrom; i++) pArray[nTo + i] = pArray[nFrom + i];
			}
			else if (nFrom < nTo) {
				for (A_I32 i = this->nCount - 1; i >= nFrom; i--)
					pArray[nTo - nFrom + i] = pArray[i];
			}
			else AuroraThrow(ParameterInvalidException, "nFrom == nTo");

			AuroraContextEnd();
		}

	public:
		inline List() noexcept : pArray(nullptr), nMaxCount(0), nCount(0) {}

		/// <summary>
		/// Initializes a List object with the provided list of objects.
		/// </summary>
		/// <param name="refList">- An initializer list to construct the list instance with.</param>
		inline List(_In_ const std::initializer_list<ElementType>& refList) {
			Allocate(refList.size());

			for (A_I32 i = 0; i < refList.size(); i++) pArray[i] = refList.begin()[i];

			nCount = refList.size();
		}

		/// <summary>
		/// Initializes a List object with the provided array of objects.
		/// </summary>
		/// <typeparam name="nCount">- The number of elements in the array.</typeparam>
		/// <param name="lpArray">- A reference to the array.</param>
		template<A_I32 nSize>
		inline List(_In_ const ElementType(&lpArray)[nSize]) {
			Allocate(nSize);

			for (A_I32 i = 0; i < nCount; i++) this->pArray[i] = lpArray[i];

			this->nCount = nCount;
		}

		/// <summary>
		/// Initializes a List object with the provided array of objects.
		/// </summary>
		/// <param name="lpArray">- A pointer to the array.</param>
		/// <param name="dwSize">- The size of the array in elements.</param>
		/// <exception cref="ParameterInvalidException"/>
		inline List(_In_ const ElementType* lpArray, _In_ A_DWORD dwSize) {
			AuroraContextStart();

			if (!lpArray) AuroraThrow(ParameterInvalidException, "lpArray");

			Allocate(dwSize);
			for (A_I32 i = 0; i < dwSize; i++) this->pArray[i] = lpArray[i];
			this->nCount = dwSize;

			AuroraContextEnd();
		}

		inline List(const List<ElementType>& cpy) {
			Allocate(cpy.nCount);
			for (A_I32 i = 0; i < cpy.nCount; i++) pArray[i] = cpy.pArray[i];
			nCount = cpy.nCount;
		}

		/// <summary>
		/// Releases the current list of objects from the heap and sets the count to 0, effectively clearing the list.
		/// </summary>
		inline A_VOID Clear() {
			if (pArray) {
				delete[] pArray;
				pArray = nullptr;
				nCount = 0;
			}
		}

		inline ~List() { Clear(); }

		/// <summary>
		/// Gets the size of the array and returns it.
		/// </summary>
		/// <returns>The size of the array.</returns>
		AURORA_NDWR_GET("size") constexpr const A_I32 size() const noexcept { return nCount; }

		/// <summary>
		/// Gets the begin iterator.
		/// </summary>
		/// <returns>The begin iterator.</returns>
		AURORA_NDWR_GET("begin") inline ElementType* begin() noexcept { return pArray; }

		/// <summary>
		/// Gets the begin const iterator.
		/// </summary>
		/// <returns>The begin const iterator.</returns>
		AURORA_NDWR_GET("begin") constexpr const ElementType* begin() const noexcept { return pArray; }

		/// <summary>
		/// Ges the end iterator.
		/// </summary>
		/// <returns>The end iterator.</returns>
		AURORA_NDWR_GET("end") inline ElementType* end() noexcept { return &pArray[nCount]; }

		/// <summary>
		/// Gets the end const iterator.
		/// </summary>
		/// <returns>The end const iterator.</returns>
		AURORA_NDWR_GET("end") constexpr const ElementType* end() const noexcept { return &pArray[nCount]; }

		/// <summary>
		/// Adds an element to the list.
		/// </summary>
		/// <param name="refElement">- A constant reference to the element to append.</param>
		/// <param name="nIndex">- The index of where to add the element. If nIndex is its default value of -1, the function will append the element to the end of the list.</param>
		inline A_VOID Add(_In_ const ElementType& refElement, _In_ A_I32 nIndex = -1) {
			Allocate(nCount + 1);

			if (nIndex == -1) {
				pArray[nCount] = refElement;
				nCount++;
			}
			else {
				Move(nIndex, nIndex + 1);
				pArray[nIndex] = refElement;
				nCount++;
			}
		}

		/// <summary>
		/// Adds a list of elements to the list.
		/// </summary>
		/// <param name="refList">- A constant reference to the list of elements to append.</param>
		/// <param name="nIndex">- The index of where to add the elements. If nIndex is its default value of -1, the function will append the elements to the end of the list.</param>
		inline A_VOID Add(_In_ const std::initializer_list<ElementType>& refList, _In_ A_I32 nIndex = -1) {
			Allocate(nCount + refList.size());

			if (nIndex == -1) {
				for (A_I32 i = 0; i < refList.size(); i++)
					pArray[nCount + i] = refList.begin()[i];

				nCount += refList.size();
			}
			else {
				Move(nIndex, nIndex + refList.size());
				for (A_I32 i = 0; i < refList.size(); i++)
					pArray[nIndex + i] = refList.begin()[i];

				nCount += refList.size();
			}
		}

		/// <summary>
		/// Adds a list of elements to the list.
		/// </summary>
		/// <param name="refList">- A constant reference to the list of elements to append.</param>
		/// <param name="nIndex">- The index of where to add the elements. If nIndex is its default value of -1, the function will append the elements to the end of the list.</param>
		inline A_VOID Add(_In_ const List<ElementType>& refList, _In_ A_I32 nIndex = -1) {
			Allocate(nCount + refList.size());

			if (nIndex == -1) {
				for (A_I32 i = 0; i < refList.size(); i++)
					pArray[nCount + i] = refList.begin()[i];

				nCount += refList.size();
			}
			else {
				Move(nIndex, nIndex + refList.size());
				for (A_I32 i = 0; i < refList.size(); i++)
					pArray[nIndex + i] = refList.begin()[i];

				nCount += refList.size();
			}
		}

		/// <summary>
		/// Adds an array of elements to the list.
		/// </summary>
		/// <param name="lpArray">- A pointer to the array to append.</param>
		/// <param name="dwSize">- The size of the array in elements.</param>
		/// <param name="nIndex">- The index of where to add the elements. If nIndex is its default value of -1, the function will append the elements to the end of the list.</param>
		/// <exception cref="ParameterInvalidException"/>
		inline A_VOID Add(_In_ const ElementType* lpArray, _In_ A_DWORD dwSize, _In_ A_I32 nIndex = -1) {
			AuroraContextStart();

			if (!lpArray) AuroraThrow(ParameterInvalidException, "lpArray");

			Allocate(this->nCount + dwSize);

			if (nIndex == -1) {
				for (int i = 0; i < dwSize; i++)
					this->pArray[this->nCount + i] = lpArray[i];

				this->nCount += dwSize;
			}
			else {
				Move(nIndex, nIndex + dwSize);
				for (int i = 0; i < dwSize; i++)
					this->pArray[nIndex + i] = lpArray[i];

				this->nCount += dwSize;
			}

			AuroraContextEnd();
		}

		/// <summary>
		/// Adds an array of elements to the list.
		/// </summary>
		/// <typeparam name="nSize">- The size of the array in elements.</typeparam>
		/// <param name="lpArray">- A pointer to the array to append.</param>
		/// <param name="nIndex">- The index of where to add the elements. If nIndex is its default value of -1, the function will append the elements to the end of the list.</param>
		template<A_I32 nSize>
		inline A_VOID Add(_In_ const ElementType(&lpArray)[nSize], _In_ A_I32 nIndex) { Add(lpArray, nSize, nIndex); }

		/// <summary>
		/// Removes a number of elements from an index.
		/// </summary>
		/// <param name="nCount">- The number of elements to remove. By default this is set to 1.</param>
		/// <param name="nIndex">- The index of where to remove elements. If nIndex is its default value of -1, the function will remove elements from the end of the list.</param>
		inline A_VOID Remove(_In_ A_I32 nCount = 1, _In_ A_I32 nIndex = -1) {
			if (nIndex == -1) {
				this->nCount -= nCount;
			}
			else {
				Move(nIndex + nCount, nIndex);
				this->nCount -= nCount;
			}
		}

		/// <summary>
		/// A function template for a function used to compare elements in this array and sort them accordingly.
		/// </summary>
		using CompareFunction = Function<CompareResult(const ElementType& refElement1, const ElementType& refElement2)>;

		/// <summary>
		/// Sorts the list according to the routine provided.
		/// </summary>
		/// <param name="lpfnCompareFunction">
		/// - A function pointer or lambda expression of a routine comparing two values from the list at a time and returning a value from the CompareResult enumeration.
		/// </param>
		inline A_VOID Sort(_In_ CompareFunction lpfnCompareFunction) { qsort_s((void*)pArray, nCount, sizeof(ElementType), [](void* pContext, const void* pElem1, const void* pElem2) -> int { return (int)((*(CompareFunction*)pContext)(*(ElementType*)pElem1, *(ElementType*)pElem2)); }, &lpfnCompareFunction); }

		inline operator ElementType* () noexcept { return pArray; }
		constexpr operator const ElementType* () const noexcept { return pArray; }

		AURORA_NDWR_GET("operator[]") inline ElementType& operator[](A_I32 nIndex) {
			return pArray[nIndex];
		}

		AURORA_NDWR_GET("operator[]") constexpr const ElementType& operator[](A_I32 nIndex) const {
			return pArray[nIndex];
		}
	};

	/// <summary>
	/// A class for taking a list of arguments as a function parameter.
	/// </summary>
	/// <typeparam name="ElementType">- The type of the arguments.</typeparam>
	template<NonVoidType ElementType>
	class ArgumentList {
		ElementType* pArray;
		int nCount;

	public:
		inline ArgumentList() noexcept : pArray(nullptr), nCount(0) {}

		/// <summary>
		/// Initializes an ArgumentList object with the provided list of objects.
		/// </summary>
		/// <param name="refList">- An initializer list to construct the argument list instance with.</param>
		inline ArgumentList(_In_ const std::initializer_list<ElementType>& refList) {
			nCount = refList.size();
			if (nCount > 0) {
				pArray = new ElementType[nCount];
				for (A_I32 i = 0; i < nCount; i++) pArray[i] = refList.begin()[i];
			}
		}

		inline ArgumentList(const ArgumentList<ElementType>& cpy) {
			if (cpy.pArray) {
				pArray = new ElementType[cpy.nCount];
				nCount = cpy.nCount;

				for (A_I32 i = 0; i < nCount; i++)
					pArray[i] = cpy.pArray[i];
			}
		}

		inline ~ArgumentList() {
			if (pArray) {
				delete[] pArray;
				pArray = nullptr;
				nCount = 0;
			}
		}

		/// <summary>
		/// Gets the size of the array and returns it.
		/// </summary>
		/// <returns>The size of the array.</returns>
		AURORA_NDWR_GET("size") constexpr const A_I32 size() const noexcept { return nCount; }

		/// <summary>
		/// Gets the begin iterator.
		/// </summary>
		/// <returns>The begin iterator.</returns>
		AURORA_NDWR_GET("begin") inline ElementType* begin() noexcept { return pArray; }

		/// <summary>
		/// Gets the begin const iterator.
		/// </summary>
		/// <returns>The begin const iterator.</returns>
		AURORA_NDWR_GET("begin") constexpr const ElementType* begin() const noexcept { return pArray; }

		/// <summary>
		/// Ges the end iterator.
		/// </summary>
		/// <returns>The end iterator.</returns>
		AURORA_NDWR_GET("end") inline ElementType* end() noexcept { return &pArray[nCount]; }

		/// <summary>
		/// Gets the end const iterator.
		/// </summary>
		/// <returns>The end const iterator.</returns>
		AURORA_NDWR_GET("end") constexpr const ElementType* end() const noexcept { return &pArray[nCount]; }

		AURORA_NDWR_GET("operator ElementType*") inline operator ElementType* () noexcept { return pArray; }
		AURORA_NDWR_GET("operator const ElementType*") constexpr operator const ElementType* () const noexcept { return pArray; }

		AURORA_NDWR_GET("operator[]") inline ElementType& operator[](A_I32 nIndex) { return pArray[nIndex]; }
		AURORA_NDWR_GET("operator[]") constexpr const ElementType operator[](A_I32 nIndex) const { return pArray[nIndex]; }
	};
}

#endif // !__AURORA_ARRAY_H__