#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "../H/Definitions.h"

#include <memory.h>

template<typename FN>
class Event {};

template<typename... A>
class Event<void(*)(A...)> {
public:
	typedef void(*LPFUNCTION)(A...);

private:
	LPFUNCTION lpszFunctions[64];

public:
	inline Event() { memset(lpszFunctions, 0x00, sizeof(lpszFunctions)); }

	inline void Invoke(A... params) {
		for (INT i = 0; i < 64; i++)
			if (lpszFunctions[i])
				lpszFunctions[i](params...);
	}

	inline void operator+=(LPFUNCTION lpFunction) {
		for (INT i = 0; i < 64; i++) {
			if (!lpszFunctions[i]) {
				lpszFunctions[i] = lpFunction;
				break;
			}
		}
	}

	inline void operator-=(LPFUNCTION lpFunction) {
		for (INT i = 0; i < 64; i++) {
			if (lpszFunctions[i] == lpFunction) {
				lpszFunctions[i] = nullptr;
				break;
			}
		}
	}
};

#endif // !__EVENTS_H__