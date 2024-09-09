#ifndef ARTEMIS_COMMUNICATIONS_HXX

#include "Definitions.hxx"

#include <Windows.h>	// HANDLE

#include <type_traits>	// std::is_base_of_v
#include <string_view>  // std::string_view
#include <thread>		// std::thread

namespace Artemis {
	constexpr const char* ClientInboundPipeName = "\\\\.\\pipe\\ArtemisClientInboundMessagePipe";
	constexpr const char* ClientOutboundPipeName = "\\\\.\\pipe\\ArtemisClientOutboundMessagePipe";

	constexpr int MaximumMessageSize = 1024;
	
	enum class message_type {
		exit
	};

	class message {
		message_type _MessageType;
		char _DispatcherName[128];

	public:
		ARTEMIS_FRAMEWORK message(message_type _MessageType) noexcept;

		ARTEMIS_FRAMEWORK void set_message_dispatcher_name(const std::string_view& _DispatcherName) noexcept;

		ARTEMIS_FRAMEWORK message_type type() const noexcept;
		ARTEMIS_FRAMEWORK const char* const dispatcher_name() const noexcept;
	};

	static_assert(sizeof(message) <= MaximumMessageSize, "Message struct is too large.");

	template<typename T>
	concept derived_message_type = std::is_base_of_v<message, T>;

	namespace _ {
		class __debug_message_host {
			HANDLE hServerInbound;
			HANDLE hServerOutbound;

		public:
			inline __debug_message_host() {
				this->hServerInbound = CreateNamedPipeA(
					ClientOutboundPipeName,
					PIPE_ACCESS_INBOUND,
					PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
					1,
					MaximumMessageSize,
					MaximumMessageSize,
					NULL,
					nullptr
				);

				this->hServerOutbound = CreateNamedPipeA(
					ClientInboundPipeName,
					PIPE_ACCESS_OUTBOUND,
					PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
					1,
					MaximumMessageSize,
					MaximumMessageSize,
					NULL,
					nullptr
				);
			}

			inline ~__debug_message_host() {
				CloseHandle(this->hServerInbound);
				CloseHandle(this->hServerOutbound);
			}
		};
	}

	class message_dispatcher;

	class message_recipent {
		HANDLE hPipeInbound;
		message* pMessageBody;

	public:
		ARTEMIS_FRAMEWORK message_recipent() noexcept;

		ARTEMIS_FRAMEWORK message_recipent(const char* const _MessagePipeName);

		message_recipent(const message_recipent&) = delete;
		ARTEMIS_FRAMEWORK message_recipent(message_recipent&&) noexcept;

		ARTEMIS_FRAMEWORK ~message_recipent() noexcept;

		ARTEMIS_FRAMEWORK bool await_message();

		ARTEMIS_FRAMEWORK message* get_message_body() noexcept;

		template<derived_message_type T>
		inline T* get_message_body() noexcept { return (T*)this->get_message_body(); }

		message_recipent& operator=(const message_recipent&) = delete;
		ARTEMIS_FRAMEWORK message_recipent& operator=(message_recipent&&) noexcept;

		ARTEMIS_FRAMEWORK friend std::pair<message_dispatcher*, message_recipent*> create_anonymous_pipeline(std::string_view&& _DispatcherName);
	};

	class message_dispatcher {
		HANDLE hPipeOutbound;
		std::string_view _DispatcherName;

		std::thread _RelayThread;

	public:
		ARTEMIS_FRAMEWORK message_dispatcher(std::string_view&& _DispatcherName) noexcept;

		ARTEMIS_FRAMEWORK message_dispatcher(std::string_view&& _DispatcherName, const char* const _MessagePipeName);

		message_dispatcher(const message_dispatcher&) = delete;
		ARTEMIS_FRAMEWORK message_dispatcher(message_dispatcher&&) noexcept;

		ARTEMIS_FRAMEWORK ~message_dispatcher() noexcept;

		ARTEMIS_FRAMEWORK void dispatch_message(message* _Message, size_t _Size);

		inline void dispatch_message(message* _Message) { this->dispatch_message(_Message, sizeof(message)); }

		template<derived_message_type T>
		inline void dispatch_message(T* _Message) {
			static_assert(sizeof(T) <= MaximumMessageSize);
			this->dispatch_message((message*)_Message, sizeof(T));
		}

		ARTEMIS_FRAMEWORK void relay_messages_from_recipent(message_recipent* _Recipent);

		message_dispatcher& operator=(const message_dispatcher&) = delete;
		message_dispatcher& operator=(message_dispatcher&&) noexcept;

		ARTEMIS_FRAMEWORK friend std::pair<message_dispatcher*, message_recipent*> create_anonymous_pipeline(std::string_view&& _DispatcherName);
	};

	ARTEMIS_FRAMEWORK std::pair<message_dispatcher*, message_recipent*> create_anonymous_pipeline(std::string_view&& _DispatcherName);
}

#endif // !ARTEMIS_COMMUNICATIONS_HXX