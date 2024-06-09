#include "pch.h"
#include "CppUnitTest.h"

#include <API/Memory.hxx>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Artemis::API;

namespace ArtemisTests {
	TEST_CLASS(MemoryTests) {
	public:
		TEST_METHOD(TestMethod1) {
			int buffer[16];
			ReadArray(0x0, buffer, 16);
			ReadArray(0x0, buffer);
			
			std::array<int, 16> buffer2;
			ReadArray(0x0, &buffer2);
			buffer2 = ReadArray<int, 16>(0x0);

			std::vector<int> buffer3;
			ReadArray(0x0, &buffer3, 16);
			buffer3 = ReadArray<int>(0x0, 16);
		}
	};
}