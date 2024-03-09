#ifndef ARTEMIS_API_CONSOLE_HXX
#define ARTEMIS_API_CONSOLE_HXX

#include "Definitions.hxx"

#include <Windows.h>

namespace Artemis::API {
	enum class ConsoleStream {
		In,
		Out,
		Error
	};

	ARTEMIS_API BOOL SetConsoleWindowTitle(_In_z_ LPCSTR lpConsoleWindowTitle);

	ARTEMIS_API BOOL OpenConsoleWindow(_In_opt_z_ LPCSTR lpConsoleWindowTitle);

	ARTEMIS_API BOOL OpenConsoleWindow();

	ARTEMIS_API BOOL CloseConsoleWindow();

	ARTEMIS_API BOOL OpenConsoleStream(_In_ ConsoleStream nConsoleStream);

	ARTEMIS_API BOOL OpenAllConsoleStreams();

	ARTEMIS_API BOOL CloseConsoleStream(_In_ ConsoleStream nConsoleStream);

	ARTEMIS_API BOOL CloseAllConsoleStreams();

	class ARTEMIS_API Logger {

	};
}

#endif // !ARTEMIS_API_CONSOLE_HXX