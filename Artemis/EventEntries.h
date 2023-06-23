#pragma once

#include "EventManager.h"

class EnterMainMenuEventEntry : public Artemis::IEventEntry {
public:
	virtual bool Condition() final;
	virtual void Invoke() final;
};

class EnterCustomGameLobbyEventEntry : public Artemis::IEventEntry {
public:
	virtual bool Condition() final;
	virtual void Invoke() final;
};

class EnterPickPhaseEventEntry : public Artemis::IEventEntry {
public:
	virtual bool Condition() final;
	virtual void Invoke() final;
};

class EnterGameEventEntry : public Artemis::IEventEntry {
public:
	virtual bool Condition() final;
	virtual void Invoke() final;
};