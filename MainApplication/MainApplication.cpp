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
    BitContainer bc = readFileToBC("C:\\files\\lex_2023-03-06.log");
    BitContainer preamble{1, 1, 0, 1, 0, 0, 1, 1};

    cout << bc.size();
    cout << endl;

    // DEBUG

    // for (size_t i = 0; i < bc.size(); i++)
    //{
    //     if (i % 100 == 0)
    //     {
    //         cout << endl;
    //     }
    //     cout << bc.get(i);
    // }

    for (size_t i = 0; i < bc.size(); i++)
    {
        if (i % 100 == 0)
        {
            cout << endl;
        }
        cout << bc.get(i);
    }

    //KMP(bc, 
}
// 110100110000000011011011111111010