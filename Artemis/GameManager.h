#ifndef __ARTEMIS_ENGINE_GAME_MANAGER_H__
#define __ARTEMIS_ENGINE_GAME_MANAGER_H__

#include "pch.h"
#include "Definitions.h"

namespace Artemis::Engine {
	enum class GameState : int {
		Invalid = 0,
		Shutdown = 1,
		MainMenu = 2,
		GamePreLobby = 3,
		PendingRoundStartup = 4,
		PickPhase = 5,
		Playing = 6,
		PlayerControlAllowed = 7
	};

	class ARTEMIS_API GameManager {
		A_ADDR uGameManager;

	public:
		GameManager(const Aurora::ModuleInfo& refRainbowSix);

		GameState GetGameState() const;
	};
}

#endif // !__ARTEMIS_ENGINE_GAME_MANAGER_H__