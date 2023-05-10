#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include "../SignalDecodingQZSSLib/framework.h"

using std::cin;
using std::cout;
using std::endl;

int main()
{
    const int MESSAGE_LENGTH = 1776;
    const int FULL_MESSAGE_LENGTH = 1800;

    std::string output_info;
    size_t file_size;
    BitContainer bc = readFileToBC("C:\\files\\lex_2023-03-06.log", file_size);

    std::ofstream fout("output.txt");
    fout << "File size (in bits):\t" << file_size << std::endl;
    BitContainer preamble("11010011");
    BitContainer crc_polynomial("1100001100100110011111011");

    // std::vector<size_t> preamble_indexes = KMP(bc, preamble);
    // std::vector<size_t> checked_preambles(0);
    // size_t preamble_index;

    ////здесь проверка CRC (можно указать любое число до preamble_indexes.size() ~ 8000+)
    // for (size_t i = 0; i < 1000 /*preamble_indexes.size()*/; ++i)
    //{
    //     preamble_index = preamble_indexes[i];
    //     if (preamble_index + FULL_MESSAGE_LENGTH > bc.size())
    //     {
    //         break;
    //     }
    //     BitContainer probable_message = bc.subContainer(preamble_index, FULL_MESSAGE_LENGTH);
    //     if (checkCRC(probable_message, crc_polynomial, MESSAGE_LENGTH))
    //     {
    //         checked_preambles.push_back(preamble_index);
    //     }
    // }

    size_t preamble_index;
    while (true)
    {
        if (!KMP(bc, preamble, preamble_index))
        {
            fout << "NO PREAMBLES HAS BEEN FOUNDED" << endl;
            return 0;
        }

        BitContainer probable_message = bc.subContainer(preamble_index, FULL_MESSAGE_LENGTH);
        if (checkCRC(probable_message, crc_polynomial, MESSAGE_LENGTH))
        {
            //founded good preamble_index
            break;
        }
    }


    // preamble_index = preamble_indexes[preamble_indexes.size() - 1];
    // BitContainer debug_message = bc.subContainer(preamble_index, FULL_MESSAGE_LENGTH);
    // CheckCRC(debug_message, crc_polynomial, MESSAGE_LENGTH);

    // for (size_t i = 0; i < checked_preambles.size(); i++)
    //{

    //    size_t message_begin_index = checked_preambles[i];

    //    //bc.subContainer(message_begin_index, 81).show(81);
    //    //cout << "[------][----][--------][----------][--][------------------][--][------][------]|" << endl;

    //    size_t data_begin_index = message_begin_index + 81; //начинается dataPart

    //    BitContainer header_part = bc.subContainer(message_begin_index, 81);
    //    HeaderRTCM header_RTCM(header_part);

    //    //можно закомментировать, тогда будет выводиться информация о хедерах по CLAS и QZNMA
    //    if (header_RTCM.getMessageType().vendor_ID != 2)
    //    {
    //        continue;
    //    }

    //    header_RTCM.showInfo();
    //    BitContainer message_number = bc.subContainer(data_begin_index, 12); // message_number у subType'ов должен быть 4073
    //    if (header_RTCM.getMessageType().vendor_ID == 2)
    //    {
    //        if (message_number == BitContainer("111111101001"))
    //        {
    //            char message_subtype = bc.subContainer(data_begin_index + 12, 4).getNum(); //покажем 4 бита - message sub type
    //            if (message_subtype == 1)
    //            {
    //                BitContainer message_one = bc.subContainer(data_begin_index, 1695);
    //                // message_one.show(message_one.size());
    //                SubTypeOne message_type_one(message_one);
    //                message_type_one.showInfo();
    //            }
    //            else
    //            {
    //                std::cout << "=================================== SUBTYPE " << (int)message_subtype
    //                          << " ===================================" << std::endl;
    //                std::cout << "================================= SUBTYPE " << (int)message_subtype
    //                          << " END =================================" << std::endl
    //                          << std::endl;
    //            }
    //        }
    //        else
    //        {
    //            //если это не начало сообщения, то выведет ++++
    //            //но это можно проверить в через RTCM header -> message_type -> subframe_indicator
    //            cout << "++++" << endl << endl;
    //        }
    //    }
    //}
}
// 110100110000000011011011111111010