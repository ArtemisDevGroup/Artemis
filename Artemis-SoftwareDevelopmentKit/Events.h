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

	enum class FrameFunctionType { PreCall, PostCall };

	template<typename FN>
	class FramedEvent {};

	template<typename... A>
	class FramedEvent<void(*)(A...)> {
	public:
		typedef void(*LPFUNCTION)(A...);
		typedef void(*LPFRAMEFUNCTION)();

	private:
		LPFUNCTION lpszFunctions[64];
		LPFRAMEFUNCTION lpfnPre;
		LPFRAMEFUNCTION lpfnPost;

	public:
		inline FramedEvent() : lpfnPre(nullptr), lpfnPost(nullptr) { memset(lpszFunctions, 0x00, sizeof(lpszFunctions)); }

		inline void Invoke(A... params) {
			for (INT i = 0; i < 64; i++)
				if (lpszFunctions[i]) {
					if (lpfnPre) lpfnPre();
					lpszFunctions[i](params...);
					if (lpfnPost) lpfnPost();
				}
		}

		inline void SetFrameFunction(_In_ FrameFunctionType fft, _In_ LPFRAMEFUNCTION lpfnFrame) {
			switch (fft) {
			case FrameFunctionType::PreCall:
				lpfnPre = lpfnFrame;
				break;
			case FrameFunctionType::PostCall:
				lpfnPost = lpfnFrame;
				break;
			default:
				return;
			}
		}

		inline void operator+=(_In_ LPFUNCTION lpFunction) {
			for (INT i = 0; i < 64; i++) {
				if (!lpszFunctions[i]) {
					lpszFunctions[i] = lpFunction;
					break;
				}
			}
		}

		inline void operator-=(_In_ LPFUNCTION lpFunction) {
			for (INT i = 0; i < 64; i++) {
				if (lpszFunctions[i] == lpFunction) {
					lpszFunctions[i] = nullptr;
					break;
				}
			}
		}
	};
}

#endif // !__EVENTS_H__