#ifndef __DEF_H__
#define __DEF_H__

#if defined(_M_IX86) && !defined(_X86_)
	#define _X86_
#elif defined(_M_AMD64) && !defined(_AMD64_)
	#define _AMD64_
#elif !defined(_X86_) && !defined(_AMD64_)
	#error No target architecture!
#endif

#include <minwindef.h>

typedef unsigned __int64 QWORD;

#define IN_RANGE(n, lo, hi) ((n >= lo) && (n <= hi))
#define ARRAY_LENGTH(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

#define IS_LOWERCASE(c) IN_RANGE(c, 'a', 'z')
#define IS_UPPERCASE(c) IN_RANGE(c, 'A', 'Z')
#define IS_LETTER(c) (IS_LOWERCASE(c) || IS_UPPERCASE(c))
#define IS_DIGIT(c) IN_RANGE(c, '0', '9')
#define IS_HEX(c) (IS_DIGIT(c) && (IN_RANGE(c, 'A', 'F') || IN_RANGE(c, 'a', 'f')))

#define TO_LOWERCASE(c) (c + '\x20')
#define TO_UPPERCASE(c) (c - '\x20')

#endif // !__DEF_H__