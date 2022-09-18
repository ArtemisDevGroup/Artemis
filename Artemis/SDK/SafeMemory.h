#ifndef __SAFE_MEMORY_H__
#define __SAFE_MEMORY_H__

#ifdef __cplusplus

#include "Def.h"
#include "ErrorApi.h"

/// <summary>
/// Allocates memory on the heap.
/// </summary>
/// <param name="_Size">- The number of bytes to allocate.</param>
/// <returns>A pointer to the allocated memory.</returns>
/// <exception cref="MemoryFullException"/>
void* malloc_s(_In_ size_t _Size);

/// <summary>
/// Allocates memory on the heap and initializes it to 0.
/// </summary>
/// <param name="_Count">- The number of elements to allocate.</param>
/// <param name="_Size">- The size of one element.</param>
/// <returns>A pointer to the allocated memory.</returns>
/// <exception cref="MemoryFullException"/>
void* calloc_s(_In_ size_t _Count, _In_ size_t _Size);

/// <summary>
/// Frees previously allocated memory.
/// </summary>
/// <param name="ppBlock">- A pointer to a pointer containing the address of the allocation.</param>
void free_s(_Inout_ void** ppBlock);

/// <summary>
/// Frees previously allocated memory.
/// </summary>
/// <typeparam name="T">The type of the pointer.</typeparam>
/// <param name="ppBlock">- A pointer to a pointer containing the address of the allocation.</param>
template<typename T>
inline void free_s(_Inout_ T** ppBlock) { free_s((void**)ppBlock); }


#endif // __cplusplus

#endif // !__SAFE_MEMORY_H__