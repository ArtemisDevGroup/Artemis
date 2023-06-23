#include "pch.h"
#include "EventEntries.h"
#include "Events.h"

#include "GameManager.h"

static Artemis::Engine::GameManager GameManager(Aurora::GetCurrentProcessInfo().GetModule());
static Artemis::Engine::GameState LastGameState = Artemis::Engine::GameState::MainMenu;

bool EnterMainMenuEventEntry::Condition() {
	Artemis::Engine::GameState GameState = GameManager.GetGameState();
	bool bCondition = GameState != LastGameState && GameState == Artemis::Engine::GameState::MainMenu;
	LastGameState = GameState;
	return bCondition;
}

void EnterMainMenuEventEntry::Invoke() {
	Artemis::Engine::Events::EnterMainMenuEventArgs e;
	Artemis::Engine::Events::EnterMainMenuEvent.Invoke(this, &e);
}

bool EnterCustomGameLobbyEventEntry::Condition() {
	Artemis::Engine::GameState GameState = GameManager.GetGameState();
	bool bCondition = GameState != LastGameState && GameState == Artemis::Engine::GameState::GamePreLobby;
	LastGameState = GameState;
	return bCondition;
}

void EnterCustomGameLobbyEventEntry::Invoke() {
	Artemis::Engine::Events::EnterCustomGameLobbyEventArgs e;
	Artemis::Engine::Events::EnterCustomGameLobbyEvent.Invoke(this, &e);
}

bool EnterPickPhaseEventEntry::Condition() {
	Artemis::Engine::GameState GameState = GameManager.GetGameState();
	bool bCondition = GameState != LastGameState && GameState == Artemis::Engine::GameState::PickPhase;
	LastGameState = GameState;
	return bCondition;
}

void EnterPickPhaseEventEntry::Invoke() {
	Artemis::Engine::Events::EnterPickPhaseEventArgs e;
	Artemis::Engine::Events::EnterPickPhaseEvent.Invoke(this, &e);
}

bool EnterGameEventEntry::Condition() {
	Artemis::Engine::GameState GameState = GameManager.GetGameState();
	bool bCondition = GameState != LastGameState && GameState == Artemis::Engine::GameState::Playing;
	LastGameState = GameState;
	return bCondition;
}

void EnterGameEventEntry::Invoke() {
	Artemis::Engine::Events::EnterGameEventArgs e;
	Artemis::Engine::Events::EnterGameEvent.Invoke(this, &e);
}