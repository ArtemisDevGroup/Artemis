#ifndef ARTEMIS_API_EVENT_HXX
#define ARTEMIS_API_EVENT_HXX

#include "Definitions.hxx"

#include <Windows.h>

#include <type_traits>
#include <vector>
#include <functional>
#include <tuple>

namespace Artemis::API {
	struct EventArgs {
		BOOL bHandled;
	};

	template<typename T>
	concept EventArgsType = std::is_base_of_v<EventArgs, T>;

	template<EventArgsType InstanceEventArgs = EventArgs>
	class Event {
		using EventHandler = std::function<VOID(LPVOID lpSender, EventArgs& e)>;

		static constexpr INT nMaxHandlerCount = 64;

		BOOL rgbOccupied[nMaxHandlerCount];
		EventHandler rgEventHandlers[nMaxHandlerCount];

	public:
		Event() {
			ZeroMemory(rgbOccupied, sizeof(rgbOccupied));
		}

		DWORD Subscribe(_In_ EventHandler lpHandler) {
			for (DWORD i = 0; i < nMaxHandlerCount; i++)
				if (!rgbOccupied[i]) {
					rgbOccupied[i] = true;
					rgEventHandlers[i] = lpHandler;
					return i;
				}

			return -1;
		}

		VOID Unsubscribe(_In_ DWORD dwIndex) {
			if (dwIndex >= nMaxHandlerCount)
				return;

			rgbOccupied[dwIndex] = false;
		}

		_Check_return_ INT GetHandlerCount() noexcept {
			INT nCount = 0;

			for (INT i = 0; i < nMaxHandlerCount; i++)
				if (rgbOccupied[i])
					nCount++;

			return nCount;
		}

		INT UnsubscribeAll() {
			INT nCount = GetHandlerCount();
			ZeroMemory(rgbOccupied, sizeof(rgbOccupied));
			return nCount;
		}

		VOID Invoke(_In_opt_ LPVOID lpSender, _In_ EventArgs* e, _In_ BOOL bDeleteEventArgs) {
			for (INT i = 0; i < nMaxHandlerCount; i++)
				if (rgbOccupied[i])
					rgEventHandlers[i](lpSender, *e);

			if (bDeleteEventArgs)
				delete e;
		}

		VOID Invoke(_In_opt_ LPVOID lpSender, _In_ EventArgs* e) { Invoke(lpSender, e, true); }
	};
}

#endif // !ARTEMIS_API_EVENT_HXX