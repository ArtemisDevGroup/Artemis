#include "pch.h"
#include "Midnight.hxx"

namespace Artemis {
	midnight* g_DataInstance = nullptr;

	void alloc_midnight() {
		if (!g_DataInstance)
			g_DataInstance = new midnight();
	}
	void release_midnight() {
		if (g_DataInstance) {
			delete g_DataInstance;
			g_DataInstance = nullptr;
		}
	}
}

extern ::Artemis::midnight* const athis = ::Artemis::g_DataInstance;