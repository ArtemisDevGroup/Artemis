//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __INTERFACES_H__
#define __INTERFACES_H__

namespace Artemis {
	class ARTEMIS_API IDisposable {
	public:
		virtual void Release() = 0;
	};

	class ARTEMIS_API IRegisterable {
	public:
		IRegisterable() : dwRegisteredCount(0) {}
		DWORD dwRegisteredCount; // The number of times this object has been registered.
	};
}

#endif // !__INTERFACES_H__