#include "pch.h"
#include "EventManager.h"

namespace Artemis {
	void EventManager::Invoke() {
		for (IEventEntry* pEntry : InvocableCollection)
			if (pEntry && pEntry->Condition())
				pEntry->Invoke();
	}
}