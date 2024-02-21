#include "pch.h"
#include "CppUnitTest.h"

#include <API/Event.hxx>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ArtemisTests {
	TEST_CLASS(EventTests) {
	public:
		Artemis::API::Event<> evn;
		DWORD dwLambda1Id;
		DWORD dwLambda2Id;
		INT n = 0;

		TEST_METHOD(ComprehensiveTest) {
			evn.UnsubscribeAll();

			dwLambda1Id = evn.Subscribe([](void*, Artemis::API::EventArgs& e) { e.bHandled = true; });
			Assert::AreEqual(1, evn.GetHandlerCount(), L"Checking if the correct amount of handlers have been subscribed.");

			dwLambda2Id = evn.Subscribe([this](void*, Artemis::API::EventArgs& e) -> void { if (e.bHandled) n = 10; });
			Assert::AreEqual(2, evn.GetHandlerCount(), L"Checking if the correct amount of handlers have been subscribed.");

			evn.Invoke(nullptr, new Artemis::API::EventArgs());
			Assert::AreEqual(10, n, L"Checking if Lambda 2 set the integer properly.");

			evn.Unsubscribe(dwLambda1Id);
			evn.Unsubscribe(dwLambda2Id);
			Assert::AreEqual(0, evn.GetHandlerCount(), L"Checking if the handler count is the correct amount after unsubscribing the Lambda handlers.");
		}
	};
}
