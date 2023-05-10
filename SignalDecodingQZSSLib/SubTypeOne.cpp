#include "pch.h"

SubTypeOne::SubTypeOne(BitContainer& message_) : message{message_}
{
    GPS_epoch_time = message_.subContainer(16, 20).getNum();
    ssr_update_interval = message_.subContainer(36, 4).getNum();
    multiple_message_indicator = message_.get(40);
    IOD = message_.subContainer(41, 4).getNum();
    number_of_GNSS = message_.subContainer(45, 4).getNum();
    size_t index = 49;
    for (size_t i = 0; i < number_of_GNSS; i++)
    {

        // BitContainer gnss_bits = message_.subContainer(index, )
        // GNSS_Mask gnss = ;
        // GNSSes.push_back();
    }
}

BitContainer SubTypeOne::getMessage()
{
    return message;
}

std::vector<GNSS_Mask> SubTypeOne::getGNSSes()
{
    return GNSSes;
}

std::string SubTypeOne::getInfo()
{
    std::string result;
    result += "=================================== SUBTYPE 1 ===================================\n";
    result += "GPS epoch time:\t" + std::to_string(GPS_epoch_time) + "\n";
    result += "SSR update interval:\t" + std::to_string(ssr_update_interval) + "\n";
    result += "Multiple message indicator:\t" + std::to_string(multiple_message_indicator) + "\n";
    result += "IOD:\t" + std::to_string(IOD) + "\n";
    result += "Number of GNSS:\t" + std::to_string(number_of_GNSS) + "\n";
    
    // TODO
    //std::cout << "GNSSes TBD" << std::endl;

    //std::cout << "================================= SUBTYPE 1 END =================================" << std::endl << std::endl;

    return result;
}