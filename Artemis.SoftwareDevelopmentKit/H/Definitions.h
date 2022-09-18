#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <sal.h>

#ifdef _M_IX86
#define _X86_
#elif defined(_M_AMD64)
#define _AMD64_
#else
#error No target architecture!
#endif

#include <minwindef.h>

#define MAX_NAME				64										// The max characters in a name.
#define MAX_MESSAGE				256										// The max characters in a message.
#define MAX_PATH				260										// The max characters in a system path.

#define IN_RANGE(n, lo, hi) ((n >= lo) && (n <= hi))					// Checks if the value is in the range of two other values.
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))							// Gets the size of an array.
#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)								// Gets the higher nibble of a byte.
#define LO_NIBBLE(b) ((b) & 0x0F)										// Gets the lower nibble of a byte.

#define IS_LOWERCASE(c) IN_RANGE(c, 'a', 'z')							// Checks if a character is lower case.
#define IS_UPPERCASE(c) IN_RANGE(c, 'A', 'Z')							// Checks if a character is upper case.
#define IS_LETTER(c) (IS_LOWERCASE(c) || IS_UPPERCASE(c))				// Checks if a character is a letter.
#define IS_DIGIT(c) IN_RANGE(c, '0', '9')								// Checks if a character is a digit.
#define IS_HEX(c) (IS_DIGIT(c) && (IN_RANGE(c, 'A', 'F') || IN_RANGE(c, 'a', 'f'))) // Checks if a character is a hexadecimal digit.

#define TO_LOWERCASE(c) (c + '\x20')									// Converts an upper case letter to a lower case one.
#define TO_UPPERCASE(c) (c - '\x20')									// Converts a lower case character to an upper case one.

#endif // !__DEFINITIONS_H__