#include "SafeMemory.h"

#include <stdlib.h>

void* malloc_s(_In_ size_t _Size) {
	void* ret = malloc(_Size);
	if (!ret) throw api::MemoryFullException();
	return ret;
}

void* calloc_s(_In_ size_t _Count, _In_ size_t _Size) {
	void* ret = calloc(_Count, _Size);
	if (!ret) throw api::MemoryFullException();
	return ret;
}

void* pointers[32];
int currentIndex = 0;

void free_s(_Inout_ void** ppBlock) {
	if (!*ppBlock) return;

	if (currentIndex == 31) {
		memset(pointers, 0x00, sizeof(pointers));
		currentIndex = 0;
	}

	for (int i = 0; i < 32; i++) {
		if (pointers[i] == *ppBlock) return;
	}

	free(*ppBlock);
	pointers[currentIndex] = *ppBlock;
	*ppBlock = nullptr;
}