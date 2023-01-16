#include "GameClasses.h"
#include "Const.h"

namespace Artemis {
	CameraManager::CameraManager(_In_ Memory* pMemory) :
		X(*(float*)pMemory->ReadPtrAddress(Constants::c_CameraX)),
		Y(*(float*)pMemory->ReadPtrAddress(Constants::c_CameraY)),
		Z(*(float*)pMemory->ReadPtrAddress(Constants::c_CameraZ)),
		FieldOfView(*(float*)pMemory->ReadPtrAddress(Constants::c_FieldOfView)),
		pMem(pMemory) {}

	void CameraManager::ToggleFreezeCamera() {
		CONTEXT_BEGIN;

		switch (pMem->Read<BYTE>(pMem->GetModuleBase() + Constants::c_FreezeCamera.GetOffset())) {

		}

		CONTEXT_END;
	}

	void CameraManager::ToggleFreezeFov() {

	}
}