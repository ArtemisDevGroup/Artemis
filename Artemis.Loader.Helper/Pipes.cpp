#include <Windows.h>

namespace Artemis {
	namespace Loader {
		namespace Helper {
			public ref class PipeServer {
				HANDLE hPipe;

			public:
				PipeServer() {
					hPipe = CreateNamedPipeA(
						".\\\\pipe\\ArtemisRpcPipe",
						PIPE_ACCESS_INBOUND,
						PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
						1,
						16,
						16,
						0,
						nullptr
					);
				}

				void WaitForConnection() { ConnectNamedPipe(hPipe, nullptr); }

				void Read(array<System::Byte>^ byteArray) {
					BYTE buf[16];

					DWORD dwReadCount;
					
					ReadFile(
						hPipe,
						buf,
						16,
						&dwReadCount,
						nullptr
					);

					for (int i = 0; i < dwReadCount; i++) {
						byteArray[i] = buf[i];
					}
				}

				void Release() { CloseHandle(hPipe); }
			};
		}
	}
}