//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "Definitions.h"

#include <memory.h>

namespace Artemis {

	template<typename FN>
	class Event {};

	template<typename... A>
	class Event<void(*)(A...)> {
	public:
		typedef void(*LPFUNCTION)(A...);

	private:
		LPFUNCTION lpszFunctions[MAX_INVOKE];

	public:
		inline Event() { memset(lpszFunctions, 0x00, sizeof(lpszFunctions)); }

		inline void Invoke(A... params) const {
			for (INT i = 0; i < MAX_INVOKE; i++)
				if (lpszFunctions[i])
					lpszFunctions[i](params...);
		}

		inline void operator+=(_In_ LPFUNCTION lpFunction) {
			for (INT i = 0; i < MAX_INVOKE; i++) {
				if (!lpszFunctions[i]) {
					lpszFunctions[i] = lpFunction;
					break;
				}
			}
		}

		inline void operator-=(_In_ LPFUNCTION lpFunction) {
			for (INT i = 0; i < MAX_INVOKE; i++) {
				if (lpszFunctions[i] == lpFunction) {
					lpszFunctions[i] = nullptr;
					break;
				}
			}
		}
	};
}

#endif // !__EVENTS_H__