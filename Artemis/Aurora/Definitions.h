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

#ifndef __AURORA_DEFINITIONS_H__
#define __AURORA_DEFINITIONS_H__

#include <sal.h>

// ---------- Macro functions ----------

#define MSB(T) ((T)(1 << (sizeof(T) * 8 - 1)))	// Gets the most significant bit of the specified type.
#define LSB(T) ((T)1)							// Gets the least significant bit of the specified type.

// ---------- Import / Export macros ----------

#define AURORA_EXPORT __declspec(dllexport)
#define AURORA_IMPORT __declspec(dllimport)

#ifdef _AURORA_EXPORT
#define AURORA_API AURORA_EXPORT
#else
#define AURORA_API AURORA_IMPORT
#endif

// ---------- SAL annotations ----------

#define _In_a32_ _In_range_(0x10000U, 0x7FFEFFFFU)
#define _In_opt_a32_(n) _When_(_Param_(n) != 0, _In_a32_)

#define _In_a64_ _In_range_(0x10000U, 0x7FFFFFFEFFFFUi64)
#define _In_opt_a64_(n) _When_(_Param_(n) != 0, _In_a64_)

#ifdef _WIN64
#define _In_a_ _In_a64_
#define _In_opt_a_(n) _In_opt_a64_(n)
#else
#define _In_a_ _In_a32_
#define _In_opt_a_(n) _In_opt_a32_(n)
#endif // _WIN64

// ---------- Predefined constants ----------

#ifndef MAX_PATH
// The maximum number of characters in a valid file path.
#define MAX_PATH 260
#endif // !MAX_PATH

#ifndef MAX_NAME
// The maximum number of characters in a name.
#define MAX_NAME 128
#endif // !MAX_NAME

#ifndef MAX_MSG
// The maximum number of characers in a message.
#define MAX_MSG 256
#endif // !MAX_MSG

#ifndef MAX_INVOKE
// The maximum number of invokable objects registered per instance.
#define MAX_INVOKE 64
#endif // !MAX_INVOKE

// The value of an invalid index.
#define INVALID_INDEX (-1)

// ---------- Miscellaneous macros ----------

// Declares that the functions return value or return value of class or enum type shall not be discarded.
#define AURORA_ND [[nodiscard]]

// Declares that the functions return value or return value of class or enum type shall not be discarded. Additionally specifies a reason.
#define AURORA_NDWR(reason) [[nodiscard(reason)]]

#define AURORA_NDWR_GET(name) AURORA_NDWR("Call to getter '" name "' unnecesary if return value is discarded.")
#define AURORA_NDWR_PURE(name) AURORA_NDWR("Call to pure function '" name "' unnecessary if return value is discarded; function has no side effects.")
#define AURORA_NDWR_DISP(name) AURORA_NDWR("Call to function '" name "' returns a disposable resource; discarding return value leads to resource leaks.")
#define AURORA_NDWR_CREATE(name) AURORA_NDWR("Call to function '" name "' constructs and returns an object as its only operation; discarding this value makes operation obsolete.")

// ---------- Generic and no-return types ----------

// A void. Used to indicate a function not having a return value, Eg: 'void foo();'. Also used to indicate that a function does not have any parameters (in legacy C), Eg: 'int foo(void);'
#define A_VOID void
// A void pointer. Generally used as an out pointer of a generic type. Is prefixed as 'lp'.
typedef A_VOID* A_LPVOID;
// A constant void pointer. Generally used as an in pointer of a generic type. Is prefixed as 'lp'.
typedef const A_VOID* A_LPCVOID;

// ---------- Character and string types ----------

// An ANSI character, prefixed as 'c'.
typedef char A_CHAR;
// An ANSI character pointer, prefixed as 'lp'.
typedef char* A_LPCHAR;
// An ANSI character pointer. Used for string buffers and is prefixed as 'lp'.
typedef char* A_LPSTR;
// A constant ANSI character pointer. Used for strings and is prefixed as 'lp'.
typedef const char* A_LPCSTR;

// A UTF-16LE character, prefixed as 'wc'.
typedef wchar_t A_WCHAR;
// A UTF-16LE character pointer, prefixed as 'lp'.
typedef wchar_t* A_LPWCHAR;
// A UTF-16LE character pointer. Used for wide string buffers and is prefixed as 'lp'.
typedef wchar_t* A_LPWSTR;
// A UTF-16LE character pointer. Used for wide strings and is prefixed as 'lp'.
typedef const wchar_t* A_LPCWSTR;

// ---------- Boolean types ----------

// A boolean, prefixed as 'b'.
typedef bool A_BOOL;
// A boolean pointer, prefixed as 'lp'.
typedef bool* A_LPBOOL;
// A constant boolean pointer, prefixed as 'lp'.
typedef const bool* A_LPCBOOL;

// ---------- Signed integral types ----------

// An 8-bit signed integer, prefixed as 'n'.
typedef signed __int8 A_I8;
// A 16-bit signed integer, prefixed as 'n'.
typedef signed __int16 A_I16;
// A 32-bit signed integer, prefixed as 'n'.
typedef signed __int32 A_I32;
// A 64-bit signed integer, prefixed as 'n'.
typedef signed __int64 A_I64;

// An 8-bit signed integer pointer, prefixed as 'lp'.
typedef A_I8* A_LPI8;
// A 16-bit signed integer pointer, prefixed as 'lp'.
typedef A_I16* A_LPI16;
// A 32-bit signed integer pointer, prefixed as 'lp'.
typedef A_I32* A_LPI32;
// A 64-bit signed integer pointer, prefixed as 'lp'.
typedef A_I64* A_LPI64;

// A constant 8-bit signed integer pointer, prefixed as 'lp'.
typedef const A_I8* A_LPCI8;
// A constant 16-bit signed integer pointer, prefixed as 'lp'.
typedef const A_I16* A_LPCI16;
// A constant 32-bit signed integer pointer, prefixed as 'lp'.
typedef const A_I32* A_LPCI32;
// A constant 64-bit signed integer pointer, prefixed as 'lp'.
typedef const A_I64* A_LPCI64;

// ---------- Unsigned integral types ----------

// An 8-bit unsigned integer, prefixed as 'u'.
typedef unsigned __int8 A_U8;
// A 16-bit unsigned integer, prefixed as 'u'.
typedef unsigned __int16 A_U16;
// A 32-bit unsigned integer, prefixed as 'u'.
typedef unsigned __int32 A_U32;
// A 64-bit unsigned integer, prefixed as 'u'.
typedef unsigned __int64 A_U64;

// An 8-bit unsigned integer pointer, prefixed as 'lp'.
typedef A_U8* A_LPU8;
// A 16-bit unsigned integer pointer, prefixed as 'lp'.
typedef A_U16* A_LPU16;
// A 32-bit unsigned integer pointer, prefixed as 'lp'.
typedef A_U32* A_LPU32;
// A 64-bit unsigned integer pointer, prefixed as 'lp'.
typedef A_U64* A_LPU64;

// A constant 8-bit unsigned integer pointer, prefixed as 'lp'.
typedef const A_U8* A_LPCU8;
// A constant 16-bit unsigned integer pointer, prefixed as 'lp'.
typedef const A_U16* A_LPCU16;
// A constant 32-bit unsigned integer pointer, prefixed as 'lp'.
typedef const A_U32* A_LPCU32;
// A constant 64-bit unsigned integer pointer, prefixed as 'lp'.
typedef const A_U64* A_LPCU64;

// ---------- BYTE, WORD, DWORD and QWORD types ----------

// An 8-bit unsigned integer, prefixed as 'u'.
typedef unsigned char A_BYTE;
// A 16-bit unsigned integer, prefixed as 'w'.
typedef unsigned short A_WORD;
// A 32-bit unsigned integer, prefixed as 'dw'.
typedef unsigned long A_DWORD;
// A 64-bit unsigned integer, prefixed as 'qw'.
typedef unsigned long long A_QWORD;

// An 8-bit unsigned integer pointer, prefixed as 'lp'.
typedef A_BYTE* A_LPBYTE;
// A 16-bit unsigned integer pointer, prefixed as 'lp'.
typedef A_WORD* A_LPWORD;
// A 32-bit unsigned integer pointer, prefixed as 'lp'.
typedef A_DWORD* A_LPDWORD;
// A 64-bit unsigned integer pointer, prefixed as 'lp'.
typedef A_QWORD* A_LPQWORD;

// A constant 8-bit unsigned integer pointer, prefixed as 'lp'.
typedef const A_BYTE* A_LPCBYTE;
// A constant 16-bit unsigned integer pointer, prefixed as 'lp'.
typedef const A_WORD* A_LPCWORD;
// A constant 32-bit unsigned integer pointer, prefixed as 'lp'.
typedef const A_DWORD* A_LPCDWORD;
// A constant 64-bit unsigned integer pointer, prefixed as 'lp'.
typedef const A_QWORD* A_LPCQWORD;

// ---------- Floating point types ----------

// A 32-bit floating point, prefixed as 'f'.
typedef float A_FL32;
// A 64-bit floating point, prefixed as 'f'.
typedef double A_FL64;

// A 32-bit floating point pointer, prefixed as 'lp'.
typedef A_FL32* A_LPFL32;
// A 64-bit floating point pointer, prefixed as 'lp'.
typedef A_FL64* A_LPFL64;

// A constant 32-bit floating point pointer, prefixed as 'lp'.
typedef const A_FL32* A_LPCFL32;
// A constant 64-bit floating point pointer, prefixed as 'lp'.
typedef const A_FL64* A_LPCFL64;

// ---------- Integral address types ----------

// An integral representation of an address pointing to a virtual memory location in a 32-bit process. Prefixed as 'u'.
typedef unsigned __int32 A_ADDR32;
// An integral representation of an address pointing to a virtual memory location in a 64-bit process. Prefixed as 'u'.
typedef unsigned __int64 A_ADDR64;

// A pointer to an integral representation of an address pointing to a virtual memory location in a 32-bit process. Prefixed as 'lp'.
typedef A_ADDR32* A_LPADDR32;
// A pointer to an integral representation of an address pointing to a virtual memory location in a 64-bit process. Prefixed as 'lp'.
typedef A_ADDR64* A_LPADDR64;

// A constant pointer to an integral representation of an address pointing to a virtual memory location in a 32-bit process. Prefixed as 'lp'.
typedef const A_ADDR32* A_LPCADDR32;
// A constant pointer to an integral representation of an address pointing to a virtual memory location in a 64-bit process. Prefixed as 'lp'.
typedef const A_ADDR64* A_LPCADDR64;

#ifdef _WIN64
// An integral representation of an address pointing to a virtual memory location in a 64-bit process. Prefixed as 'u'.
typedef A_ADDR64 A_ADDR;
// A pointer to an integral representation of an address pointing to a virtual memory location in a 64-bit process. Prefixed as 'lp'.
typedef A_ADDR64* A_LPADDR;
// A constant pointer to an integral representation of an address pointing to a virtual memory location in a 64-bit process. Prefixed as 'lp'.
typedef const A_ADDR64* A_LPCADDR;
#else
// An integral representation of an address pointing to a virtual memory location in a 32-bit process. Prefixed as 'u'.
typedef A_ADDR32 A_ADDR;
// A pointer to an integral representation of an address pointing to a virtual memory location in a 32-bit process. Prefixed as 'lp'.
typedef A_ADDR32* A_LPADDR;
// A constant pointer to an integral representation of an address pointing to a virtual memory location in a 32-bit process. Prefixed as 'lp'.
typedef const A_ADDR32* A_LPCADDR;
#endif // _WIN64

// ---------- Concepts ----------

#include <type_traits>
#include <concepts>

namespace Aurora {
	// A null class type and object.
	constexpr class NullClass_t {} NullClass;

	// A concept that restrains a Read functions from returning a void to avoid weird compiler errors.
	template<typename T> concept ReadReturnType = !std::is_same<T, void>::value;

	// A concept that restrains a Write functions from accepting a void as a parameter to avoid weird compiler errors.
	template<typename T> concept WriteDataType = !std::is_same<T, void>::value;
	
	// A concept that restrains a type to either one of two address types.
	template<typename T> concept AddressType = std::is_same<T, A_ADDR32>::value || std::is_same<T, A_ADDR64>::value;

	// A concept that restrains a template type from being void.
	template<typename T> concept NonVoidType = !std::is_same<T, void>::value;

	// A concept that restrains a template type to exclusively accepting enumerations.
	template<typename T> concept EnumerationType = std::is_enum<T>::value;

	// A concept that restains a template type to exclusively accepting integrals.
	template<typename T> concept IntegralType = std::is_integral<T>::value;

	// A concept that restrains a template type to exclusively accepting classes.
	template<typename T> concept ClassType = std::is_class<T>::value;

	// A concept that restrains a template type to have full arithmetic capability, excluding modulus.
	template<typename T> concept ArithmeticType = requires(T a) {
		{ a + a } -> std::convertible_to<T>;
		{ a - a } -> std::convertible_to<T>;
		{ a * a } -> std::convertible_to<T>;
		{ a / a } -> std::convertible_to<T>;

		{ a += a } -> std::convertible_to<T>;
		{ a -= a } -> std::convertible_to<T>;
		{ a *= a } -> std::convertible_to<T>;
		{ a /= a } -> std::convertible_to<T>;
	};

	// A concept that restrains a template type to have full arithmetic capability aswell as full bitwise capability.
	template<typename T> concept BitwiseType = ArithmeticType<T> && requires(T a) {
		{ a ^ a } -> std::convertible_to<T>;
		{ a & a } -> std::convertible_to<T>;
		{ a | a } -> std::convertible_to<T>;
		{ a % a } -> std::convertible_to<T>;
		{ a << a } -> std::convertible_to<T>;
		{ a >> a } -> std::convertible_to<T>;
		{ ~a } -> std::convertible_to<T>;

		{ a ^= a } -> std::convertible_to<T>;
		{ a &= a } -> std::convertible_to<T>;
		{ a |= a } -> std::convertible_to<T>;
		{ a %= a } -> std::convertible_to<T>;
	};

	// A struct that determines whether the templated type is a function pointer or not.
	template<typename T> struct IsFunctionPtr { static constexpr bool value = false; };

	// A struct that determines whether the templated type is a function pointer or not.
	template<typename R, typename... A> struct IsFunctionPtr<R(*)(A...)> { static constexpr bool value = true; };

	// A bool that determines whether the templated type is a function pointer or not.
	template<typename T> constexpr bool IsFunctionPtr_v = IsFunctionPtr<T>::value;

	// A concept that restrains a template type to only be a function pointer.
	template<typename T> concept FunctionPtrType = IsFunctionPtr_v<T>;
}

// ---------- Functions ----------

#include <functional>

namespace Aurora {
	/// <summary>
	/// A function pointer or lambda expression.
	/// </summary>
	/// <typeparam name="T">- A function template, syntaxed: ReturnType(ParameterType...).</typeparam>
	template<typename T>
	using Function = std::function<T>;
}

#endif // !__AURORA_DEFINITIONS_H__