#include "pch.h"
#include "BitContainerTest.h"

bool BitContainerTest::test_ALL()
{
    return test_add() && test_subContainer() && test_toLength() && test_getNum() && test_trimLeadingZeros() && test_equals() &&
           test_operatorXOR() && test_operatorEQUAL();
}

bool BitContainerTest::test_add()
{
    bool result = true;
    BitContainer test1("1011010101110");
    BitContainer test2("1101101010111");
    test1.add(test2);
    result &= (BitContainer("10110101011101101101010111") == test1);

    test1.fromString("");
    test1.add(test2);
    result &= (test2 == test1);

    test1.fromString("");
    test2.fromString("");
    result &= (test1 == test2);
    test1.fromString("00000000");
    test2.fromString("10110101011101101101010110");
    test1.add(test2);
    result &= (BitContainer("0000000010110101011101101101010110") == test1);

    test1.fromString("10011011010011110000101000010110");
    test2.fromString("10011011010011110000101000010110000001011110101111001101");
    test1.add(test2);
    result &= (BitContainer("1001101101001111000010100001011010011011010011110000101000010110000001011110101111001101") == test1);

    test1.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                     "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                     "1111011001100101011010000110");
    test2.fromString("10011011010011110000101000010110000001011110101111001101");
    test1.add(test2);
    result &=
        (BitContainer("11101011110111001101110100001001011000111011101110100100000110011010101111111001110101110010111010111101000111110"
                      "10100111110100001011001110001111000011011000000001011100001111111001010111011010100111100001011010101100010101000"
                      "01111101100110010101101000011010011011010011110000101000010110000001011110101111001101") == test1);

    test1.fromString("000");
    test2.fromString("111");
    test1.add(test2);
    result &= (BitContainer("000111") == test1);
    return result;
}

bool BitContainerTest::test_subContainer()
{
    bool result = true;
    BitContainer test("10110101011101101101010110");
    result &= (BitContainer("1") == test.subContainer(0, 1));
    result &= (BitContainer("0") == test.subContainer(25, 1));
    result &= (BitContainer("11011011") == test.subContainer(10, 8));
    result &= (BitContainer("10110101011101101101010110") == test.subContainer(0, 26));

    // must throw exception
    try
    {
        test.subContainer(0, 30);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.subContainer(10, 20);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }

    test.fromString("10011011010011110000101000010110");
    result &= (BitContainer("1") == test.subContainer(0, 1));
    result &= (BitContainer("0") == test.subContainer(31, 1));
    result &= (BitContainer("0111100001010") == test.subContainer(11, 13));
    result &= (BitContainer("10000101000010110") == test.subContainer(15, 17));
    result &= (BitContainer("10011011010011110000101000010110") == test.subContainer(0, 32));
    // must throw exception
    try
    {
        test.subContainer(0, 33);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.subContainer(30, 4);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }

    test.fromString("10011011010011110000101000010110000001011110101111001101");
    result &= (BitContainer("1") == test.subContainer(0, 1));
    result &= (BitContainer("1") == test.subContainer(55, 1));
    result &= (BitContainer("0001011110101111") == test.subContainer(34, 16));
    result &= (BitContainer("00001010000101100000010111101011") == test.subContainer(16, 32));
    result &= (BitContainer("000010100001011000000101111010111") == test.subContainer(16, 33));
    result &= (BitContainer("10011011010011110000101000010110") == test.subContainer(0, 32));
    result &= (BitContainer("000001011110101111001101") == test.subContainer(32, 24));
    result &= (BitContainer("00010110000001011110101111001101") == test.subContainer(24, 32));
    result &= (BitContainer("111001101") == test.subContainer(47, 9));
    result &= (BitContainer("10011011010011110000101000010110000001011110101111001101") == test.subContainer(0, 56));
    // must throw exception
    try
    {
        test.subContainer(0, 57);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.subContainer(30, 30);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }

    test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                    "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                    "1111011001100101011010000110");
    result &= (BitContainer("1") == test.subContainer(0, 1));
    result &= (BitContainer("0") == test.subContainer(255, 1));
    result &= (BitContainer("0101111") == test.subContainer(3, 7));
    result &= (BitContainer("00010010110001110") == test.subContainer(25, 17));
    result &= (BitContainer("1") == test.subContainer(31, 1));
    result &= (BitContainer("0110001110") == test.subContainer(32, 10));
    result &= (BitContainer("11010000100101100011101110111010") == test.subContainer(20, 32));
    result &= (BitContainer("01100011101110111010010000011001") == test.subContainer(32, 32));
    result &= (BitContainer("0110001110111011101001000001100110101011111110011101011100101110") == test.subContainer(32, 64));
    result &= (BitContainer("1110111001101110100001001011000111011101110100100000110011010101") == test.subContainer(7, 64));
    result &= (BitContainer("1110111001101110100001001011000111011101110100100000110011010101111111001110101110010111010111101000"
                            "1111101010011111010000101100") == test.subContainer(7, 128));
    result &= (BitContainer("0101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                            "1111011001100101011010000110") == test.subContainer(128, 128));
    result &= (BitContainer("1110101111011100110111010000100101100011101110111010010000011001101010111111100111010111001011101011"
                            "1101000111110101001111101000010110011100011110000110110000000010111000011111110010101110110101001111"
                            "00001011010101100010101000011111011001100101011010000110") == test.subContainer(0, 256));
    // must throw exception
    try
    {
        test.subContainer(0, 257);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.subContainer(100, 157);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
}

bool BitContainerTest::test_toLength()
{
    bool result = true;
    BitContainer test("10110101011101101101010110");
    result &= (BitContainer("1") == test.toLength(1));
    result &= (BitContainer("1011010101110") == test.toLength(13));
    result &= (BitContainer("10110101011101101101010110") == test.toLength(26));
    result &= (BitContainer("00010110101011101101101010110") == test.toLength(29));
    result &= (BitContainer("00000010110101011101101101010110") == test.toLength(32));
    result &= (BitContainer("0000000010110101011101101101010110") == test.toLength(34));
    result &= (BitContainer("0000000000000000000000000000000000000010110101011101101101010110") == test.toLength(64));
    result &= (BitContainer("0000000000000000000000000000000000000000010110101011101101101010110") == test.toLength(67));

    test.fromString("10011011010011110000101000010110");
    result &= (BitContainer("1") == test.toLength(1));
    result &= (BitContainer("1001101101001") == test.toLength(13));
    result &= (BitContainer("10011011010011110000101000") == test.toLength(26));
    result &= (BitContainer("10011011010011110000101000010") == test.toLength(29));
    result &= (BitContainer("10011011010011110000101000010110") == test.toLength(32));
    result &= (BitContainer("0010011011010011110000101000010110") == test.toLength(34));
    result &= (BitContainer("0000000000000000000000000000000010011011010011110000101000010110") == test.toLength(64));
    result &= (BitContainer("0000000000000000000000000000000000010011011010011110000101000010110") == test.toLength(67));

    test.fromString("10011011010011110000101000010110000001011110101111001101");
    result &= (BitContainer("1") == test.toLength(1));
    result &= (BitContainer("1001101101001") == test.toLength(13));
    result &= (BitContainer("10011011010011110000101000") == test.toLength(26));
    result &= (BitContainer("10011011010011110000101000010") == test.toLength(29));
    result &= (BitContainer("10011011010011110000101000010110") == test.toLength(32));
    result &= (BitContainer("1001101101001111000010100001011000") == test.toLength(34));
    result &= (BitContainer("10011011010011110000101000010110000001011110101111001101") == test.toLength(56));
    result &= (BitContainer("0000000010011011010011110000101000010110000001011110101111001101") == test.toLength(64));
    result &= (BitContainer("0000000000010011011010011110000101000010110000001011110101111001101") == test.toLength(67));

    test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                    "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                    "1111011001100101011010000110");

    result &= (BitContainer("1") == test.toLength(1));
    result &= (BitContainer("11101011110111") == test.toLength(14));
    result &= (BitContainer("11101011110111001101110100001001011000111011") == test.toLength(44));
    result &= (BitContainer("1110101111011100110111010000100101100011101110111010010000011001101010111111100111010111001011") ==
               test.toLength(94));
    result &= (BitContainer(
                   "111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101") ==
               test.toLength(114));
    result &= (BitContainer("11101011110111001101110100001001011000111011101110100100000110011010101111111001110101110010111010111101000111"
                            "11010100111110100001011001110001111000011011000000001011100001111111001010111011010100111100001011010101100010"
                            "101000011111011001100101011010000110") == test.toLength(256));
    result &=
        (BitContainer("00011101011110111001101110100001001011000111011101110100100000110011010101111111001110101110010111010111101000111"
                      "11010100111110100001011001110001111000011011000000001011100001111111001010111011010100111100001011010101100010"
                      "101000011111011001100101011010000110") == test.toLength(259));
    result &= (BitContainer("00000000000000000111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101"
                            "11010111101000111"
                            "11010100111110100001011001110001111000011011000000001011100001111111001010111011010100111100001011010101100010"
                            "101000011111011001100101011010000110") == test.toLength(273));
    result &= (BitContainer("00000000000000000000000000000000000000000000000000000000000000001110101111011100110111010000100101100011101110"
                            "1110100100000110011010101111111001110101110010111010111101000111"
                            "11010100111110100001011001110001111000011011000000001011100001111111001010111011010100111100001011010101100010"
                            "101000011111011001100101011010000110") == test.toLength(320));
    return result;
}

bool BitContainerTest::test_getNum()
{
    bool result = true;
    BitContainer test("1011010101110");
    result &= (0b10110UL == test.getNum(0, 5));
    result &= (0b1010101UL == test.getNum(3, 7));
    result &= (0b010101UL == test.getNum(4, 6));
    result &= (0b1011010101110UL == test.getNum(0, 13));
    result &= (0b1UL == test.getNum(0, 1));
    result &= (0b0UL == test.getNum(12, 1));
    // отрицательное значение size_t не бывает и конвертируется сразу в положительное
    // Assert::ExpectException<std::out_of_range>([&test] { test.getNum(-1, 2); });

    try
    {
        test.getNum(0, 14);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(5, 9);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }

    // exactly 1 unsigned long
    test.fromString("10011011010011110000101000010110");
    result &= (0b1001101101001111000UL == test.getNum(0, 19));
    result &= (0b11010011UL == test.getNum(6, 8));
    result &= (0b10011011010011110000101000010110UL == test.getNum(0, 32));
    result &= (0b1UL == test.getNum(0, 1));
    result &= (0b0UL == test.getNum(31, 1));
    try
    {
        test.getNum(0, 65);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(31, 5);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(32, 1);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(30, 0);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(32, 0);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }

    // more than 1 unsigned long and less than 2
    test.fromString("10011011010011110000101000010110000001011110101111001101");
    result &= (0b1001101101001111UL == test.getNum(0, 16));
    result &= (0b10110100111100001010000UL == test.getNum(4, 23));
    result &= (0b000101111010111100110UL == test.getNum(34, 21));
    result &= (0b10011011010011110000101000010110UL == test.getNum(0, 32));
    result &= (0b000001011110101111001101UL == test.getNum(32, 24));
    result &= (0b001011000000101UL == test.getNum(25, 15));
    result &= (0b1UL == test.getNum(0, 1));
    result &= (0b1UL == test.getNum(55, 1));
    try
    {
        test.getNum(0, 65);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(32, 0);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(56, 3);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(56, 0);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }

    // more than 2 unsigned long
    test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                    "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                    "1111011001100101011010000110");
    result &= (0b1110101111011100110UL == test.getNum(0, 19));
    result &= (0b100011101110111010010UL == test.getNum(34, 21));
    result &= (0b1001000001100110101011111UL == test.getNum(50, 25));
    result &= (0b1110100001011001110001111UL == test.getNum(120, 25));
    result &= (0b011111011001100101011010000110UL == test.getNum(226, 30));
    result &= (0b00011111011001100101011010000110UL == test.getNum(224, 32));
    result &= (0b1UL == test.getNum(0, 1));
    result &= (0b0UL == test.getNum(255, 1));
    try
    {
        test.getNum(0, 65);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(240, 40);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(256, 0);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(256, 1);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    try
    {
        test.getNum(255, 2);
        return false;
    }
    catch (const std::out_of_range&)
    {
    }
    return result;
}

bool BitContainerTest::test_trimLeadingZeros()
{
    bool result = true;
    BitContainer container = BitContainer("0000000000000010110101011101101101010110");
    container.trimLeadingZeros();
    result &= (BitContainer("10110101011101101101010110") == container);

    container = BitContainer("10110101011101101101010110");
    container.trimLeadingZeros();
    result &= (BitContainer("10110101011101101101010110") == container);

    container = BitContainer("0000000000000000000000000000");
    container.trimLeadingZeros();
    result &= (BitContainer("") == container);

    container = BitContainer("0000011010101011101101101010110");
    container.trimLeadingZeros();
    result &= (BitContainer("11010101011101101101010110") == container);

    container = BitContainer("0");
    container.trimLeadingZeros();
    result &= (BitContainer("") == container);

    container = BitContainer("1110101011101101101010110");
    container.trimLeadingZeros();
    result &= (BitContainer("1110101011101101101010110") == container);
    return result;
}

bool BitContainerTest::test_equals()
{
    bool result = true;
    BitContainer test("10110101011101101101010110");
    result &= (test.equals(BitContainer("10110101011101101101010110")));
    result &= (test.equals(BitContainer("000010110101011101101101010110")));
    result &= (test.equals(BitContainer("0000000000000010110101011101101101010110")));
    result &= (test.equals(BitContainer("00000000000000000000000000000000000000010110101011101101101010110")));
    result &=
        (test.equals(BitContainer("0000000000000000000000000000000000000000000000000000000000000000000000010110101011101101101010110")));
    result &=
        (test.equals(BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                  "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                  "000000000000000000000000000000010110101011101101101010110")));
    result &= (!test.equals(BitContainer("1011011001011101101101010110")));
    result &= (!test.equals(BitContainer("0000101110101011101101101010110")));
    result &= (!test.equals(BitContainer("00000100010110101011101101101010110")));
    result &= (!test.equals(BitContainer("00000000000000000000000000000000000000011110101011101101101010110")));
    result &=
        (!test.equals(BitContainer("0000000000000000000000000000000001100000000000000000000000000000000000010110101011101101101010110")));
    result &=
        (!test.equals(BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                   "00000000000000000000000001100000000000000000000000000000000000000000000000000000000000000000000000000"
                                   "000000000000000000000000000000010110101011101101101010110")));

    test.fromString("10011011010011110000101000010110");
    result &= (test.equals(BitContainer("10011011010011110000101000010110")));
    result &= (test.equals(BitContainer("00010011011010011110000101000010110")));
    result &= (test.equals(BitContainer("00000000010011011010011110000101000010110")));
    result &= (test.equals(BitContainer("000000000000000000000000000000000010011011010011110000101000010110")));
    result &= (test.equals(BitContainer("000000000000000000000000000000000000000000000000000000000000010011011010011110000101000010110")));
    result &= (test.equals(
        BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                     "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                     "000000000000000000000010011011010011110000101000010110")));

    test.fromString("10011011010011110000101000010110000001011110101111001101");
    result &= (test.equals(BitContainer("10011011010011110000101000010110000001011110101111001101")));
    result &= (test.equals(BitContainer("00010011011010011110000101000010110000001011110101111001101")));
    result &= (test.equals(BitContainer("00000000010011011010011110000101000010110000001011110101111001101")));
    result &= (test.equals(BitContainer("000000000000000000000000000000000010011011010011110000101000010110000001011110101111001101")));
    result &= (test.equals(BitContainer(
        "000000000000000000000000000000000000000000000000000000000000010011011010011110000101000010110000001011110101111001101")));
    result &= (test.equals(
        BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                     "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                     "000000000000000000000010011011010011110000101000010110000001011110101111001101")));
    test.fromString("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100101110101111010001111101"
                    "010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000101101010110001010100001"
                    "1111011001100101011010000110");
    result &= (test.equals(BitContainer("111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100"
                                        "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
                                        "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
    result &= (test.equals(BitContainer("000111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100"
                                        "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
                                        "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
    result &=
        (test.equals(BitContainer("000000000111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100"
                                  "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
                                  "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
    result &= (test.equals(BitContainer(
        "0000000000000000000000000000000000111010111101110011011101000010010110001110111011101001000001100110101011111110011101011100"
        "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
        "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
    result &= (test.equals(BitContainer("000000000000000000000000000000000000000000000000000000000000011101011110111001101110100001"
                                        "0010110001110111011101001000001100110101011111110011101011100"
                                        "101110101111010001111101010011111010000101100111000111100001101100000000101110000111111100"
                                        "1010111011010100111100001011010101100010101000011111011001100101011010000110")));
    result &= (test.equals(
        BitContainer("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                     "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                     "00000000000000000000001110101111011100110111010000100101100011101110111010010000011001101010111111100111010111001"
                     "01110101111010001111101010011111010000101100111000111100001101100000000101110000111111100101011101101010011110000"
                     "1011010101100010101000011111011001100101011010000110")));

    return result;
}

bool BitContainerTest::test_operatorXOR()
{
    bool result = true;
    BitContainer container1("10110101011101101101010110");
    BitContainer container2("11010111010101010101110010");
    BitContainer expectedResult1("01100010001000111000100100");
    result &= (expectedResult1 == (container1 ^ container2));

    BitContainer container3("110101011101101101010110");
    BitContainer container4("10110101011101101101010110");
    try
    {
        container3 ^ container4;
        result &= false;
    }
    catch (const std::invalid_argument& ex)
    {
    }

    BitContainer container5("000110101010110");
    BitContainer container6("000101010101101");
    BitContainer expectedResult3("000011111111011");
    result &= (expectedResult3 == (container5 ^ container6));

    BitContainer container7("000110101010110");
    BitContainer container8("00011101010110");
    try
    {
        container7 ^ container8;
        result &= false;
    }
    catch (const std::invalid_argument& ex)
    {
    }

    BitContainer container9("1" + std::string(259, '0'));
    BitContainer container10("0" + std::string(259, '1'));
    BitContainer expectedResult5("1" + std::string(259, '1'));
    result &= (expectedResult5 == (container9 ^ container10));

    BitContainer container11("1" + std::string(259, '0'));
    BitContainer container12("1" + std::string(259, '0'));
    BitContainer expectedResult6("0" + std::string(259, '0'));
    result &= (expectedResult6 == (container11 ^ container12));

    return result;
}

bool BitContainerTest::test_operatorEQUAL()
{
    bool result = true;
    BitContainer container1("10110101011101101101010110");
    BitContainer container2("10110101011101101101010110");
    result &= (container1 == container2);

    BitContainer container3("110101011101101101010110");
    BitContainer container4("10110101011101101101010110");
    result &= !(container3 == container4);

    BitContainer container5("000110101010110");
    BitContainer container6("110101010110");
    result &= !(container5 == container6);

    BitContainer container7("000110101010110");
    BitContainer container8("1110101010110");
    result &= !(container7 == container8);

    BitContainer container9("1110101011101101101010110");
    BitContainer container10("1110101011101101101010110" + std::string(50, '0'));
    result &= !(container9 == container10);

    BitContainer container11("1" + std::string(259, '0'));
    BitContainer container12("1" + std::string(259, '0'));
    result &= (container11 == container12);

    BitContainer container13("1" + std::string(258, '0') + "1");
    BitContainer container14("1" + std::string(259, '0'));
    result &= !(container13 == container14);

    return result;
}
