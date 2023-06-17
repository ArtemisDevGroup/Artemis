#include "pch.h"
#include "KeybindManager.h"

namespace Artemis {
	void IKeybind::Invoke() {
		if (GetAsyncKeyState((int)nKey) & (1 << (Aurora::Binary<SHORT>::BufferBitCount - 1)))
			OnKeyPress();
	}

	KeybindManager::KeybindManager() { memset(KeybindCollection, 0, sizeof(KeybindCollection)); }

	KeybindManager::~KeybindManager() { this->Release(); }

	KeybindIndex KeybindManager::Add(_In_ IKeybind* pKeybind) {
		if (pKeybind->IsExclusive())
			for (IKeybind* pExistingKeybind : KeybindCollection)
				if (pExistingKeybind)
					if (pExistingKeybind->GetKey() == pKeybind->GetKey()) {
						delete pKeybind;
						return INVALID_INDEX;
					}
					else continue;
				else continue;
		else
			for (IKeybind* pExistingKeybind : KeybindCollection)
				if (pExistingKeybind)
					if (pExistingKeybind->IsExclusive())
						if (pExistingKeybind->GetKey() == pKeybind->GetKey()) {
							delete pKeybind;
							return INVALID_INDEX;
						}

		for (KeybindIndex i = 0; i < MAX_INVOKE; i++)
			if (!KeybindCollection[i]) {
				KeybindCollection[i] = pKeybind;
				return i;
			}

		delete pKeybind;
		return INVALID_INDEX;
	}

	void KeybindManager::Release(_In_range_(INVALID_INDEX, MAX_INVOKE) KeybindIndex nIndex) {
		if (nIndex == -1) {
			for (IKeybind* pKeybind : KeybindCollection)
				delete pKeybind;
			memset(KeybindCollection, 0, sizeof(KeybindCollection));
		}
		else {
			delete KeybindCollection[nIndex];
			KeybindCollection[nIndex] = nullptr;
		}
	}

	_Ret_maybenull_ IKeybind* KeybindManager::Get(_In_range_(0, MAX_INVOKE) KeybindIndex nIndex) {
		if (nIndex > 64 || nIndex < 0) return nullptr;
		return KeybindCollection[nIndex];
	}

	void KeybindManager::Invoke() {
		for (IKeybind* pKeybind : KeybindCollection)
			if (pKeybind)
				pKeybind->Invoke();
	}
}