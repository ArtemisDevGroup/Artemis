#ifndef __STRING_H__
#define __STRING_H__

#ifdef __cplusplus

#include "Def.h"
#include "ErrorApi.h"

#include <string>

/// <summary>
/// Represents an ANSI string.
/// </summary>
class String {
	LPSTR lpStr;
	DWORD dwAllocSize;

	void Alloc(DWORD dwCount);
public:
	String();

	/// <summary>
	/// The copy constructor.
	/// </summary>
	/// <param name="str">- The object to copy.</param>
	/// <exception cref="MemoryFullException"/>
	String(const String& str);

	~String();

	/// <summary>
	/// Constructs a String object and copies contents from the parameter.
	/// </summary>
	/// <param name="str">- The string to copy contents from.</param>
	/// <exception cref="MemoryFullException"/>
	String(_In_ std::string str);

	/// <summary>
	/// Constructs a String object and copies contents from the parameter.
	/// </summary>
	/// <param name="lpString">- The string to copy contents from.</param>
	/// <exception cref="MemoryFullException"/>
	String(_In_z_ LPCSTR lpString);

	/// <summary>
	/// Gets a const pointer to the string buffer.
	/// </summary>
	/// <returns>A pointer to the string.</returns>
	LPCSTR CStr() const;

	/// <summary>
	/// Gets an std::string object representing the contents of this string instance.
	/// </summary>
	/// <returns>An std::string object.</returns>
	std::string StdStr() const;

	/// <summary>
	/// Gets the string length, excluding the null terminator.
	/// </summary>
	/// <returns>The string length.</returns>
	SIZE_T Size() const;

	/// <summary>
	/// Clears the string buffer.
	/// </summary>
	void Clear();

	/// <summary>
	/// Creates a substring of the current string.
	/// </summary>
	/// <param name="nIndex">- The index of the first character in the substring.</param>
	/// <param name="nCount">- The number of characters to copy. The default value (-1) will copy every character after the index into the substring.</param>
	/// <returns>A String object representing the created substring.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="MemoryFullException"/>
	String SubString(_In_ INT nIndex, _In_ INT nCount = -1);

	/// <summary>
	/// Inserts a character into the string represented by this instance at the provided index.
	/// </summary>
	/// <param name="nIndex">- The index to insert at.</param>
	/// <param name="c">- The character to insert.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="MemoryFullException"/>
	void Insert(_In_ INT nIndex, _In_ CHAR c);

	/// <summary>
	/// Inserts a string into the string represented by this instance at the provided index.
	/// </summary>
	/// <param name="nIndex">- The index to insert at.</param>
	/// <param name="lpString">- A pointer to the string to insert.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="MemoryFullException"/>
	void Insert(_In_ INT nIndex, _In_z_ LPCSTR lpString);

	/// <summary>
	/// Inserts a string into the string represented by this instance at the provided index.
	/// </summary>
	/// <param name="nIndex">- The index to insert at.</param>
	/// <param name="str">- The string to insert.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="MemoryFullException"/>
	void Insert(_In_ INT nIndex, _In_ String str);

	/// <summary>
	/// Removes a number of characters from an index.
	/// </summary>
	/// <param name="nIndex">- The index to start removing characters from.</param>
	/// <param name="nCount">- The number of characters to remove.</param>
	/// <exception cref="ParameterInvalidException"/>
	void Remove(_In_ INT nIndex, _In_ INT nCount);

	/// <summary>
	/// Searches for a character and returns the index of it.
	/// </summary>
	/// <param name="c">- The character to search for.</param>
	/// <returns>The index of the character or -1 if not found.</returns>
	INT Find(_In_ CHAR c);

	/// <summary>
	/// Searches for a string and returns the index of it.
	/// </summary>
	/// <param name="lpString">- A pointer to the string to search for.</param>
	/// <returns>The index of the first character in the string or -1 if not found.</returns>
	INT Find(_In_z_ LPCSTR lpString);

	/// <summary>
	/// Searches for a string and returns the index of it.
	/// </summary>
	/// <param name="str">- The string to search for.</param>
	/// <returns>The index of the first character in the string or -1 if not found.</returns>
	INT Find(_In_ String str);

	/// <summary>
	/// Gets a reference to a character in the array.
	/// </summary>
	/// <param name="nIndex">- The index of the character in the array.</param>
	/// <returns>A reference to a character.</returns>
	/// <exception cref="IndexOutOufRangeException"/>
	CHAR& operator[](_In_ INT nIndex);
};

/// <summary>
/// Represents a UTF-16LE string.
/// </summary>
class WString {
	LPWSTR lpStr;
	DWORD dwAllocSize;

	void Alloc(DWORD dwCount);
public:
	WString();

	/// <summary>
	/// The copy constructor.
	/// </summary>
	/// <param name="str">- The object to copy.</param>
	/// <exception cref="MemoryFullException"/>
	WString(const WString& str);

	~WString();

	/// <summary>
	/// Constructs a WString object and copies contents from the parameter.
	/// </summary>
	/// <param name="str">- The string to copy contents from.</param>
	/// <exception cref="MemoryFullException"/>
	WString(_In_ std::wstring str);

	/// <summary>
	/// Constructs a WString object and copies contents from the parameter.
	/// </summary>
	/// <param name="lpString">- The string to copy contents from.</param>
	/// <exception cref="MemoryFullException"/>
	WString(_In_z_ LPCWSTR lpString);

	/// <summary>
	/// Gets a const pointer to the string buffer.
	/// </summary>
	/// <returns>A pointer to the string.</returns>
	LPCWSTR CStr() const;

	/// <summary>
	/// Gets an std::wstring object representing the contents of this string instance.
	/// </summary>
	/// <returns>An std::string object.</returns>
	std::wstring StdStr() const;

	/// <summary>
	/// Gets the string length, excluding the null terminator.
	/// </summary>
	/// <returns>The string length.</returns>
	SIZE_T Size() const;

	/// <summary>
	/// Clears the string buffer.
	/// </summary>
	void Clear();

	/// <summary>
	/// Creates a substring of the current string.
	/// </summary>
	/// <param name="nIndex">- The index of the first character in the substring.</param>
	/// <param name="nCount">- The number of characters to copy. The default value (-1) will copy every character after the index into the substring.</param>
	/// <returns>A String object representing the created substring.</returns>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="MemoryFullException"/>
	WString SubString(_In_ INT nIndex, _In_ INT nCount = -1);

	/// <summary>
	/// Inserts a character into the string represented by this instance at the provided index.
	/// </summary>
	/// <param name="nIndex">- The index to insert at.</param>
	/// <param name="wc">- The character to insert.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="MemoryFullException"/>
	void Insert(_In_ INT nIndex, _In_ WCHAR wc);

	/// <summary>
	/// Inserts a string into the string represented by this instance at the provided index.
	/// </summary>
	/// <param name="nIndex">- The index to insert at.</param>
	/// <param name="lpString">- A pointer to the string to insert.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="MemoryFullException"/>
	void Insert(_In_ INT nIndex, _In_z_ LPCWSTR lpString);

	/// <summary>
	/// Inserts a string into the string represented by this instance at the provided index.
	/// </summary>
	/// <param name="nIndex">- The index to insert at.</param>
	/// <param name="str">- The string to insert.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="MemoryFullException"/>
	void Insert(_In_ INT nIndex, _In_ WString str);

	/// <summary>
	/// Removes a number of characters from an index.
	/// </summary>
	/// <param name="nIndex">- The index to start removing characters from.</param>
	/// <param name="nCount">- The number of characters to remove.</param>
	/// <exception cref="ParameterInvalidException"/>
	void Remove(_In_ INT nIndex, _In_ INT nCount);

	/// <summary>
	/// Searches for a character and returns the index of it.
	/// </summary>
	/// <param name="c">- The character to search for.</param>
	/// <returns>The index of the character or -1 if not found.</returns>
	INT Find(_In_ WCHAR wc);

	/// <summary>
	/// Searches for a string and returns the index of it.
	/// </summary>
	/// <param name="lpString">- A pointer to the string to search for.</param>
	/// <returns>The index of the first character in the string or -1 if not found.</returns>
	INT Find(_In_z_ LPCWSTR lpString);

	/// <summary>
	/// Searches for a string and returns the index of it.
	/// </summary>
	/// <param name="str">- The string to search for.</param>
	/// <returns>The index of the first character in the string or -1 if not found.</returns>
	INT Find(_In_ WString str);

	/// <summary>
	/// Gets a reference to a character in the array.
	/// </summary>
	/// <param name="nIndex">- The index of the character in the array.</param>
	/// <returns>A reference to a character.</returns>
	/// <exception cref="IndexOutOufRangeException"/>
	WCHAR& operator[](_In_ INT nIndex);
};

#endif // __cplusplus

#endif // !__STRING_H__