#include "pch.h"
#include "ExecutionContext.hxx"

namespace Artemis::_ {
	static std::unordered_map<DWORD, extension*> g_ThreadExecutionContext;

	void __execution_context::set(DWORD _ThreadId, extension* _Extension) noexcept {
		g_ThreadExecutionContext.insert_or_assign(_ThreadId, _Extension);
	}

	void __execution_context::set(extension* _Extension) noexcept {
		set(GetCurrentThreadId(), _Extension);
	}

	extension* __execution_context::get(DWORD _ThreadId) noexcept {
		if (!g_ThreadExecutionContext.contains(_ThreadId))
			return nullptr;

		return g_ThreadExecutionContext.at(_ThreadId);
	}

	extension* __execution_context::get() noexcept {
		return get(GetCurrentThreadId());
	}

	void __execution_context::invalidate(extension* _Extension) noexcept {
		while (true) {
			for (auto it = g_ThreadExecutionContext.begin(); ; ++it) {
				if (it == g_ThreadExecutionContext.end())
					return;
				
				if (it->second == _Extension) {
					g_ThreadExecutionContext.erase(it);
					break;
				}
			}
		}
	}
}