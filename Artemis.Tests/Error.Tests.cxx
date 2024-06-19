#include "CppUnitTest.h"

#include "API/Error.hxx"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Artemis::API;

namespace ArtemisTests {
	TEST_CLASS(ErrorTests) {
	public:
		TEST_METHOD(RecordAndFetchTest) {
			call_stack_manager* pGlobal = call_stack_manager::global();

			call_stack* pCallStack = pGlobal->fetch();
			Assert::IsNull(pCallStack);

			call_stack* pReturn = pGlobal->record("TestFunction", "TestFile", 1);
			Assert::IsNotNull(pReturn);

			pCallStack = pGlobal->fetch();
			Assert::IsNotNull(pCallStack);

			Assert::AreEqual(1ui64, pCallStack->entries().size());
			
			const call_stack_entry* pEntry = &pCallStack->entries()[0];

			Assert::AreEqual("TestFunction", pEntry->_Function.c_str());
			Assert::AreEqual("TestFile", pEntry->_File.c_str());
			Assert::AreEqual(1, pEntry->_Line);

			pCallStack->drop();
		}
	};
}