//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include "SafeMemory.h"

namespace Artemis {
	ARTEMIS_API void SafeDelete(_In_ void* pBlock) {
		if (pBlock) {
			__try {
				delete pBlock;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}
	}
}