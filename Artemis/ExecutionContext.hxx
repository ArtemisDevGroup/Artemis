#ifndef __ARTEMIS_EXECUTION_CONTEXT_HXX__
#define __ARTEMIS_EXECUTION_CONTEXT_HXX__

#include "Definitions.hxx"
ARTEMIS_INTERNAL_HEADER

#include "Extension.hxx"

#include <Windows.h>	// DWORD

namespace Artemis::_ {
	class __execution_context {
	public:
		static void set(DWORD _ThreadId, extension* _Extension) noexcept;
		static void set(extension* _Extension) noexcept;

		static extension* get(DWORD _ThreadId) noexcept;
		static extension* get() noexcept;

		static void invalidate(extension* _Extension) noexcept;
	};
}

#endif // !__ARTEMIS_EXECUTION_CONTEXT_HXX__