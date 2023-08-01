#include "pch.h"
#include "CppUnitTest.h"
#include "../SignalDecodingQZSSLib/BitContainer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestProject
{
TEST_CLASS(BitContainerTest)
{
public:
    TEST_METHOD(GetNumTest){};
    TEST_METHOD(UnsignedGetNumTest)
    {
        // less than 1 unsigned long
        BitContainer test("1011010101110");
        Assert::AreEqual(0b10110UL, test.getNum(0, 5));
        Assert::AreEqual(0b1010101UL, test.getNum(3, 7));
        Assert::AreEqual(0b010101UL, test.getNum(4, 6));
        Assert::AreEqual(0b1011010101110UL, test.getNum(0, 13));
        Assert::AreEqual(0b1UL, test.getNum(0, 1));
        Assert::AreEqual(0b0UL, test.getNum(12, 1));
        // отрицательное значение size_t не бывает и конвертируется сразу в положительное
        // Assert::ExpectException<std::out_of_range>([&test] { test.getNum(-1, 2); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(0, 14); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(5, 9); });

        // exactly 1 unsigned long
        test.fromString("10011011010011110000101000010110");
        Assert::AreEqual(0b1001101101001111000UL, test.getNum(0, 19));
        Assert::AreEqual(0b11010011UL, test.getNum(6, 8));
        Assert::AreEqual(0b10011011010011110000101000010110UL, test.getNum(0, 32));
        Assert::AreEqual(0b1UL, test.getNum(0, 1));
        Assert::AreEqual(0b0UL, test.getNum(31, 1));
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(0, 65); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(31, 5); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(32, 1); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(30, 0); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(32, 0); });
        
        // more than 1 unsigned long and less than 2
        test.fromString("10011011010011110000101000010110000001011110101111001101");
        Assert::AreEqual(0b1001101101001111UL, test.getNum(0, 16));
        Assert::AreEqual(0b10110100111100001010000UL, test.getNum(4, 23));
        Assert::AreEqual(0b000101111010111100110UL, test.getNum(34, 21));
        Assert::AreEqual(0b10011011010011110000101000010110UL, test.getNum(0, 32));
        Assert::AreEqual(0b000001011110101111001101UL, test.getNum(32, 24));
        Assert::AreEqual(0b001011000000101UL, test.getNum(25, 15));
        Assert::AreEqual(0b1UL, test.getNum(0, 1));
        Assert::AreEqual(0b1UL, test.getNum(55, 1));
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(0, 65); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(40, 25); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(56, 3); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(56, 0); });

        // more than 2 unsigned long
        test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                        "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                        "1111011001100101011010000110");
        Assert::AreEqual(0b1110101111011100110UL, test.getNum(0, 19));
        Assert::AreEqual(0b100011101110111010010UL, test.getNum(34, 21));
        Assert::AreEqual(0b1001000001100110101011111UL, test.getNum(50, 25));
        Assert::AreEqual(0b1110100001011001110001111UL, test.getNum(120, 25));
        Assert::AreEqual(0b011111011001100101011010000110UL, test.getNum(226, 30));
        Assert::AreEqual(0b00011111011001100101011010000110UL, test.getNum(224, 32));
        Assert::AreEqual(0b1UL, test.getNum(0, 1));
        Assert::AreEqual(0b0UL, test.getNum(255, 1));
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(0, 65); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(240, 40); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(256, 0); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(256, 1); });
        Assert::ExpectException<std::out_of_range>([&test] { test.getNum(255, 2); });
    };
};

TEST_CLASS(UnitTestProject2)
{
public:
    TEST_METHOD(GetNumTest3){};

    TEST_METHOD(GetNumTest4){};
};
} // namespace UnitTestProject
