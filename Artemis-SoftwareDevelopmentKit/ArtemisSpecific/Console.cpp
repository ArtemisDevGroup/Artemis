#include "Console.h"

#include "..\Exceptions.h"

namespace Artemis {
	LPCSTR lpszAsciiArt[13] = {
		"<------------------------------------------------------------------------------------------------------>",
		"|                 .........     .......................   ....              ....   ....       ...      |",
		"|       ,d'      .xKKKKKKK0ko, .oKKKKKKKKKKxoOKKKKKK0Kx. .oKKO:           .l0KKc  .xKKo.   ,ok0K0Od:.  |",
		"|      .OWx.     .oOOOOOOOXWMNo.:xxxxxxxxxxloXMWXOOOkOo. .kMMMX:          lNMMMo  '0MMx. .dNMN0O0NMWk. |",
		"|     .dWMNl              .xWMX;            :NMWl        .kMMMMX:        cNMMMMo  '0MMx. cWMNl. .;kK0: |",
		"|     cNMMMX:            .'kWMX;    '::,.   :NMWo....... .kMMMMMK;      :XMMMMMo  '0MMx. cNMNo..   ..  |",
		"|    ;KMX0NM0'   .oOOOOOO0XWMXl.   .dMM0'   :NMMX0000000OoOMMXKWMK;    :XMWXNMMo  '0MMx. .dNMWX0Oxo,   |",
		"|   .OMWo.xWWk.  .OMMNKXWMMXd'      dMM0'   :NMMX0OOO000koOMMk;xWMK;  ;KMWdcKMMo  '0MMx.   'lxkOKWMNx. |",
		"|  .xWMO. '0MWo. 'OMMk'.dWMNc       dMM0'   :NMWo.       .kMMO..xWMK;;KMWx.,KMMo  '0MMx.        .lXMWo |",
		"|  lNMX;   cXMNc 'OMMx. .dWMXc      dMM0'   :NMWo        .kMMk. .xWMXXMWx. .coo,  '0MMx.        .cXMWo |",
		"| :XMWo    .dWMK,'OMMx.  .xWMX:     dMM0'   :NMMXOOOOOd. .kMMO.  .xWMMWx.       'dONMMXkl.     ,ONMWk' |",
		"| dKKd.     .xKKo,dKKo.   .xKKx'    lKKx.   ,OKKKKKKKKx. .oKKd.   .cooc.        ,OKKKKKKx.     ;0Od:.  |",
		"<------------------------------------------------------------------------------------------------------>"
	};

	Console::Console() : bAllocated(FALSE), bShowing(FALSE), pOut(nullptr), pIn(nullptr), hConsoleOut(nullptr), hConsoleWnd(nullptr) {}

	void Console::Allocate() {
		if (!bAllocated) {
			if (!AllocConsole()) throw WindowsApiException("AllocConsole");

			if (freopen_s(&pOut, "CONOUT$", "w", stdout) || freopen_s(&pIn, "CONIN$", "r", stdin))
				throw Exception("Something went wrong with freopen_s.", ExceptionCode::Unknown);

			hConsoleWnd = GetConsoleWindow();
			hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
			bAllocated = TRUE;

			for (INT i = 0; i < 13; i++) printf("%s\n", lpszAsciiArt[i]);
		}
	}

	void Console::Release() {
		if (bAllocated) {
			if (!FreeConsole()) throw WindowsApiException("FreeConsole");

			fclose(pOut);
			fclose(pIn);

			bAllocated = FALSE;
		}
	}

	void Console::Show() const { if (bAllocated && bShowing) if (!ShowWindow(hConsoleWnd, SW_SHOW)) throw WindowsApiException("ShowWindow"); }
	void Console::Hide() const { if (bAllocated && !bShowing) if (!ShowWindow(hConsoleWnd, SW_HIDE)) throw WindowsApiException("ShowWindow"); }

	void Console::SetTextColor(_In_ WORD wColor) const { if (bAllocated) if (!SetConsoleTextAttribute(hConsoleOut, wColor)) throw WindowsApiException("SetConsoleTextAttribute"); }

	Console::~Console() { Release(); }
}