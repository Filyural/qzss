#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../SignalDecodingQZSSLib/framework.h"

using std::cin;
using std::cout;
using std::endl;

int main()
{
    const int MESSAGE_LENGTH = 1776;
    const int FULL_MESSAGE_LENGTH = 1800;

    Bits::BitContainer bc = Bits::readFileToBC("C:\\files\\lex_2023-03-06.log");
    Bits::BitContainer preamble("11010011");
    Bits::BitContainer crc_polynomial("1100001100100110011111011");

    cout << crc_polynomial.getNum() << endl;
    // BitContainer test_sequence("1001001110010");
    // BitContainer test_crc_polynomial("10111");
    // BitContainer test_sequence = bc.subContainer(5400, 1776);
    // BitContainer test_crc_polynomial("1100001100100110011111011");
    // BitContainer test_result = CalculateCRC(test_sequence, test_crc_polynomial);
    // test_result.show(test_result.size());
    //  DEBUG

    // cout << ((CheckCRC(bc.subContainer(5400, 1800), crc_polynomial, MESSAGE_LENGTH)) ? "True" : "False") << endl;

    std::vector<size_t> preamble_indexes = KMP(bc, preamble);
    std::vector<size_t> checked_preambles(0);
    size_t preamble_index;

    //здесь проверка CRC (можно указать любое число до preamble_indexes.size() ~ 8000)
    for (size_t i = 0; i < 1000 /*preamble_indexes.size()*/; ++i)
    {
        preamble_index = preamble_indexes[i];
        if (preamble_index + FULL_MESSAGE_LENGTH > bc.size())
        {
            break;
        }
        if (CheckCRC(bc.subContainer(preamble_index, FULL_MESSAGE_LENGTH), crc_polynomial, MESSAGE_LENGTH))
        {
            checked_preambles.push_back(preamble_index);
        }
    }

    // preamble_index = preamble_indexes[preamble_indexes.size() - 1];
    // BitContainer debug_message = bc.subContainer(preamble_index, FULL_MESSAGE_LENGTH);
    // CheckCRC(debug_message, crc_polynomial, MESSAGE_LENGTH);

    for (size_t i = 0; i < checked_preambles.size(); i++)
    {
        size_t message_begin_index = checked_preambles[i] + 81;                       //начинается dataPart
        Bits::BitContainer message_number = bc.subContainer(message_begin_index, 12); // message_number у subType'ов должен быть 4073
        if (message_number == Bits::BitContainer("111111101001"))
        {
            char message_subtype = bc.subContainer(message_begin_index + 12, 4).getNum(); //покажем 4 бита - message sub type
            if (message_subtype == 1)
            {
                Bits::BitContainer message_one = bc.subContainer(message_begin_index, 1695);
                SubTypeOne message_type_one(message_one);
                message_type_one.showInfo();
            }
            else
            {
                cout << "Message subtype:" << (int)message_subtype << endl;
            }
        }
        else
        {
            cout << "++++" << endl; //если это не начало сообщения, то выведет ++++
        }
    }
}
// 110100110000000011011011111111010