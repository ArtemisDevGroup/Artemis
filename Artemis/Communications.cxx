#include "pch.h"
#include "Communications.hxx"

#include "API/Exception.hxx"

namespace Artemis {
#pragma region Class message

	message::message(message_type _MessageType) noexcept : _MessageType(_MessageType), _DispatcherName() {}

	void message::set_message_dispatcher_name(const std::string_view& _DispatcherName) noexcept {
		assert(_DispatcherName.size() < sizeof(this->_DispatcherName));

		strcpy_s(this->_DispatcherName, sizeof(this->_DispatcherName), _DispatcherName.data());
	}

	message_type message::type() const noexcept { return this->_MessageType; }
	const char* const message::dispatcher_name() const noexcept { return this->_DispatcherName; }

#pragma endregion

#pragma region Class message_recipent

	message_recipent::message_recipent() noexcept : hPipeInbound(nullptr), pMessageBody(nullptr) {}

	message_recipent::message_recipent(const char* const _MessagePipeName) : pMessageBody(nullptr) {
		__stack_record();

		this->hPipeInbound = CreateFileA(
			_MessagePipeName,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (this->hPipeInbound == INVALID_HANDLE_VALUE)
			throw API::win32_exception("CreateFileA");

		__stack_escape();
	}

	message_recipent::message_recipent(message_recipent&& _Other) noexcept : pMessageBody(nullptr) {
		if (_Other.hPipeInbound && _Other.hPipeInbound != INVALID_HANDLE_VALUE) {
			this->hPipeInbound = _Other.hPipeInbound;
			_Other.hPipeInbound = nullptr;
		}
		else this->hPipeInbound = nullptr;
	}

	message_recipent::~message_recipent() noexcept {
		if (this->hPipeInbound && this->hPipeInbound != INVALID_HANDLE_VALUE) {
			CloseHandle(this->hPipeInbound);
			this->hPipeInbound = nullptr;
		}

		if (this->pMessageBody) {
			delete[] (char*)this->pMessageBody;
			this->pMessageBody = nullptr;
		}
	}

	bool message_recipent::await_message(nullptr_t) {
		__stack_record();

		if (this->pMessageBody) {
			delete[](char*)this->pMessageBody;
			this->pMessageBody = nullptr;
		}

		this->pMessageBody = (message*)new char[MaximumMessageSize];

		if (!ReadFile(this->hPipeInbound, this->pMessageBody, MaximumMessageSize, nullptr, nullptr))
			throw API::win32_exception("ReadFile");

		__stack_escape();
		return (bool)this->pMessageBody->type();
	}

	bool message_recipent::await_message() {
		__stack_record();

		if (this->_OnMessageCallback)
			throw API::invalid_state_exception("Cannot await message due to an on message callback already awaiting a message for this instance.");

		bool result;
		__stack_rethrow(result = this->await_message(nullptr));

		__stack_escape();
		return result;
	}

	message* message_recipent::get_message_body() noexcept { return this->pMessageBody; }

	void message_recipent::set_onmessage_callback(std::function<void(message*)>&& _Callback) {
		__stack_record();

		this->_OnMessageCallback = std::move(_Callback);

		try {
			this->_WatchThread = std::thread([this]() {
				while (this->await_message(nullptr)) {
					message* msg = this->get_message_body();
					if (msg->type() == message_type::thread_exit)
						return;
					this->_OnMessageCallback(msg);
				}
				this->_OnMessageCallback(this->get_message_body());
				});
		}
		catch (const std::exception& e) {
			throw API::exception(e.what());
		}

		__stack_escape();
	}

	message_recipent& message_recipent::operator=(message_recipent&& _Other) noexcept {
		if (_Other.hPipeInbound && _Other.hPipeInbound != INVALID_HANDLE_VALUE) {
			this->hPipeInbound = _Other.hPipeInbound;
			_Other.hPipeInbound = nullptr;
		}
		else this->hPipeInbound = nullptr;

		return *this;
	}

#pragma endregion

#pragma region Class message_dispatcher
	
	message_dispatcher::message_dispatcher(std::string_view&& _DispatcherName) noexcept : hPipeOutbound(nullptr), _DispatcherName(std::move(_DispatcherName)) {}
	// !! ^^ DOES NOT MAKE SURE LEN OF _DispatcherName <= 128

	message_dispatcher::message_dispatcher(std::string_view&& _DispatcherName, const char* const _MessagePipeName) : _DispatcherName(std::move(_DispatcherName)) {
		__stack_record();

		if (_DispatcherName.size() >= 128)
			throw API::argument_exception("String is longer than the maximum number of allowed characters (128).", "_DispatcherName");

		this->hPipeOutbound = CreateFileA(
			_MessagePipeName,
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (this->hPipeOutbound == INVALID_HANDLE_VALUE)
			throw API::win32_exception("CreateFileA");

		__stack_escape();
	}

	message_dispatcher::message_dispatcher(message_dispatcher&& _Other) noexcept : _DispatcherName(std::move(_Other._DispatcherName)) {
		if (_Other.hPipeOutbound && _Other.hPipeOutbound != INVALID_HANDLE_VALUE) {
			this->hPipeOutbound = _Other.hPipeOutbound;
			_Other.hPipeOutbound = nullptr;
		}
		else this->hPipeOutbound = nullptr;
	}

	message_dispatcher::~message_dispatcher() noexcept {
		if (this->hPipeOutbound && this->hPipeOutbound != INVALID_HANDLE_VALUE) {
			CloseHandle(this->hPipeOutbound);
			this->hPipeOutbound = nullptr;
		}
	}

	void message_dispatcher::dispatch_message(message* _Message, size_t _Size) {
		__stack_record();

		_Message->set_message_dispatcher_name(this->_DispatcherName);

		if (!WriteFile(this->hPipeOutbound, _Message, (DWORD)_Size, nullptr, nullptr))
			throw API::win32_exception("WriteFile");

		__stack_escape();
	}

	void message_dispatcher::relay_messages_from_recipent(message_recipent* _Recipent) {
		__stack_record();
		__stack_rethrow(_Recipent->set_onmessage_callback([this](message* _Msg) { this->dispatch_message(_Msg); }));
		__stack_escape();
	}

	message_dispatcher& message_dispatcher::operator=(message_dispatcher&& _Other) noexcept {
		this->_DispatcherName = std::move(_Other._DispatcherName);

		if (_Other.hPipeOutbound && _Other.hPipeOutbound != INVALID_HANDLE_VALUE) {
			this->hPipeOutbound = _Other.hPipeOutbound;
			_Other.hPipeOutbound = nullptr;
		}
		else this->hPipeOutbound = nullptr;

		return *this;
	}

#pragma endregion

	std::pair<message_dispatcher*, message_recipent*> create_anonymous_pipeline(std::string_view&& _DispatcherName) {
		__stack_record();

		message_dispatcher* dispatcher = new message_dispatcher(std::move(_DispatcherName));
		message_recipent* recipent = new message_recipent();

		if (!CreatePipe(
			&recipent->hPipeInbound,
			&dispatcher->hPipeOutbound,
			nullptr,
			MaximumMessageSize
		)) {
			DWORD dwLastError = GetLastError();
			delete dispatcher;
			delete recipent;
			throw API::win32_exception(dwLastError, "CreatePipe");
		}

		__stack_escape();
		return std::make_pair(dispatcher, recipent);
	}
}