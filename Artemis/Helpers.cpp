#include "SDK/Logger.h"
#include "SDK/MemApi_Info.h"

namespace env {
	Logger log;
	MODULE mod;

	Logger& LogClass() { return log; };

	bool bRun = true;

	bool IsRunning() { return bRun; }
	void Stop() { bRun = false; }

	void InitMainModule() { XEGetModuleInformation(&mod, nullptr); }
	const LPMODULE GetMainModule() { return &mod; }
}