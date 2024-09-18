#ifndef __ARTEMIS_KEY_ACTION_HXX__
#define __ARTEMIS_KEY_ACTION_HXX__

#include <Windows.h> // UINT, WORD, VK_*

#include <type_traits>	// std::is_base_of_v, std::remove_reference_t
#include <vector>		// std::vector
#include <functional>	// std::function
#include <utility>		// std::pair, std::forward
#include <bitset>		// std::bitset

#include "Definitions.hxx"

#include "ExecutionContext.hxx"

#include "API/Error.hxx"
#include "API/Logging.hxx"

namespace Artemis {
	enum class key : WORD {
		left_mouse_button = VK_LBUTTON,
		right_mouse_button = VK_RBUTTON,
		middle_mouse_button = VK_MBUTTON,
		extra_mouse_button_1 = VK_XBUTTON1,
		extra_mouse_button_2 = VK_XBUTTON2,

		backspace = VK_BACK,
		tab = VK_TAB,
		enter = VK_RETURN,
		shift = VK_SHIFT,
		control = VK_CONTROL,
		alt = VK_MENU,
		pause = VK_PAUSE,
		capsLock = VK_CAPITAL,
		escape = VK_ESCAPE,
		space = VK_SPACE,
		page_up = VK_PRIOR,
		page_down = VK_NEXT,
		end = VK_END,
		home = VK_HOME,

		left_arrow = VK_LEFT,
		up_arrow = VK_UP,
		right_arrow = VK_RIGHT,
		down_arrow = VK_DOWN,

		print_screen = VK_SNAPSHOT,
		insert = VK_INSERT,
		del = VK_DELETE,

		zero = '0',
		one = '1',
		two = '2',
		three = '3',
		four = '4',
		five = '5',
		six = '6',
		seven = '7',
		eight = '8',
		nine = '9',

		a = 'A',
		b = 'B',
		c = 'C',
		d = 'D',
		e = 'E',
		f = 'F',
		g = 'G',
		h = 'H',
		i = 'I',
		j = 'J',
		k = 'K',
		l = 'L',
		m = 'M',
		n = 'N',
		o = 'O',
		p = 'P',
		q = 'Q',
		r = 'R',
		s = 'S',
		t = 'T',
		u = 'U',
		v = 'V',
		w = 'W',
		x = 'X',
		y = 'Y',
		z = 'Z',

		windows_left = VK_LWIN,
		windows_right = VK_RWIN,
		apps = VK_APPS,
		numpad_zero = VK_NUMPAD0,
		numpad_one = VK_NUMPAD1,
		numpad_two = VK_NUMPAD2,
		numpad_three = VK_NUMPAD3,
		numpad_four = VK_NUMPAD4,
		numpad_five = VK_NUMPAD5,
		numpad_six = VK_NUMPAD6,
		numpad_seven = VK_NUMPAD7,
		numpad_eight = VK_NUMPAD8,
		numpad_nine = VK_NUMPAD9,
		multiply = VK_MULTIPLY,
		add = VK_ADD,
		separator = VK_SEPARATOR,
		subtract = VK_SUBTRACT,
		decimal = VK_DECIMAL,
		divide = VK_DIVIDE,

		f1 = VK_F1,
		f2 = VK_F2,
		f3 = VK_F3,
		f4 = VK_F4,
		f5 = VK_F5,
		f6 = VK_F6,
		f7 = VK_F7,
		f8 = VK_F8,
		f9 = VK_F9,
		f10 = VK_F10,
		f11 = VK_F11,
		f12 = VK_F12,

		num_lock = VK_NUMLOCK,
		scroll_lock = VK_SCROLL,
		shift_left = VK_LSHIFT,
		shift_right = VK_RSHIFT,
		control_left = VK_LCONTROL,
		control_right = VK_RCONTROL,
		alt_left = VK_LMENU,
		alt_right = VK_RMENU,
		volume_mute = VK_VOLUME_MUTE,
		volume_down = VK_VOLUME_DOWN,
		volume_up = VK_VOLUME_UP,
		next_track = VK_MEDIA_NEXT_TRACK,
		previous_track = VK_MEDIA_PREV_TRACK,
		stop_track = VK_MEDIA_STOP,
		play_pause = VK_MEDIA_PLAY_PAUSE
	};

	namespace _ {
		class __key_container {
			key _Key;

		public:
			constexpr __key_container(key _Key) : _Key(_Key) {}

			constexpr key key() const noexcept { return this->_Key; }
		};

		class __key_down_action {
		public:
			virtual void on_key_down() = 0;
		};

		class __key_up_action {
		public:
			virtual void on_key_up() = 0;
		};
	}

	class key_down_action : public _::__key_container, public _::__key_down_action {
	public:
		using _::__key_container::__key_container;
	};

	class key_up_action : public _::__key_container, public _::__key_up_action {
	public:
		using _::__key_container::__key_container;
	};

	class key_action : public _::__key_container, public _::__key_down_action, public _::__key_up_action {
	public:
		using _::__key_container::__key_container;
	};

	class key_action_manager : public API::loggable {
		std::bitset<2048> _IdMap;

		template<typename _Ty>
		struct _identifiable_object {
			short _Id;
			_Ty _Object;
		};

		std::vector<_identifiable_object<_::__contextualized_object<key_down_action*>>> _KeyDownActions;
		std::vector<_identifiable_object<_::__contextualized_object<key_up_action*>>> _KeyUpActions;
		std::vector<_identifiable_object<_::__contextualized_object<key_action*>>> _KeyActions;

		std::vector<_identifiable_object<_::__contextualized_object<std::pair<key, std::function<void()>>>>> _DirectKeyActions;

	public:
		template<class _Ty>
			requires(std::is_base_of_v<key_down_action, std::remove_reference_t<_Ty>>)
		inline short register_action(_Ty&& _KeyDownAction) noexcept {
			for (short i = 0; i < this->_IdMap.size(); i++)
				if (!this->_IdMap.test(i)) {
					this->_IdMap.set(i);
					this->_KeyDownActions.push_back({ i, { _::__execution_context::get(), new _Ty(std::forward<_Ty>(_KeyDownAction)) }});
					return i;
				}
			return -1;
		}

		template<typename _Ty>
			requires(std::is_base_of_v<key_up_action, std::remove_reference_t<_Ty>>)
		inline short register_action(_Ty&& _KeyUpAction) noexcept {
			for (short i = 0; i < this->_IdMap.size(); i++)
				if (!this->_IdMap.test(i)) {
					this->_IdMap.set(i);
					this->_KeyUpActions.push_back({ i, { _::__execution_context::get(), new _Ty(std::forward<_Ty>(_KeyUpAction)) }});
					return i;
				}
			return -1;
		}

		template<typename _Ty>
			requires(std::is_base_of_v<key_action, std::remove_reference_t<_Ty>>)
		inline short register_action(_Ty&& _KeyAction) noexcept {
			for (short i = 0; i < this->_IdMap.size(); i++)
				if (!this->_IdMap.test(i)) {
					this->_IdMap.set(i);
					this->_KeyActions.push_back({ i, { _::__execution_context::get(), new _Ty(std::forward<_Ty>(_KeyAction)) }});
					return i;
				}
			return -1;
		}

		ARTEMIS_FRAMEWORK short register_action(key _Key, std::function<void()>&& _Action) noexcept;

		template<typename _Ty>
		inline _Ty* get(short _Id) const {
			__stack_record();

			if (!this->_IdMap.test(_Id))
				throw API::argument_exception("Argument contains an invalid id.", "_Id");

			_Ty* ret = nullptr;

			for (const auto& o : this->_KeyDownActions)
				if (o._Id == _Id) {
					ret = (_Ty*)o._Object._Object;
					break;
				}

			if (!ret) {
				for (const auto& o : this->_KeyUpActions)
					if (o._Id == _Id) {
						ret = (_Ty*)o._Object._Object;
						break;
					}

				if (!ret) {
					for (const auto& o : this->_KeyActions)
						if (o._Id == _Id) {
							ret = (_Ty*)o._Object._Object;
							break;
						}

					if (!ret)
						throw API::invalid_state_exception("_Id identifies a purely functional action with no object.");
				}
			}

			__stack_escape();
			return ret;
		}

		ARTEMIS_FRAMEWORK void remove(short _Id);

		ARTEMIS_FRAMEWORK void invoke(UINT _WindowMessage, key _Key) const noexcept;
	};
}

#endif // !__ARTEMIS_KEY_ACTION_HXX__