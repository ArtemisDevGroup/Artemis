#include "GameClasses.h"
#include "Const.h"

namespace Artemis {
	CameraManager::CameraManager(_In_ Midnight* pMidnight) :
		X(*(float*)pMidnight->Mem.ReadPtrAddress(&Constants::c_CameraX)),
		Y(*(float*)pMidnight->Mem.ReadPtrAddress(&Constants::c_CameraY)),
		Z(*(float*)pMidnight->Mem.ReadPtrAddress(&Constants::c_CameraZ)),
		FieldOfView(*(float*)pMidnight->Mem.ReadPtrAddress(&Constants::c_FieldOfView)) {}
}