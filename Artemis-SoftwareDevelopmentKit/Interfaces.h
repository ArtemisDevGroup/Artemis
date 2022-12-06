#ifndef __INTERFACES_H__
#define __INTERFACES_H__

namespace Artemis {
	class IDisposable {
	public:
		virtual void Release() = 0;
	};

	class IRegisterable {
	public:
		IRegisterable() : dwRegisteredCount(0) {}
		DWORD dwRegisteredCount; // The number of times this object has been registered.
	};
}

#endif // !__INTERFACES_H__