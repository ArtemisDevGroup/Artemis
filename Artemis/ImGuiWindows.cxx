#include "pch.h"
#include "ImGuiWindows.hxx"

#include "ExceptionFrames.hxx"

namespace Artemis {
	bool HideAllWindows;

#pragma region Class iwindow

	iwindow::iwindow(std::string_view&& _WindowName) noexcept : IsWindowVisible(true), _WindowName(std::move(_WindowName)) {}

	void iwindow::present() noexcept {
		if (!HideAllWindows && this->IsWindowVisible) {
			if (ImGui::Begin(this->_WindowName.data(), &this->IsWindowVisible)) {
				_::__safe_exception_net net;
				net.set_instance_logger(this->Log);
				net.exec_l2([this]() { this->window(); });
			}
			ImGui::End();
		}
	}

	const std::string_view& iwindow::name() const noexcept { return this->_WindowName; }

#pragma endregion

#pragma region Class window_manager

	window_manager::window_manager() noexcept : _WindowInstances() {}

	window_manager::window_manager(window_manager&& _Other) noexcept {
		this->_WindowInstances = std::move(_Other._WindowInstances);
	}

	window_manager::~window_manager() noexcept {
		if (this->_WindowInstances.size() > 0) {
			for (iwindow* p : this->_WindowInstances)
				delete p;
			this->_WindowInstances.clear();
		}
	} 

	iwindow* window_manager::register_window(iwindow* _WindowInstance) {
		__stack_record();

		for (iwindow* p : this->_WindowInstances)
			if (p->name() == _WindowInstance->name()) {
				delete _WindowInstance;
				throw API::argument_exception("Window shares name with an already existing window.", "_WindowInstance");
			}

		_WindowInstance->set_instance_logger(this->Log);
		this->_WindowInstances.push_back(_WindowInstance);

		__stack_escape();
		return _WindowInstance;
	}
	
	iwindow* window_manager::get_window(std::string_view&& _WindowName) {
		__stack_record();

		iwindow* ret = nullptr;

		for (iwindow* p : this->_WindowInstances)
			if (p->name() == _WindowName) {
				ret = p;
				break;
			}

		if (!ret)
			throw API::argument_exception("No window with specified name found.", "_WindowName");

		__stack_escape();
		return ret;
	}

	void window_manager::remove_window(iwindow* _WindowInstance) {
		__stack_record();

		for (auto it = this->_WindowInstances.begin(); it != this->_WindowInstances.end(); ++it)
			if (*it == _WindowInstance) {
				delete _WindowInstance;
				this->_WindowInstances.erase(it);
				__stack_escape();
				return;
			}

		throw API::argument_exception("Instance does not exist as a registered window.", "_WindowInstance");
	}

	void window_manager::remove_window(std::string_view&& _WindowName) {
		__stack_record();
		__stack_rethrow(this->remove_window(this->get_window(std::forward<std::string_view>(_WindowName))));
		__stack_escape();
	}

	void window_manager::present_all() noexcept {
		for (iwindow* p : this->_WindowInstances)
			p->present();
	}

	window_manager& window_manager::operator=(window_manager&& _Other) noexcept {
		this->_WindowInstances = std::move(_Other._WindowInstances);

		return *this;
	}

#pragma endregion
}