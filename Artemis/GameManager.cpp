#include "pch.h"
#include "GameManager.h"

#include "Constants.h"

namespace Artemis::Engine {
	GameManager::GameManager(const Aurora::ModuleInfo& refRainbowSix) : uGameManager(refRainbowSix.GetModuleBaseAddress() + Constants::c_uGameManagerOffset) {}

	GameState GameManager::GetGameState() const {
		return *(GameState*)(*(A_ADDR*)uGameManager + Constants::c_uGameStateOffset);
	}
}