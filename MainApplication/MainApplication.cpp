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
    // cout << bc.size();
    // cout << endl;

    // DEBUG

    cout << endl;
    //for (size_t i = 13; i < 33; i++)
    //{
    //    if (i % 100 == 0)
    //    {
    //        cout << endl;
    //    }
    //    cout << bc.get(i);
    //}

    std::vector<size_t> preamble_indexes = KMP(bc, preamble);

    for (size_t i = 0; i < 100 /*preamble_indexes.size()*/; i++)
    {
        cout << "Index of expected preamle: " << preamble_indexes[i] << endl;
    }
}
// 110100110000000011011011111111010