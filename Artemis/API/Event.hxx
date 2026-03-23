#ifndef __ARTEMIS_API_EVENT_HXX__
#define __ARTEMIS_API_EVENT_HXX__

#include "Definitions.hxx"

#include <cstdint>		// std::uint64_t, std::size_t
#include <concepts>		// std::derived_from
#include <vector>		// std::vector
#include <functional>	// std::function

namespace Artemis::API {
	/**
	 * @brief The base event args data structure.
	 */
	struct event_args {
		bool _Handled;
	};

	/**
	 * @brief Represents an invocable and subscribable event.
	 * @tparam _Sender The type of object that is intended to invoke this event. Defaults to `void`.
	 * @tparam _Ea The event argument data structure type.
	 */
	template<class _Sender = void, std::derived_from<event_args> _Ea = event_args>
	class event {
		using event_handler = std::function<void(_Sender* _Sender, _Ea* e)>;

		struct _ { event_handler _Handler; std::uint64_t _Id; };

		std::vector<_> _EventHandlers;
		std::uint64_t _NextId;

	public:
		event() : _NextId(0) {}

		/**
		 * @brief Subscribes a handler to the event.
		 * @param[in] _Handler The handler function.
		 * @return An id referencing the registered handler.
		 */
		std::uint64_t subscribe(event_handler _Handler) {
			this->_EventHandlers.push_back(_ { _Handler, this->_NextId });
			return this->_NextId++;
		}

		/**
		 * @brief Unsubscribes a handler from the event by id.
		 * @param[in] _Id The id of the handler to unregister.
		 */
		void unsubscribe(std::uint64_t _Id) {
			for (auto i = this->_EventHandlers.begin(); i != this->_EventHandlers.end(); ++i)
				if (i->_Id == _Id) {
					this->_EventHandlers.erase(i);
					return;
				}
		}

		/**
		 * @brief Gets the number of registered handlers.
		 * @return The number of registered handlers.
		 */
		std::size_t handler_count() const noexcept {
			return _EventHandlers.size();
		}

		/**
		 * @brief Unsubscribes all registered handlers.
		 * @return The number of handlers that were unregistered.
		 */
		std::size_t unsubscribe_all() {
			std::size_t count = this->handler_count();
			this->_EventHandlers.clear();
			return count;
		}

		/**
		 * @brief Invokes the registered handlers.
		 * @param[in] _Sender A pointer to the object invoking this event.
		 * @param[in] e A pointer to the invocation event args.
		 * @param[in] _DeleteEventArgs Declares whether to delete the object pointed to by `e` after invoking the handlers.
		 */
		void invoke(_Sender* _Sender, _Ea* e, bool _DeleteEventArgs) const {
			for (const _& handler : this->_EventHandlers)
				handler._Handler(_Sender, e);

			if (_DeleteEventArgs)
				delete e;
		}
	};
}

#endif // !__ARTEMIS_API_EVENT_HXX__