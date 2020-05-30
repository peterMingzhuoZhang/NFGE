#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NFGE::Math;

namespace MathTest
{		
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestConstructor)
		{
			// TODO: Your test code here
			Vector3 v;
			Assert::AreEqual(v.x, 0.0f);
			Assert::AreEqual(v.y, 0.0f);
			Assert::AreEqual(v.z, 0.0f);

			Vector3 v1(1.0f);
			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 1.0f);
			Assert::AreEqual(v1.z, 1.0f);
			
			Vector3 v2(1.0f, 2.0f, 3.0f);
			Assert::AreEqual(v2.x, 1.0f);
			Assert::AreEqual(v2.y, 2.0f);
			Assert::AreEqual(v2.z, 3.0f);
		}

		TEST_METHOD(TestNegetion)
		{
			Vector3 v1(2.0f, 3.0f, 4.0f);
			Vector3 v2 = -v1;

			Assert::AreEqual(v2.x, -2.0f);
			Assert::AreEqual(v2.y, -3.0f);
			Assert::AreEqual(v2.z, -4.0f);
		}

		TEST_METHOD(TestAddition)
		{
			Vector3 v0(1.0f);
			Vector3 v1(2.0f, 3.0f, 4.0f);
			Vector3 v2 = v0 + v1;

			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 1.0f);
			Assert::AreEqual(v0.z, 1.0f);

			Assert::AreEqual(v1.x, 2.0f);
			Assert::AreEqual(v1.y, 3.0f);
			Assert::AreEqual(v1.z, 4.0f);
			
			Assert::AreEqual(v2.x, 3.0f);
			Assert::AreEqual(v2.y, 4.0f);
			Assert::AreEqual(v2.z, 5.0f);
		}

		TEST_METHOD(TestSubtruction)
		{
			Vector3 v0(2.0f, 2.0f, 2.0f);
			Vector3 v1(2.0f, 3.0f, 4.0f);
			Vector3 v2 = v0 - v1;

			Assert::AreEqual(v2.x, 0.0f);
			Assert::AreEqual(v2.y, -1.0f);
			Assert::AreEqual(v2.z, -2.0f);
		}

		TEST_METHOD(TestMutiply)
		{
			Vector3 v0(2.0f, 2.0f, 2.0f);
			float s = 2.0f;
			Vector3 v2 = v0 * s;

			Assert::AreEqual(v2.x, 4.0f);
			Assert::AreEqual(v2.y, 4.0f);
			Assert::AreEqual(v2.z, 4.0f);
		}

		TEST_METHOD(TestDivid)
		{
			Vector3 v0(2.0f, 2.0f, 2.0f);
			float s = 2.0f;
			Vector3 v2 = v0 / s;

			Assert::AreEqual(v2.x, 1.0f);
			Assert::AreEqual(v2.y, 1.0f);
			Assert::AreEqual(v2.z, 1.0f);
		}

		TEST_METHOD(TestAdditionAssigment)
		{
			Vector3 v0(2.0f, 2.0f, 2.0f);
			Vector3 v1(2.0f);
			Vector3 v2 = (v0 += v1);


			Assert::AreEqual(v0.x, 4.0f);
			Assert::AreEqual(v0.y, 4.0f);
			Assert::AreEqual(v0.z, 4.0f);

			Assert::AreEqual(v2.x, 4.0f);
			Assert::AreEqual(v2.y, 4.0f);
			Assert::AreEqual(v2.z, 4.0f);
		}

		TEST_METHOD(TestSubtructionAssigment)
		{
			Vector3 v0(2.0f, 2.0f, 2.0f);
			float s = 2.0f;
			Vector3 v2 = (v0 -= s);


			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);

			Assert::AreEqual(v2.x, 0.0f);
			Assert::AreEqual(v2.y, 0.0f);
			Assert::AreEqual(v2.z, 0.0f);
		}

		TEST_METHOD(TestMutiplyAssigment)
		{
			Vector3 v0(2.0f, 2.0f, 2.0f);
			float s = 2.0f;
			Vector3 v2 = (v0 *= s);


			Assert::AreEqual(v0.x, 4.0f);
			Assert::AreEqual(v0.y, 4.0f);
			Assert::AreEqual(v0.z, 4.0f);

			Assert::AreEqual(v2.x, 4.0f);
			Assert::AreEqual(v2.y, 4.0f);
			Assert::AreEqual(v2.z, 4.0f);
		}

		TEST_METHOD(TestDivisionAssigment)
		{
			Vector3 v0(2.0f, 2.0f, 2.0f);
			float s = 2.0f;
			Vector3 v2 = (v0 /= s);


			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 1.0f);
			Assert::AreEqual(v0.z, 1.0f);

			Assert::AreEqual(v2.x, 1.0f);
			Assert::AreEqual(v2.y, 1.0f);
			Assert::AreEqual(v2.z, 1.0f);
		}
	};
}