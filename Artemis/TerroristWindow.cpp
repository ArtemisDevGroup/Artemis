//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014 and @Intifofo.									|
// @Astrea0014: https://github.com/Astrea0014											|
// @Intifofo: https://github.com/Intifofo												|
//-------------------------------------------------------------------------------------->

#include "Window.h"
#include <ArtemisSpecific/Const.h>

using namespace Artemis::Constants;

TerroristWindow::TerroristWindow() : IWindow("Terrorist Window", WND_TERRORISTWINDOW) {
	Logger* pLog = &Midnight::GetInst()->Log;
	ConfigurationSection WndSect = Midnight::GetInst()->GlobalConfig.GetSection("Window");

	try {
		bShow = WndSect.GetPropertyValue("TerroristWindow");
	}
	catch (ObjectNotFoundException& e) {
		bShow = FALSE;
		pLog->LogInfo(__FUNCTION__, "ConfigurationProperty \"TerroristWindow\" could not be found, creating...");
		WndSect.AddProperty("TerroristWindow", FALSE);
	}
}

void TerroristWindow::Window() {

	static Memory* pm = &Midnight::GetInst()->Mem;
	static Logger* pLog = &Midnight::GetInst()->Log;
	static ADDRESS uBase = pm->GetModuleBase();


	//ImGui::Button("Spawn");
	//if (ImGui::Button("Spawn")) {
	//
	//	int nothing = pm->Read<int>(0);
	//
	//
	//	int buffer[20];
	//
	//	pLog->LogInfo(__FUNCTION__, "This Ran For Some Reason");
	//
	//	pm->ReadArray(uBase+c_GameManager,buffer,20);
	//
	//	pm->ReadPtr<int>(000,{30,60,30,(ADDRESS)nothing,(ADDRESS)0-40,20});
	//
	//
	//	pm->CreateHook();
	//
	//}
}

void TerroristWindow::Inini() {
	/// run once on startup stuff

}