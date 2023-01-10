#ifndef __GAME_CLASSES_H__
#define __GAME_CLASSES_H__

#include "Midnight.h"

namespace Artemis {
	class CameraManager {
	public:
		float& X;
		float& Y;
		float& Z;

		float& FieldOfView;

		CameraManager(_In_ Midnight* pMidnight);

		void ToggleFreezeCamera();
		void ToggleFreezeFov();
	};
}

#endif // !__GAME_CLASSES_H__