#pragma once

#include "pch.h"
#include "Definitions.h"

class PresentHook {
	LPVOID pTarget;

	PresentHook();
	~PresentHook();

public:
	static PresentHook* Create();
	void Enable();
	void Disable();
	void Release();
};