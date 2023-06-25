#ifndef __ARTEMIS_ENGINE_EVENTS_H__
#define __ARTEMIS_ENGINE_EVENTS_H__

#include "Definitions.h"

namespace Artemis::Engine::Events {
	class ARTEMIS_API OnNewFrameEventArgs {};

	class ARTEMIS_API EnterMainMenuEventArgs {};
	class ARTEMIS_API EnterCustomGameLobbyEventArgs {};
	class ARTEMIS_API EnterPickPhaseEventArgs {};
	class ARTEMIS_API EnterGameEventArgs {};

	ARTEMIS_API extern Aurora::Event<OnNewFrameEventArgs> OnNewFrameEvent;

	ARTEMIS_API extern Aurora::Event<EnterMainMenuEventArgs> EnterMainMenuEvent;
	ARTEMIS_API extern Aurora::Event<EnterCustomGameLobbyEventArgs> EnterCustomGameLobbyEvent;
	ARTEMIS_API extern Aurora::Event<EnterPickPhaseEventArgs> EnterPickPhaseEvent;
	ARTEMIS_API extern Aurora::Event<EnterGameEventArgs> EnterGameEvent;
}

#endif // !__ARTEMIS_ENGINE_EVENTS_H__