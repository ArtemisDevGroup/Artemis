#include "CppUnitTest.h"

#include "API/Threading.hxx"

#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Artemis::API;

namespace ArtemisTests {
	TEST_CLASS(ThreadingTests) {
	public:
		TEST_METHOD(TwoThreadRequestIndefinite) {
			thread_safe<int> x = 10;

			std::thread t1([&x]() {
				Logger::WriteMessage("[Thread 1] Requesting resource.\n");
				x.request();
				Logger::WriteMessage("[Thread 1] Obtained resource.\n");
				
				std::this_thread::sleep_for(std::chrono::seconds(5));
				Assert::AreEqual(10, *x);

				x.release();
				Logger::WriteMessage("[Thread 1] Released resource.\n");
				});

			std::thread t2([&x]() {
				Logger::WriteMessage("[Thread 2] Requesting resource.\n");
				x.request();
				Logger::WriteMessage("[Thread 2] Obtained resource.\n");

				*x = 20;

				x.release();
				Logger::WriteMessage("[Thread 2] Released resource.\n");
				});

			t1.join();
			t2.join();

			x.request();
			Assert::AreEqual(20, *x);
			x.release();
		}
	};
}