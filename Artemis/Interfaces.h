#ifndef __INTERFACES_H__
#define __INTERFACES_H__

namespace Artemis {
	class IDisposable {
	public:
		virtual void Release() = 0;
	};
}

#endif // !__INTERFACES_H__