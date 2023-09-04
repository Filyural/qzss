#include "pch.h"
#include "CppUnitTest.h"
#include "../SignalDecodingQZSSLib/BitContainer/BitContainer.h"

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
    TEST_METHOD(getNumTest)
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

    TEST_METHOD(subContainerTest)
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

    // TODO остальные тесты
    TEST_METHOD(toLengthTest)
    {
        BitContainer test("10110101011101101101010110");
        Assert::AreEqual(BitContainer("1"), test.toLength(1));
        Assert::AreEqual(BitContainer("1011010101110"), test.toLength(13));
        Assert::AreEqual(BitContainer("10110101011101101101010110"), test.toLength(26));
        Assert::AreEqual(BitContainer("00010110101011101101101010110"), test.toLength(29));
        Assert::AreEqual(BitContainer("00000010110101011101101101010110"), test.toLength(32));
        Assert::AreEqual(BitContainer("0000000010110101011101101101010110"), test.toLength(34));
        Assert::AreEqual(BitContainer("0000000000000000000000000000000000000010110101011101101101010110"), test.toLength(64));
        Assert::AreEqual(BitContainer("0000000000000000000000000000000000000000010110101011101101101010110"), test.toLength(67));

        test.fromString("10011011010011110000101000010110");
        Assert::AreEqual(BitContainer("1"), test.toLength(1));
        Assert::AreEqual(BitContainer("1001101101001"), test.toLength(13));
        Assert::AreEqual(BitContainer("10011011010011110000101000"), test.toLength(26));
        Assert::AreEqual(BitContainer("10011011010011110000101000010"), test.toLength(29));
        Assert::AreEqual(BitContainer("10011011010011110000101000010110"), test.toLength(32));
        Assert::AreEqual(BitContainer("0010011011010011110000101000010110"), test.toLength(34));
        Assert::AreEqual(BitContainer("0000000000000000000000000000000010011011010011110000101000010110"), test.toLength(64));
        Assert::AreEqual(BitContainer("0000000000000000000000000000000000010011011010011110000101000010110"), test.toLength(67));

        // TODO
        test.fromString("10011011010011110000101000010110000001011110101111001101");
        Assert::AreEqual(BitContainer("1"), test.toLength(1));
        Assert::AreEqual(BitContainer("1001101101001"), test.toLength(13));
        Assert::AreEqual(BitContainer("10011011010011110000101000"), test.toLength(26));
        Assert::AreEqual(BitContainer("10011011010011110000101000010"), test.toLength(29));
        Assert::AreEqual(BitContainer("10011011010011110000101000010110"), test.toLength(32));
        Assert::AreEqual(BitContainer("1001101101001111000010100001011000"), test.toLength(34));
        Assert::AreEqual(BitContainer("0000000010011011010011110000101000010110000001011110101111001101"), test.toLength(64));
        Assert::AreEqual(BitContainer("0000000000010011011010011110000101000010110000001011110101111001101"), test.toLength(67));
        test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                        "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                        "1111011001100101011010000110");
    }
    TEST_METHOD(equalsTest)
    {
        BitContainer test("10110101011101101101010110");
        Assert::IsTrue(test.equals(BitContainer("10110101011101101101010110")));
        Assert::IsTrue(test.equals(BitContainer("000010110101011101101101010110")));
        Assert::IsTrue(test.equals(BitContainer("0000000000000010110101011101101101010110")));
        Assert::IsTrue(test.equals(BitContainer("00000000000000000000000000000000000000010110101011101101101010110")));
        Assert::IsTrue(
            test.equals(BitContainer("0000000000000000000000000000000000000000000000000000000000000000000000010110101011101101101010110")));
        Assert::IsTrue(
            test.equals(BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                     "000000000000000000000000000000010110101011101101101010110")));
        Assert::IsFalse(test.equals(BitContainer("1011011001011101101101010110")));
        Assert::IsFalse(test.equals(BitContainer("0000101110101011101101101010110")));
        Assert::IsFalse(test.equals(BitContainer("00000100010110101011101101101010110")));
        Assert::IsFalse(test.equals(BitContainer("00000000000000000000000000000000000000011110101011101101101010110")));
        Assert::IsFalse(
            test.equals(BitContainer("0000000000000000000000000000000001100000000000000000000000000000000000010110101011101101101010110")));
        Assert::IsFalse(
            test.equals(BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                     "00000000000000000000000001100000000000000000000000000000000000000000000000000000000000000000000000000"
                                     "000000000000000000000000000000010110101011101101101010110")));


        test.fromString("10011011010011110000101000010110");
        Assert::IsTrue(test.equals(BitContainer("10011011010011110000101000010110")));
        Assert::IsTrue(test.equals(BitContainer("00010011011010011110000101000010110")));
        Assert::IsTrue(test.equals(BitContainer("00000000010011011010011110000101000010110")));
        Assert::IsTrue(test.equals(BitContainer("000000000000000000000000000000000010011011010011110000101000010110")));
        Assert::IsTrue(
            test.equals(BitContainer("000000000000000000000000000000000000000000000000000000000000010011011010011110000101000010110")));
        Assert::IsTrue(test.equals(
            BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "000000000000000000000010011011010011110000101000010110")));

        test.fromString("10011011010011110000101000010110000001011110101111001101");
        Assert::IsTrue(test.equals(BitContainer("10011011010011110000101000010110000001011110101111001101")));
        Assert::IsTrue(test.equals(BitContainer("00010011011010011110000101000010110000001011110101111001101")));
        Assert::IsTrue(test.equals(BitContainer("00000000010011011010011110000101000010110000001011110101111001101")));
        Assert::IsTrue(
            test.equals(BitContainer("000000000000000000000000000000000010011011010011110000101000010110000001011110101111001101")));
        Assert::IsTrue(test.equals(BitContainer(
            "000000000000000000000000000000000000000000000000000000000000010011011010011110000101000010110000001011110101111001101")));
        Assert::IsTrue(test.equals(
            BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "000000000000000000000010011011010011110000101000010110000001011110101111001101")));
        test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                        "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                        "1111011001100101011010000110");
        Assert::IsTrue(test.equals(BitContainer("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100"
                                                "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
                                                "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
        Assert::IsTrue(
            test.equals(BitContainer("000111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100"
                                     "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
                                     "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
        Assert::IsTrue(
            test.equals(BitContainer("000000000111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100"
                                     "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
                                     "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
        Assert::IsTrue(test.equals(BitContainer(
            "0000000000000000000000000000000000111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100"
            "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
            "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
        Assert::IsTrue(test.equals(BitContainer("000000000000000000000000000000000000000000000000000000000000011101011110111001101110100001"
                                                "0010110001110111011101001000001100110101011111110011101011100"
                                                "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
                                                "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
        Assert::IsTrue(test.equals(
            BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                         "00000000000000000000001110101111011100110111010000100101100011101110111010010000011001101010111111100111010111001"
                         "01110101111010001111101010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000"
                         "1011010101100010101000011111011001100101011010000110")));
    }
    TEST_METHOD(operatorXORTest)
    {
        BitContainer result;
        result = BitContainer("10110101011101101101010110") ^ BitContainer("10110101011101101101010110");
        Assert::AreEqual(BitContainer("00000000000000000000000000"), result);
        result = BitContainer("101") ^ BitContainer("001");
        Assert::AreEqual(BitContainer("100"), result);
        //Assert::AreEqual(BitContainer("00000000000000000000000000"), result);
        BitContainer container1("10110101011101101101010110");
        BitContainer container2("11010111010101010101110010");
        BitContainer expectedResult1("01100010001000111000100100");
        Assert::AreEqual(expectedResult1, container1 ^ container2);

        BitContainer container3("110101011101101101010110");
        BitContainer container4("10110101011101101101010110");
        Assert::ExpectException<std::invalid_argument>([&]() { container3 ^ container4; });

        BitContainer container5("000110101010110");
        BitContainer container6("000101010101101");
        BitContainer expectedResult3("000011111111011");
        Assert::AreEqual(expectedResult3, container5 ^ container6);

        BitContainer container7("000110101010110");
        BitContainer container8("00011101010110");
        Assert::ExpectException<std::invalid_argument>([&]() { container7 ^ container8; });

        BitContainer container9("1" + std::string(259, '0'));
        BitContainer container10("0" + std::string(259, '1'));
        BitContainer expectedResult5("1" + std::string(259, '1'));
        Assert::AreEqual(expectedResult5, container9 ^ container10);
    }

    TEST_METHOD(trimLeadingZerosTest)
    {
        BitContainer result;
        result = BitContainer("0000000000000010110101011101101101010110");
        result.trimLeadingZeros();
        Assert::AreEqual(BitContainer("10110101011101101101010110"), result);
        // Test 1: No leading zeros
        BitContainer result1("10110101011101101101010110");
        result1.trimLeadingZeros();
        Assert::AreEqual(BitContainer("10110101011101101101010110"), result1);

        // Test 2: All leading zeros
        BitContainer result2("0000000000000000000000000000");
        result2.trimLeadingZeros();
        Assert::AreEqual(BitContainer(""), result2);

        // Test 3: Leading zeros and ones
        BitContainer result3("0000011010101011101101101010110");
        result3.trimLeadingZeros();
        Assert::AreEqual(BitContainer("11010101011101101101010110"), result3);

        // Test 4: Only one bit
        BitContainer result4("0");
        result4.trimLeadingZeros();
        Assert::AreEqual(BitContainer(""), result4);

        // Test 5: No leading zeros, different length
        BitContainer result5("1110101011101101101010110");
        result5.trimLeadingZeros();
        Assert::AreEqual(BitContainer("1110101011101101101010110"), result5);
    }
};
} // namespace UnitTestProject
