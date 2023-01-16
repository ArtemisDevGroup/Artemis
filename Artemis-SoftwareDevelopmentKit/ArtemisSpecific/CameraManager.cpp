#include "GameClasses.h"
#include "Const.h"

namespace Artemis {
	CameraManager::CameraManager(_In_ Memory* pMemory) :
		X(*(float*)pMemory->ReadPtrAddress(Constants::c_CameraX)),
		Y(*(float*)pMemory->ReadPtrAddress(Constants::c_CameraY)),
		Z(*(float*)pMemory->ReadPtrAddress(Constants::c_CameraZ)),
		FieldOfView(*(float*)pMemory->ReadPtrAddress(Constants::c_FieldOfView)) {}

	void CameraManager::ToggleFreezeCamera() {

	}

	void CameraManager::ToggleFreezeFov() {

	}
}