#include "pch.h"
#include "CppUnitTest.h"

#include <API/Error.hxx>

using namespace Artemis::API;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ArtemisTests {
	TEST_CLASS(ErrorTests) {
	public:
		BEGIN_TEST_METHOD_ATTRIBUTE(SetAndGetBasicError)
			TEST_DESCRIPTION("Sets and gets basic errors with no extended attributes.")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(SetAndGetBasicError) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::Unknown);
			ErrorInfo e = GetLastArtemisError();
			Assert::AreEqual((int)ErrorCode::Unknown, (int)e.dwErrorCode, L"Wrong error code.");
			Assert::AreEqual("An unknown error has occured.", e.szErrorMessage, L"Wrong error message.");
			Assert::AreEqual(__FUNCTION__, e.szFunction, L"Wrong function.");

			SetLastArtemisError("MyFunction", ErrorCode::Success);
			ErrorInfo e2 = GetLastArtemisError();
			Assert::AreEqual((int)ErrorCode::Success, (int)e2.dwErrorCode, L"Wrong error code.");
			Assert::AreEqual("An operation was successful.", e2.szErrorMessage, L"Wrong error message.");
			Assert::AreEqual("MyFunction", e2.szFunction, L"Wrong function.");
		}
	};
}