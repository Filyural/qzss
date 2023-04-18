#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../SignalDecodingQZSSLib/framework.h"

using std::cin;
using std::cout;
using std::endl;

using namespace Bits;

int main()
{
    const int MESSAGE_LENGTH = 1776;
    const int FULL_MESSAGE_LENGTH = 1800;

    BitContainer bc = readFileToBC("C:\\files\\lex_2023-03-06.log");
    BitContainer preamble("11010011");
    BitContainer crc_polynomial("1100001100100110011111011");
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

    try
    {

        for (size_t i = 0; i < 1000/*preamble_indexes.size()*/; ++i)
        {
            preamble_index = preamble_indexes[i];
            if (CheckCRC(bc.subContainer(preamble_index, FULL_MESSAGE_LENGTH), crc_polynomial, MESSAGE_LENGTH))
            {
                checked_preambles.push_back(preamble_indexes[i]);
            }
        }
    }
    catch (const std::out_of_range&)
    {
        //временно для обработки последних найденных индексов
    }

    // preamble_index = preamble_indexes[preamble_indexes.size() - 1];
    // BitContainer debug_message = bc.subContainer(preamble_index, FULL_MESSAGE_LENGTH);
    // CheckCRC(debug_message, crc_polynomial, MESSAGE_LENGTH);

    for (size_t i = 0; i < checked_preambles.size(); i++)
    {
        size_t message_begin_index = checked_preambles[i] + 81;
        BitContainer message_number = bc.subContainer(message_begin_index, 12);
        if (message_number == BitContainer("111111101001"))
        {
            bc.subContainer(message_begin_index + 12, 4).show(4);

        }
        else
        {
            cout << "++++" << endl;
        }
    }
}
// 110100110000000011011011111111010