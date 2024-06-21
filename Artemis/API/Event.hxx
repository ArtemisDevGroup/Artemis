#ifndef ARTEMIS_API_EVENT_HXX
#define ARTEMIS_API_EVENT_HXX

#include "Definitions.hxx"

#include <Windows.h>

#include <type_traits>
#include <vector>
#include <functional>

namespace Artemis::API {
	struct event_args {
		bool _Handled;
	};

	template<class _Sender = void, class _Ea = event_args>
		requires(std::is_base_of_v<event_args, _Ea>)
	class event {
		using event_handler = std::function<void(_Sender* _Sender, _Ea* e)>;

		struct _ { event_handler _Handler; uint64_t _Id; };

		std::vector<_> _EventHandlers;
		uint64_t _NextId;

	public:
		event() : _NextId(0) {}

		uint64_t subscribe(event_handler _Handler) {
			this->_EventHandlers.push_back(_ { _Handler, this->_NextId });
			this->_NextId++;
		}

		void unsubscribe(uint64_t _Id) {
			for (auto i = this->_EventHandlers.begin(); i != this->_EventHandlers.end(); std::next(i))
				if (i->_Id == _Id) {
					this->_EventHandlers.erase(i);
					return;
				}
		}

		size_t handler_count() const noexcept {
			return _EventHandlers.size();
		}

		size_t unsubscribe_all() {
			size_t count = this->handler_count();
			this->_EventHandlers.clear();
			return count;
		}

		void invoke(_Sender* _Sender, _Ea* e, bool _DeleteEventArgs) const {
			for (const _& handler : this->_EventHandlers)
				handler._Handler(_Sender, e);

			if (_DeleteEventArgs)
				delete e;
		}
	};
}

#endif // !ARTEMIS_API_EVENT_HXX