#ifndef __ARTEMIS_MANAGER_H__
#define __ARTEMIS_MANAGER_H__

#include "pch.h"
#include "Definitions.h"

namespace Artemis {
	template<class T>
	concept AbstractClass = std::is_abstract<T>::value;

	template<class T>
	concept Int32 = std::is_same<T, int>::value;

	template<AbstractClass IInvocable, Int32 InvocableIndex>
	class Manager {
	protected:
		IInvocable* InvocableCollection[MAX_INVOKE];

	public:
		Manager();
		~Manager();

		InvocableIndex Add(_In_ IInvocable* pObject);

		void Release(_In_range_(INVALID_INDEX, MAX_INVOKE) InvocableIndex nIndex = INVALID_INDEX);

		_Ret_maybenull_ IInvocable* Get(_In_range_(0, MAX_INVOKE) InvocableIndex nIndex);
	};
}

#endif // !__ARTEMIS_MANAGER_H__