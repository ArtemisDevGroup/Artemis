#include "pch.h"
#include "Manager.h"

#include "EventManager.h"
#include "KeybindManager.h"
#include "WindowManager.h"

namespace Artemis {
	template<AbstractClass IInvocable, Int32 InvocableIndex>
	Manager<IInvocable, InvocableIndex>::Manager() { ZeroMemory(InvocableCollection, sizeof(InvocableCollection)); }

	template<AbstractClass IInvocable, Int32 InvocableIndex>
	Manager<IInvocable, InvocableIndex>::~Manager() { this->Release(); }

	template<AbstractClass IInvocable, Int32 InvocableIndex>
	InvocableIndex Manager<IInvocable, InvocableIndex>::Add(_In_ IInvocable* pObject) {
		for (InvocableIndex i = 0; i < MAX_INVOKE; i++)
			if (!InvocableCollection[i]) {
				InvocableCollection[i] = pObject;
				return i;
			}

		delete pObject;
		return INVALID_INDEX;
	}

	template<AbstractClass IInvocable, Int32 InvocableIndex>
	void Manager<IInvocable, InvocableIndex>::Release(_In_range_(INVALID_INDEX, MAX_INVOKE) InvocableIndex nIndex) {
		if (nIndex == INVALID_INDEX) {
			for (IInvocable* pObject : InvocableCollection)
				if (pObject)
					delete pObject;
			memset(InvocableCollection, 0, sizeof(InvocableCollection));
		}
		else if (InvocableCollection[nIndex]) {
			delete InvocableCollection[nIndex];
			InvocableCollection[nIndex] = nullptr;
		}
	}

	template<AbstractClass IInvocable, Int32 InvocableIndex>
	_Ret_maybenull_ IInvocable* Manager<IInvocable, InvocableIndex>::Get(_In_range_(0, MAX_INVOKE) InvocableIndex nIndex) {
		if (nIndex > 64 || nIndex < 0) return nullptr;
		return InvocableCollection[nIndex];
	}

	template class ARTEMIS_EXPORT Manager<IEventEntry, EventEntryIndex>;
	template class ARTEMIS_EXPORT Manager<IKeybind, KeybindIndex>;
	template class ARTEMIS_EXPORT Manager<IWindow, WindowIndex>;
}