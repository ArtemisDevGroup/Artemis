#include <Windows.h>
#include <stdio.h>

FILE* pOut = nullptr, * pIn = nullptr;

void InitializeConsole() {
	AllocConsole();

	HWND hConsoleWnd = GetConsoleWindow();

	freopen_s(&pOut, "CONOUT$", "w", stdout);
	freopen_s(&pIn, "CONIN$", "r", stdin);

#define ROWCOUNT 11

	const char* art[ROWCOUNT] = {
		"                .........     .......................   ....              ....   ....       ...     ",
		"      ,d'      .xKKKKKKK0ko, .oKKKKKKKKKKxoOKKKKKK0Kx. .oKKO:           .l0KKc  .xKKo.   ,ok0K0Od:. ",
		"     .OWx.     .oOOOOOOOXWMNo.:xxxxxxxxxxloXMWXOOOkOo. .kMMMX:          lNMMMo  '0MMx. .dNMN0O0NMWk.",
		"    .dWMNl              .xWMX;            :NMWl        .kMMMMX:        cNMMMMo  '0MMx. cWMNl. .;kK0:",
		"    cNMMMX:            .'kWMX;    '::,.   :NMWo....... .kMMMMMK;      :XMMMMMo  '0MMx. cNMNo..   .. ",
		"   ;KMX0NM0'   .oOOOOOO0XWMXl.   .dMM0'   :NMMX0000000OoOMMXKWMK;    :XMWXNMMo  '0MMx. .dNMWX0Oxo,  ",
		"  .OMWo.xWWk.  .OMMNKXWMMXd'      dMM0'   :NMMX0OOO000koOMMk;xWMK;  ;KMWdcKMMo  '0MMx.   'lxkOKWMNx.",
		" .xWMO. '0MWo. 'OMMk'.dWMNc       dMM0'   :NMWo.       .kMMO..xWMK;;KMWx.,KMMo  '0MMx.        .lXMWo",
		" lNMX;   cXMNc 'OMMx. .dWMXc      dMM0'   :NMWo        .kMMk. .xWMXXMWx. .coo,  '0MMx.        .cXMWo",
		":XMWo    .dWMK,'OMMx.  .xWMX:     dMM0'   :NMMXOOOOOd. .kMMO.  .xWMMWx.       'dONMMXkl.     ,ONMWk'",
		"dKKd.     .xKKo,dKKo.   .xKKx'    lKKx.   ,OKKKKKKKKx. .oKKd.   .cooc.        ,OKKKKKKx.     ;0Od:. "
	};

	for (int i = 0; i < ROWCOUNT; i++) printf("%s\n", art[i]);
}

void UninitializeConsole() {
	if (pOut) fclose(pOut);
	if (pIn) fclose(pIn);
	FreeConsole();
}