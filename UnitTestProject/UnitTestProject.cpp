#include "pch.h"
#include "CppUnitTest.h"
#include "../SignalDecodingQZSSLib/BitContainer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace Microsoft
{
namespace VisualStudio
{
namespace CppUnitTestFramework
{
// для отображения expected и actual в Assert::AreEqual
template <> static std::wstring ToString(const BitContainer& q)
{
    std::string str = q.toString();
    std::wstring wstr;
    wstr.reserve(str.length()); // Резервируем место для широкой строки

    for (const char c : str)
    {
        wstr.push_back(static_cast<wchar_t>(c));
    }
    return wstr;
}
} // namespace CppUnitTestFramework
} // namespace VisualStudio
} // namespace Microsoft

namespace UnitTestProject
{

TEST_CLASS(BitContainerTest)
{

public:
    TEST_METHOD(addSequenceTest)
    {
        BitContainer test1("1011010101110");
        BitContainer test2("1101101010111");
        test1.add(test2);
        Assert::AreEqual(BitContainer("10110101011101101101010111"), test1);

        test1.fromString("");
        test1.add(test2);
        Assert::AreEqual(test2, test1);

        test1.fromString("");
        test2.fromString("");
        Assert::AreEqual(test1, test2);
        test1.fromString("00000000");
        test2.fromString("10110101011101101101010110");
        test1.add(test2);
        Assert::AreEqual(BitContainer("0000000010110101011101101101010110"), test1);

        test1.fromString("10011011010011110000101000010110");
        test2.fromString("10011011010011110000101000010110000001011110101111001101");
        test1.add(test2);
        Assert::AreEqual(BitContainer("1001101101001111000010100001011010011011010011110000101000010110000001011110101111001101"), test1);

        test1.fromString(
            "111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
            "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
            "1111011001100101011010000110");
        test2.fromString("10011011010011110000101000010110000001011110101111001101");
        test1.add(test2);
        Assert::AreEqual(
            BitContainer("11101011110111001101110100001001011000111011101110100100000110011010101111111001110101110010111010111101000111110"
                         "10100111110100001011001110001111000011011000000001011100001111111001010111011010100111100001011010101100010101000"
                         "01111101100110010101101000011010011011010011110000101000010110000001011110101111001101"),
            test1);

        test1.fromString("000");
        test2.fromString("111");
        test1.add(test2);
        Assert::AreEqual(BitContainer("000111"), test1);
    };
    TEST_METHOD(GetNumTest)
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

    TEST_METHOD(SubContainerTest)
    {
        BitContainer test("10110101011101101101010110");
        Assert::AreEqual(BitContainer("1"), test.subContainer(0, 1));
        Assert::AreEqual(BitContainer("0"), test.subContainer(25, 1));
        Assert::AreEqual(BitContainer("11011011"), test.subContainer(10, 8));
        Assert::AreEqual(BitContainer("10110101011101101101010110"), test.subContainer(0, 26));
        Assert::ExpectException<std::out_of_range>([&test] { test.subContainer(0, 30); });
        Assert::ExpectException<std::out_of_range>([&test] { test.subContainer(10, 20); });

        test.fromString("10011011010011110000101000010110");
        Assert::AreEqual(BitContainer("1"), test.subContainer(0, 1));
        Assert::AreEqual(BitContainer("0"), test.subContainer(31, 1));
        Assert::AreEqual(BitContainer("0111100001010"), test.subContainer(11, 13));
        Assert::AreEqual(BitContainer("10000101000010110"), test.subContainer(15, 17));
        Assert::AreEqual(BitContainer("10011011010011110000101000010110"), test.subContainer(0, 32));
        Assert::ExpectException<std::out_of_range>([&test] { test.subContainer(0, 33); });
        Assert::ExpectException<std::out_of_range>([&test] { test.subContainer(30, 4); });

        test.fromString("10011011010011110000101000010110000001011110101111001101");
        Assert::AreEqual(BitContainer("1"), test.subContainer(0, 1));
        Assert::AreEqual(BitContainer("1"), test.subContainer(55, 1));
        Assert::AreEqual(BitContainer("0001011110101111"), test.subContainer(34, 16));
        Assert::AreEqual(BitContainer("00001010000101100000010111101011"), test.subContainer(16, 32));
        Assert::AreEqual(BitContainer("000010100001011000000101111010111"), test.subContainer(16, 33));
        Assert::AreEqual(BitContainer("10011011010011110000101000010110"), test.subContainer(0, 32));
        Assert::AreEqual(BitContainer("000001011110101111001101"), test.subContainer(32, 24));
        Assert::AreEqual(BitContainer("00010110000001011110101111001101"), test.subContainer(24, 32));
        Assert::AreEqual(BitContainer("111001101"), test.subContainer(47, 9));
        Assert::AreEqual(BitContainer("10011011010011110000101000010110000001011110101111001101"), test.subContainer(0, 56));
        Assert::ExpectException<std::out_of_range>([&test] { test.subContainer(0, 57); });
        Assert::ExpectException<std::out_of_range>([&test] { test.subContainer(30, 30); });

        test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                        "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                        "1111011001100101011010000110");
        Assert::AreEqual(BitContainer("1"), test.subContainer(0, 1));
        Assert::AreEqual(BitContainer("0"), test.subContainer(255, 1));
        Assert::AreEqual(BitContainer("0101111"), test.subContainer(3, 7));
        Assert::AreEqual(BitContainer("00010010110001110"), test.subContainer(25, 17));
        Assert::AreEqual(BitContainer("1"), test.subContainer(31, 1));
        Assert::AreEqual(BitContainer("0110001110"), test.subContainer(32, 10));
        Assert::AreEqual(BitContainer("11010000100101100011101110111010"), test.subContainer(20, 32));
        Assert::AreEqual(BitContainer("01100011101110111010010000011001"), test.subContainer(32, 32));
        Assert::AreEqual(BitContainer("0110001110111011101001000001100110101011111110011101011100101110"), test.subContainer(32, 64));
        Assert::AreEqual(BitContainer("1110111001101110100001001011000111011101110100100000110011010101"), test.subContainer(7, 64));
        Assert::AreEqual(BitContainer("1110111001101110100001001011000111011101110100100000110011010101111111001110101110010111010111101000"
                                      "1111101010011111010000101100"),
            test.subContainer(7, 128));
        Assert::AreEqual(BitContainer("0101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                                      "1111011001100101011010000110"),
            test.subContainer(128, 128));
        Assert::AreEqual(BitContainer("1110101111011100110111010000100101100011101110111010010000011001101010111111100111010111001011101011"
                                      "1101000111110101001111101000010110011100011110000110110000000010111000011111110010101110110101001111"
                                      "00001011010101100010101000011111011001100101011010000110"),
            test.subContainer(0, 256));
        Assert::ExpectException<std::out_of_range>([&test] { test.subContainer(0, 257); });
        Assert::ExpectException<std::out_of_range>([&test] { test.subContainer(100, 157); });
    }

    TEST_METHOD(ToLengthTest)
    {
        BitContainer test("10110101011101101101010110");
        Assert::AreEqual(BitContainer("1"), test.toLength(1));
        Assert::AreEqual(BitContainer("1011010101110"), test.toLength(13));
        Assert::AreEqual(BitContainer("10110101011101101101010110"), test.toLength(26));
        Assert::AreEqual(BitContainer("00010110101011101101101010110"), test.toLength(29));
        Assert::AreEqual(BitContainer("00000010110101011101101101010110"), test.toLength(32));
        Assert::AreEqual(BitContainer("0000000010110101011101101101010110"), test.toLength(34));

        test.fromString("10011011010011110000101000010110");
        test.fromString("10011011010011110000101000010110000001011110101111001101");
        test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                        "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                        "1111011001100101011010000110");
    }
};

// TEST_CLASS(UnitTestProject2)
//{
// public:
//     TEST_METHOD(GetNumTest3){};
//
//     TEST_METHOD(GetNumTest4){};
// };
} // namespace UnitTestProject
