#ifndef __ARTEMIS_EVENT_MANAGER_H__
#define __ARTEMIS_EVENT_MANAGER_H__

#include "pch.h"
#include "Definitions.h"
#include "Manager.h"

namespace Artemis {
	class IEventEntry {
	public:
		virtual bool Condition() = 0;
		virtual void Invoke() = 0;
	};

	using EventEntryIndex = int;

#ifdef _ARTEMIS_EXPORT
	extern template class Manager<IEventEntry, EventEntryIndex>;
#else
	template class ARTEMIS_IMPORT Manager<IEventEntry, EventEntryIndex>;
#endif // _ARTEMIS_EXPORT

	class ARTEMIS_API EventManager : public Manager<IEventEntry, EventEntryIndex> {
	public:
		void Invoke();
	};
}

#endif // !__ARTEMIS_EVENT_MANAGER_H__