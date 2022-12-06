#include "SafeMemory.h"

namespace Artemis {
	void SafeDelete(_In_ void* pBlock) {
		if (pBlock) {
			__try {
				delete pBlock;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}
	}
}