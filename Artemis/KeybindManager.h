#ifndef __ARTEMIS_KEYBIND_MANAGER_H__
#define __ARTEMIS_KEYBIND_MANAGER_H__

#include "pch.h"
#include "Definitions.h"
#include "Manager.h"

namespace Artemis {
	enum class VirtualKey : int {
		LeftMouseButton = VK_LBUTTON,
		RightMouseButton = VK_RBUTTON,
		MiddleMouseButton = VK_MBUTTON,
		ExtraMouseButton1 = VK_XBUTTON1,
		ExtraMouseButton2 = VK_XBUTTON2,

		Backspace = VK_BACK,
		Tab = VK_TAB,
		Enter = VK_RETURN,
		Shift = VK_SHIFT,
		Control = VK_CONTROL,
		Alt = VK_MENU,
		Pause = VK_PAUSE,
		CapsLock = VK_CAPITAL,
		Escape = VK_ESCAPE,
		Space = VK_SPACE,
		PageUp = VK_PRIOR,
		PageDown = VK_NEXT,
		End = VK_END,
		Home = VK_HOME,

		LeftArrow = VK_LEFT,
		UpArrow = VK_UP,
		RightArrow = VK_RIGHT,
		DownArrow = VK_DOWN,

		PrintScreen = VK_SNAPSHOT,
		Insert = VK_INSERT,
		Delete = VK_DELETE,

		Zero = '0',
		One = '1',
		Two = '2',
		Three = '3',
		Four = '4',
		Five = '5',
		Six = '6',
		Seven = '7',
		Eight = '8',
		Nine = '9',

		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',

		LeftWindows = VK_LWIN,
		RightWindows = VK_RWIN,
		Apps = VK_APPS,
		NumpadZero = VK_NUMPAD0,
		NumpadOne = VK_NUMPAD1,
		NumpadTwo = VK_NUMPAD2,
		NumpadThree = VK_NUMPAD3,
		NumpadFour = VK_NUMPAD4,
		NumpadFive = VK_NUMPAD5,
		NumpadSix = VK_NUMPAD6,
		NumpadSeven = VK_NUMPAD7,
		NumpadEight = VK_NUMPAD8,
		NumpadNine = VK_NUMPAD9,
		Multiply = VK_MULTIPLY,
		Add = VK_ADD,
		Separator = VK_SEPARATOR,
		Subtract = VK_SUBTRACT,
		Decimal = VK_DECIMAL,
		Divide = VK_DIVIDE,

		F1 = VK_F1,
		F2 = VK_F2,
		F3 = VK_F3,
		F4 = VK_F4,
		F5 = VK_F5,
		F6 = VK_F6,
		F7 = VK_F7,
		F8 = VK_F8,
		F9 = VK_F9,
		F10 = VK_F10,
		F11 = VK_F11,
		F12 = VK_F12,

		NumLock = VK_NUMLOCK,
		ScrollLock = VK_SCROLL,
		LeftShift = VK_LSHIFT,
		RightShift = VK_RSHIFT,
		LeftControl = VK_LCONTROL,
		RightControl = VK_RCONTROL,
		LeftAlt = VK_LMENU,
		RightAlt = VK_RMENU,
		VolumeMute = VK_VOLUME_MUTE,
		VolumeDown = VK_VOLUME_DOWN,
		VolumeUp = VK_VOLUME_UP,
		NextTrack = VK_MEDIA_NEXT_TRACK,
		PreviousTrack = VK_MEDIA_PREV_TRACK,
		StopTrack = VK_MEDIA_STOP,
		PlayPause = VK_MEDIA_PLAY_PAUSE
	};

	class ARTEMIS_API IKeybind {
		VirtualKey nKey;
		bool bExclusive;

	public:
		constexpr IKeybind(_In_ VirtualKey nKey, _In_ bool bExclusive) noexcept : nKey(nKey), bExclusive(bExclusive) {}

		virtual void OnKeyPress() = 0;

		void Invoke();

		constexpr VirtualKey GetKey() const noexcept { return nKey; }
		constexpr bool IsExclusive() const noexcept { return bExclusive; }
	};

	using KeybindIndex = int;

#ifdef _ARTEMIS_EXPORT
	extern template class Manager<IKeybind, KeybindIndex>;
#endif // _ARTEMIS_EXPORT

	class ARTEMIS_API KeybindManager : public Manager<IKeybind, KeybindIndex> {
	public:
		void Invoke();
	};
}

#endif // !__ARTEMIS_KEYBIND_MANAGER_H__