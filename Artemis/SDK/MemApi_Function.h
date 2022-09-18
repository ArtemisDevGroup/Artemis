#ifndef __MEMAPI_FUNCTION_H__
#define __MEMAPI_FUNCTION_H__

#include "Def.h"
#include "ErrorApi.h"

#ifdef __cplusplus

namespace api {

	/// <summary>
	/// Represents a function.
	/// </summary>
	/// <param name="FN">- The function type. Syntax: returntype(opt_convention*)(paramtype1, paramtype2, paramtype3+)<para> Examples: int(*)(), void(__cdecl*)(char*, int)</para></param>
	template<typename FN>
	class Function;

	template<typename R, typename... A>
	class Function<R(*)(A...)> {
		void* lpfn;
	public:
		Function() : lpfn(nullptr) {}
		Function(R(*lpFunction)(A...)) : lpfn((void*)lpFunction) {}
		Function(ADDRESS uAddress) : lpfn((void*)uAddress) {}

		R operator()(A... nParam) {
			__try {
				return ((R(*)(A...))lpfn)(nParam...);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				throw FunctionFailException();
			}
		}
	};

}

#endif // __cplusplus

#endif // !__MEMAPI_FUNCTION_H__