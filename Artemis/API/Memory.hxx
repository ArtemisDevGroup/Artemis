#ifndef ARTEMIS_API_MEMORY_HXX
#define ARTEMIS_API_MEMORY_HXX

#include "Definitions.hxx"

#include <Windows.h>

namespace Artemis::API {
	namespace RT {
		_Success_(return) ARTEMIS_API BOOL MemoryCopy_Unsafe(_In_ SIZE_T cb, _Out_writes_bytes_(cb) LPVOID lpDestination, _In_reads_bytes_(cb) LPCVOID lpSource);
		_Success_(return) ARTEMIS_API BOOL MemoryCopy_SEH(_In_ SIZE_T cb, _Out_writes_bytes_(cb) LPVOID lpDestination, _In_reads_bytes_(cb) LPCVOID lpSource);
	}

	namespace Impl {

	}

	namespace SEH {

	}
}

#endif // !ARTEMIS_API_MEMORY_HXX