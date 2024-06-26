#include "pch.h"
#include "Midnight.hxx"

namespace Artemis {
	extern midnight* g_DataInstance;

	midnight* midnight::global() { return g_DataInstance; }
}