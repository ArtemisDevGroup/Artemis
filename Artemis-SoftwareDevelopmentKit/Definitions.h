//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <sal.h>														// Includes all of the Microsoft Sign Annotation Language macros for annotating parameters and return values throughout Artemis.

#ifdef _M_IX86
#define _X86_															// Forward declares _X86_ for "minwindef.h".
#elif defined(_M_AMD64)
#define _AMD64_															// Forward declares _AMD64_ for "minwindef.h".
#else
#error No target architecture!											// Errors if the architecture is neither x86 or x64.
#endif

#include <minwindef.h>													// Includes most of the Windows API type definitions.

#ifdef _ARTEMIS_EXPORT
	#define ARTEMIS_API __declspec(dllexport)							// Exports the Artemis API.
#else
	#define ARTEMIS_API __declspec(dllimport)							// Imports the Artemis API.
#endif // EXPORT


#define MAX_NAME				64										// The max characters in a name.
#define MAX_MESSAGE				256										// The max characters in a message.
#define MAX_INVOKE				64										// The max number of invokable pointers. Affects the hard limit on things like Event or WindowManager.

#define IN_RANGE(n, lo, hi) ((n >= lo) && (n <= hi))					// Checks if the value is in the range of two other values.
#undef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))							// Gets the size of an array.
#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)								// Gets the higher nibble of a byte.
#define LO_NIBBLE(b) ((b) & 0x0F)										// Gets the lower nibble of a byte.

#define IS_LOWERCASE(c) IN_RANGE(c, 'a', 'z')							// Checks if a character is lower case.
#define IS_UPPERCASE(c) IN_RANGE(c, 'A', 'Z')							// Checks if a character is upper case.
#define IS_LETTER(c) (IS_LOWERCASE(c) || IS_UPPERCASE(c))				// Checks if a character is a letter.
#define IS_DIGIT(c) IN_RANGE(c, '0', '9')								// Checks if a character is a digit.
#define IS_HEX(c) (IS_DIGIT(c) || (IN_RANGE(c, 'A', 'F') || IN_RANGE(c, 'a', 'f'))) // Checks if a character is a hexadecimal digit.

#define TO_LOWERCASE(c) (c + '\x20')									// Converts an upper case letter to a lower case one.
#define TO_UPPERCASE(c) (c - '\x20')									// Converts a lower case character to an upper case one.

#endif // !__DEFINITIONS_H__