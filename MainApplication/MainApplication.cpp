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

    // BitContainer bc(10);
    // bc.fromString("1101011010");

    BitContainer bc = readFileToBC("C:\\files\\lex_2023-03-06.log");
    BitContainer preamble("11010011");
    BitContainer crc_polynomial("1100001100100110011111011");
    //BitContainer test_sequence("1010111010");
    //BitContainer test_crc_polynomial("101001");
    BitContainer test_sequence = bc.subContainer(0, 1776);
    BitContainer test_crc_polynomial("1100001100100110011111011");

    BitContainer test_result = CalculateCRC(test_sequence, test_crc_polynomial);
    // DEBUG

    cout << endl;
    cout << "Result size:\t" << test_result.size();
    for (size_t i = 0; i < test_result.size(); i++)
    {
        if (i % 100 == 0)
        {
            cout << endl;
        }
        cout << test_result.get(i);
    }
    cout << endl;

    BitContainer crc = bc.subContainer(1776, 24);
    for (size_t i = 0; i < 24; i++)
    {
        if (i % 100 == 0)
        {
            cout << endl;
        }
        cout << crc.get(i);
    }
    cout << endl << endl;

    //std::vector<size_t> preamble_indexes = KMP(bc, preamble);

    //for (size_t i = 0; i < 100 /*preamble_indexes.size()*/; i++)
    //{
    //    cout << "Index of expected preamle: " << preamble_indexes[i] << endl;
    //}
}
// 110100110000000011011011111111010