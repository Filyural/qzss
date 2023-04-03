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
    BitContainer bc = ReadFileToBC("C:\\files\\lex_2023-03-06.log");

    cout << bc.size();
    cout << endl;

    for (size_t i = 0; i < bc.size(); i++)
    {
        if (i % 100 == 0)
        {
            cout << endl;
        }
        cout << bc.get(i);
    }
}
// 110100110000000011011011111111010