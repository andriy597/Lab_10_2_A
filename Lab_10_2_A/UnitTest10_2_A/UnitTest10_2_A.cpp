#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab_10_2_A/Lab_10_2_A.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest102A
{
	TEST_CLASS(UnitTest102A)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Student p[3];
			p[0].prizv = "Syd"; p[0].kurs = 1; p[0].informatyka = 3;
			p[1].prizv = "Pet"; p[1].kurs = 2; p[1].informatyka = 4;
			p[2].prizv = "Boi"; p[2].kurs = 2; p[2].informatyka = 5;
			int k = BinSearch(p, 3, "Boi", 2, 5);
			Assert::AreEqual(2, k);
		}
	};
}
