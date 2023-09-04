#include "../pch.h"
#include "../BitContainer/BitContainer.h"
#include "../Headers/HeaderRTCM.h"

HeaderRTCM::HeaderRTCM(BitContainer message) : bits{message}
{
    time_of_week = message.subContainer(40, 20).getNum(0, 20);
    correction_error_bits = message.subContainer(60, 4).getNum(0, 4);
    PRN = message.subContainer(64, 8).getNum(0, 8);
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