// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

#ifndef ARTEMIS_TESTS
#error Build Configuration needs to be "Test" in order to run tests.
#endif // !ARTEMIS_TESTS

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

/*
#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ArtemisTests {
	TEST_CLASS(ArtemisTests) {
	public:
		TEST_METHOD(TestMethod1) {
		}
	};
}
*/
