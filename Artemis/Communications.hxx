#ifndef __ARTEMIS_COMMUNICATIONS_HXX__
#define __ARTEMIS_COMMUNICATIONS_HXX__

#include "Definitions.hxx"

#include <Windows.h>	// HANDLE

#include <concepts>		// std::derived_from
#include <string>		// std::string
#include <string_view>  // std::string_view
#include <thread>		// std::thread
#include <functional>	// std::function

namespace Artemis {
	constexpr std::string_view ClientInboundPipeName = "\\\\.\\pipe\\ArtemisClientInboundMessagePipe";
	constexpr std::string_view ClientOutboundPipeName = "\\\\.\\pipe\\ArtemisClientOutboundMessagePipe";

	constexpr int MaximumMessageSize = 1024;
	
	enum class message_type {
		exit,
		thread_exit
	};

	class message {
		message_type _MessageType;
		char _DispatcherName[128];

	public:
		ARTEMIS_FRAMEWORK message(message_type _MessageType) noexcept;

		ARTEMIS_FRAMEWORK void set_message_dispatcher_name(std::string_view _DispatcherName) noexcept;

		ARTEMIS_FRAMEWORK message_type type() const noexcept;
		ARTEMIS_FRAMEWORK const char* const dispatcher_name() const noexcept;
	};

	static_assert(sizeof(message) <= MaximumMessageSize, "Message struct is too large.");

	class message_dispatcher;

	class message_recipent {
		HANDLE hPipeInbound;
		message* pMessageBody;

		std::thread _WatchThread;
		std::function<void(message*)> _OnMessageCallback;

		ARTEMIS_FRAMEWORK bool await_message(nullptr_t _InternalTransaction);

	public:
		ARTEMIS_FRAMEWORK message_recipent() noexcept;

		ARTEMIS_FRAMEWORK message_recipent(const char* const _MessagePipeName);

		message_recipent(const message_recipent&) = delete;
		ARTEMIS_FRAMEWORK message_recipent(message_recipent&&) noexcept;

		ARTEMIS_FRAMEWORK ~message_recipent() noexcept;

		ARTEMIS_FRAMEWORK bool await_message();

		ARTEMIS_FRAMEWORK message* get_message_body() noexcept;

		template<std::derived_from<message> _Ty>
		inline _Ty* get_message_body() noexcept { return static_cast<_Ty*>(this->get_message_body()); }

		ARTEMIS_FRAMEWORK void set_onmessage_callback(std::function<void(message*)>&& _Callback);

		message_recipent& operator=(const message_recipent&) = delete;
		ARTEMIS_FRAMEWORK message_recipent& operator=(message_recipent&&) noexcept;

		friend std::pair<message_dispatcher*, message_recipent*> create_anonymous_pipeline(std::string_view _DispatcherName);
	};

	class message_dispatcher {
		HANDLE hPipeOutbound;
		std::string _DispatcherName;

	public:
		ARTEMIS_FRAMEWORK message_dispatcher(std::string_view _DispatcherName) noexcept;

		ARTEMIS_FRAMEWORK message_dispatcher(std::string_view _DispatcherName, std::string_view _MessagePipeName);

		message_dispatcher(const message_dispatcher&) = delete;
		ARTEMIS_FRAMEWORK message_dispatcher(message_dispatcher&&) noexcept;

		ARTEMIS_FRAMEWORK ~message_dispatcher() noexcept;

		ARTEMIS_FRAMEWORK void dispatch_message(message* _Message, size_t _Size);

		template<std::derived_from<message> _Ty>
		inline void dispatch_message(_Ty* _Message) {
			static_assert(sizeof(_Ty) <= MaximumMessageSize);
			this->dispatch_message(_Message, sizeof(_Ty));
		}

		ARTEMIS_FRAMEWORK void relay_messages_from_recipent(message_recipent* _Recipent);

		message_dispatcher& operator=(const message_dispatcher&) = delete;
		ARTEMIS_FRAMEWORK message_dispatcher& operator=(message_dispatcher&&) noexcept;

		friend std::pair<message_dispatcher*, message_recipent*> create_anonymous_pipeline(std::string_view _DispatcherName);
	};

	ARTEMIS_FRAMEWORK std::pair<message_dispatcher*, message_recipent*> create_anonymous_pipeline(std::string_view _DispatcherName);
}

#endif // !__ARTEMIS_COMMUNICATIONS_HXX__