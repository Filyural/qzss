#include "pch.h"

#include "../Headers/BitContainer.h"
#include "../Headers/HeaderRTCM.h"

HeaderRTCM::HeaderRTCM()
{
    time_of_week = 0;
    correction_error_bits = 0;
    PRN = 0;
    message_type = {0, 0, 0, 0, 0};
    alert_flag = 0;
}

HeaderRTCM::HeaderRTCM(const HeaderRTCM& header)
{
    time_of_week = header.time_of_week;
    correction_error_bits = header.correction_error_bits;
    PRN = header.PRN;
    message_type = header.message_type;
    alert_flag = header.alert_flag;
}

HeaderRTCM::HeaderRTCM(BitContainer message) : bits{message}
{
    time_of_week = message.getNum(40, 20);
    correction_error_bits = message.getNum(60, 4);
    PRN = message.getNum(64, 8);
    message_type = {message.get(79), message.get(78), message.get(77), static_cast<unsigned short>(message.getNum(75, 2)),
        static_cast<unsigned short>(message.getNum(72, 3))};
    alert_flag = message.get(80);
}

std::string HeaderRTCM::getInfo()
{
    std::string result;
    result += "===================================RTCM header===================================\n";
    result += "Time of week:\t\t\t\t" + std::to_string(time_of_week) + "\n";
    result += "Number of correction error bits:\t" + std::to_string(correction_error_bits) + "\n";
    result += "PRN:\t\t\t\t\t" + std::to_string(PRN) + "\n";
    result += "Message type:\n";
    result += message_type.getInfo();
    result += "Alert flag:\t\t\t\t" + std::to_string(alert_flag) + "\n";
    result += "=================================RTCM header END=================================\n";
    return result;
}