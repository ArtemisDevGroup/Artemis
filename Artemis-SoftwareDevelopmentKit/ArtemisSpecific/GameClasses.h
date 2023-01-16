#ifndef __GAME_CLASSES_H__
#define __GAME_CLASSES_H__

#include "..\Memory.h"

namespace Artemis {
	class ARTEMIS_API CameraManager {
		Memory* pMem;

	public:
		float& X;
		float& Y;
		float& Z;

		float& FieldOfView;

		CameraManager(_In_ Memory* pMemory);

		void ToggleFreezeCamera();
		void ToggleFreezeFov();
	};
}

#endif // !__GAME_CLASSES_H__