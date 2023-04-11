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
    BitContainer test_sequence("1001001110010");
    BitContainer test_crc_polynomial("10111");
    //BitContainer test_sequence = bc.subContainer(5400, 1776);
    //BitContainer test_crc_polynomial("1100001100100110011111011");
    BitContainer test_result = CalculateCRC(test_sequence, test_crc_polynomial);
    
    // DEBUG

    //BitContainer crc = bc.subContainer(5376, 24);

    //test_result.show(test_result.size());
    //crc.show(crc.size());

    //cout << ((CheckCRC(bc.subContainer(5400, 1800), crc_polynomial, MESSAGE_LENGTH)) ? "True" : "False") << endl;

    std::vector<size_t> preamble_indexes = KMP(bc, preamble);

    size_t preamble_index;
    for (size_t i = 0; i < 20 /*preamble_indexes.size()*/; i++)
    {
        preamble_index = preamble_indexes[i];
        cout << "Index of expected preamle:\t" << preamble_index << endl;
        cout << "Preamble correct:\t\t" << CheckCRC(bc.subContainer(preamble_index, FULL_MESSAGE_LENGTH), crc_polynomial, MESSAGE_LENGTH) << endl << endl;
    }
}
// 110100110000000011011011111111010