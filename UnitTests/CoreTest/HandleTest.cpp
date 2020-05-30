#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NFGE::Core;
namespace CoreTest
{
	TEST_CLASS(HandleTest)
	{
	public:
		class Foo
		{
		public:
			int a = 0;
			Foo() : a(4) {}
			~Foo() { a = 31; }
		};

		TEST_METHOD(TestDefaultHandle)
		{
			Handle<Foo> handle;
			Assert::IsFalse(handle.IsValid());
			Assert::IsNull(handle.Get());

			HandlePool<Foo> fooPool(1);
			Assert::IsFalse(fooPool.IsValid(handle));
			fooPool.Flush();
		}

		TEST_METHOD(TestRegister)
		{
			HandlePool<Foo> fooPool(1);
			Foo obj;

			Handle<Foo>handle = fooPool.Register(&obj);
			Assert::IsTrue(fooPool.IsValid(handle));
			Assert::IsTrue(fooPool.Get(handle) == &obj);
			
			Assert::IsTrue(handle.IsValid());
			Assert::IsTrue(handle.Get() == &obj);
			
			fooPool.Flush();
		}

		TEST_METHOD(TestHandleGet)
		{
			HandlePool<Foo> fooPool(1);
			Foo obj;

			Handle<Foo> handle = fooPool.Register(&obj);
			Assert::IsTrue(handle.IsValid());
			Assert::IsTrue(&obj == handle.Get());
			fooPool.Flush();
		}

		TEST_METHOD(TestHandleInvalidate)
		{
			HandlePool<Foo> fooPool(1);
			Foo obj;

			Handle<Foo> handle = fooPool.Register(&obj);
			handle.Invalidate();

			Assert::IsFalse(fooPool.IsValid(handle));
			Assert::IsNull(fooPool.Get(handle));
			Assert::IsFalse(handle.IsValid());
			Assert::IsNull(handle.Get());
			fooPool.Flush();
		}

		TEST_METHOD(TestHandleCopyGet)
		{
			HandlePool<Foo> fooPool(1);
			Foo obj;

			Handle<Foo> handle1 = fooPool.Register(&obj);
			Handle<Foo> handle2 = handle1;

			Assert::IsTrue(handle2.IsValid());
			Assert::IsTrue(&obj == handle2.Get());
			fooPool.Flush();
		}

		TEST_METHOD(TestHandleCopyInvalidate)
		{
			HandlePool<Foo> fooPool(1);
			Foo obj;

			Handle<Foo> handle1 = fooPool.Register(&obj);
			Handle<Foo> handle2 = handle1;
			handle1.Invalidate();

			Assert::IsTrue(handle2.IsValid());
			Assert::IsTrue(&obj == handle2.Get());
			fooPool.Flush();
		}

		TEST_METHOD(TestPoolUnregister)
		{
			HandlePool<Foo> fooPool(1);
			Foo obj;

			Handle<Foo> handle1 = fooPool.Register(&obj);
			Handle<Foo> handle2 = handle1;

			fooPool.Unregister(handle1);

			Assert::IsFalse(handle1.IsValid());
			Assert::IsNull(handle1.Get());
			Assert::IsFalse(handle2.IsValid());
			Assert::IsNull(handle2.Get());
		}

		
	};
}