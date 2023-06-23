#include "pch.h"
#include "KeybindManager.h"

namespace Artemis {
	void IKeybind::Invoke() {
		if (GetAsyncKeyState((int)nKey) & (1 << (Aurora::Binary<SHORT>::BufferBitCount - 1)))
			OnKeyPress();
	}

	void KeybindManager::Invoke() {
		for (IKeybind* pKeybind : InvocableCollection)
			if (pKeybind)
				pKeybind->Invoke();
	}
}