#include "pch.h"
#include "Events.h"

namespace Artemis::Engine::Events {
	ARTEMIS_API Aurora::Event<EnterMainMenuEventArgs> EnterMainMenuEvent;
	ARTEMIS_API Aurora::Event<EnterCustomGameLobbyEventArgs> EnterCustomGameLobbyEvent;
	ARTEMIS_API Aurora::Event<EnterPickPhaseEventArgs> EnterPickPhaseEvent;
	ARTEMIS_API Aurora::Event<EnterGameEventArgs> EnterGameEvent;
}