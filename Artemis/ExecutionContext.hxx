#ifndef __ARTEMIS_EXECUTION_CONTEXT_HXX__
#define __ARTEMIS_EXECUTION_CONTEXT_HXX__

#include "Definitions.hxx"

#include "Extension.hxx"

#include <Windows.h>	// DWORD

namespace Artemis::_ {
	class __execution_context {
	public:
		ARTEMIS_FRAMEWORK static void set(DWORD _ThreadId, extension* _Extension) noexcept;
		ARTEMIS_FRAMEWORK static void set(extension* _Extension) noexcept;

		ARTEMIS_FRAMEWORK static extension* get(DWORD _ThreadId) noexcept;
		ARTEMIS_FRAMEWORK static extension* get() noexcept;

		ARTEMIS_FRAMEWORK static void invalidate(extension* _Extension) noexcept;
	};

	template<typename _Ty>
	struct __contextualized_object {
		extension* _LoadedObjectOwner;
		_Ty _Object;
	};
}

#endif // !__ARTEMIS_EXECUTION_CONTEXT_HXX__