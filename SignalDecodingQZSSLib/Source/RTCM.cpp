#include "pch.h"
#include "../Headers/RTCM.h"
#include "../Headers/BitContainer.h"

RTCM::RTCM(const RTCM& rtcm)
{
    this->size = rtcm.size;
    this->btc = rtcm.btc;
}

RTCM::RTCM(BitContainer& const btc)
{
    if (btc.getNum(0, 8) != 0b11010011)
    {
        throw std::invalid_argument("RTCM preamble must be 11010011");
    }
    if (btc.getNum(8, 6) != 0)
    {
        throw std::invalid_argument("RTCM reserved bits incorrect");
    }
    
    size = btc.getNum(14, 10) * 8 + 48;
    if (size != btc.size())
    {
        throw std::invalid_argument("Container size != RTCM size");
    }
    if (!checkCRC(btc, BitContainer("1100001100100110011111011"), size - 24))
    {
        throw std::invalid_argument("RTCM CRC check failed");
    }
    this->btc = &(btc.subContainer(0, size));
}

BitContainer RTCM::calculateCRC(BitContainer message, const BitContainer& const polynomial)
{
    message.add(polynomial.size() - 1);
    size_t new_length = message.size();

    BitContainer sub_message = message.subContainer(0, polynomial.size());

    size_t size_difference;
    for (size_t i = polynomial.size();;)
    {
        sub_message = sub_message ^ polynomial;
        sub_message.trimLeadingZeros();
        size_difference = polynomial.size() - sub_message.size();

        if (i + size_difference > new_length)
        {
            if (i >= new_length)
            {
                return sub_message.toLength(polynomial.size() - 1);
            }
            sub_message.add(message.subContainer(i, message.size() - i));
            return sub_message.toLength(polynomial.size() - 1);
        }
        sub_message.add(message.subContainer(i, size_difference));
        i += size_difference;
    }
}

bool RTCM::checkCRC(const BitContainer& const sequence, const BitContainer& const polynomial, size_t crc_index)
{
    BitContainer message = sequence.subContainer(0, crc_index);
    BitContainer crc = sequence.subContainer(crc_index, sequence.size() - crc_index);

    BitContainer crc_calc = calculateCRC(message, polynomial);
    return crc == (crc_calc);
}

RTCM::~RTCM()
{

}
