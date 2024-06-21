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

			pCallStack = pGlobal->fetch();
			Assert::IsNull(pCallStack);
		}

		TEST_METHOD(PopUntilTest) {
			call_stack_manager* pGlobal = call_stack_manager::global();

			call_stack* pCallStack = pGlobal->fetch();
			Assert::IsNull(pCallStack);

			call_stack* pReturn = pGlobal->record("TestFunction", "TestFile", 1);
			Assert::IsNotNull(pReturn);

			pReturn = pGlobal->record("TestFunction2", "TestFile2", 2);
			Assert::IsNotNull(pReturn);

			pReturn = pGlobal->record("TestFunction3", "TestFile3", 3);
			Assert::IsNotNull(pReturn);

			call_stack_entry cse("TestFunction2", "TestFile2", 2);
			exception e(&cse);

			Assert::AreEqual(3ui64, e.calls()->entries().size());

			pCallStack = pGlobal->fetch();
			Assert::IsNotNull(pCallStack);

			Assert::AreEqual(1ui64, pCallStack->entries().size());

			call_stack_entry* pCurrent = pCallStack->entries().begin()._Ptr;
			Assert::AreEqual("TestFunction", pCurrent->_Function.c_str());
			Assert::AreEqual("TestFile", pCurrent->_File.c_str());
			Assert::AreEqual(1, pCurrent->_Line);

			pCallStack->drop();
		}

		TEST_METHOD(RecordAndEscapeTest) {
			__stack_record();

			call_stack_entry cse = *call_stack_manager::global()->fetch()->entries().begin();

			std::stringstream ss;
			ss << cse._Function << ": " << cse._File << " -> " << cse._Line;
			Logger::WriteMessage(ss.str().c_str());

			__stack_escape();

			Assert::IsNull(call_stack_manager::global()->fetch());
		}
	};
}