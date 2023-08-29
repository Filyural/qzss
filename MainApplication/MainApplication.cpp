#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include "../SignalDecodingQZSSLib/framework.h"

using std::cin;
using std::cout;
using std::endl;

int main()
{
    std:: cout << BitContainer::testPass() << endl;
    return 0;
    // BitContainer test("10110101011101101101010110");
    // test.subContainer(0, 1);

    //BitContainer test(
    //    "1011010101110110110101011010101110110110101011011111111111011011010101101011010101110110110101011011111111111011011010101101011011"
    //    "1111111110110110110101011101101101010110111111111110110110101011010101011011111111111011011010101101010110");
    //auto start = std::chrono::high_resolution_clock::now();
    //for (size_t i = 0; i < 1000000; i++)
    //{
    //    test = test ^ test;
    //}
    //auto end = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<float> duration = end - start;
    //cout << "Duration " << duration.count() << endl;
    //return 0;

    const int MESSAGE_LENGTH = 1776;
    const int FULL_MESSAGE_LENGTH = 1800;

    std::string output_info;
    size_t file_size;
    BitContainer bc = readFileToBC("C:\\files\\lex_2023-03-06.log", file_size);

    std::ofstream fout("output.txt");
    fout << "File size (in bits):\t" << file_size << std::endl;
    BitContainer preamble("11010011");
    BitContainer crc_polynomial("1100001100100110011111011");

     //std::vector<size_t> preamble_indexes = KMP(bc, preamble);
     //std::vector<size_t> checked_preambles(0);
    // size_t preamble_index;
    // std::cout << preamble_indexes.size() << endl;
    ////здесь проверка CRC (можно указать любое число до preamble_indexes.size() ~ 8000+)
    // for (size_t i = 0; i < preamble_indexes.size(); ++i)
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
            // fout << probable_message.getInfo();
            // founded good preamble_index
            break;
        }
    }

    while (preamble_index <= file_size - FULL_MESSAGE_LENGTH)
    {

        size_t message_begin_index = preamble_index;
        BitContainer header_part = bc.subContainer(message_begin_index, 81);
        HeaderRTCM header_RTCM(header_part);

        // выводить только MADOCA-PPP
        if (header_RTCM.getMessageType().vendor_ID != 2)
        {
            preamble_index += FULL_MESSAGE_LENGTH;
            continue;
        }

        fout << bc.subContainer(message_begin_index, 81).getInfo(81);
        fout << "[------][----][--------][----------][--][------------------][--][------][------]|" << endl;
        fout << header_RTCM.getInfo();

        size_t data_begin_index = message_begin_index + 81; // начинается dataPart

        if (header_RTCM.getMessageType().vendor_ID == 2)
        {
            BitContainer message_number = bc.subContainer(data_begin_index, 12); // первые 12 бит
            if (message_number == BitContainer("111111101001"))
            {
                char message_subtype = bc.subContainer(data_begin_index + 12, 4).getNum(0, 4); // покажем 4 бита - message sub type
                if (message_subtype == 1)
                {
                    BitContainer message_one = bc.subContainer(data_begin_index, 1695);
                    // message_one.show(message_one.size());
                    SubTypeOne message_type_one(message_one);
                    fout << message_type_one.getInfo();
                }
                else
                {
                    fout << "=================================== SUBTYPE " << (int)message_subtype
                         << " ===================================" << std::endl;
                    fout << "================================= SUBTYPE " << (int)message_subtype
                         << " END =================================" << std::endl
                         << std::endl;
                }
            }
            else
            {

                // если это не начало сообщения, то выведет ++++
                // но это можно проверить в через RTCM header -> message_type -> subframe_indicator
                fout << "Not the beginning of the message!" << endl << endl;
            }
        }
        preamble_index += FULL_MESSAGE_LENGTH;
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