#include "pch.h"

SubTypeOne::SubTypeOne(Bits::BitContainer& message_) : message{message_}
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

Bits::BitContainer SubTypeOne::getMessage()
{
    return message;
}

std::vector<GNSS_Mask> SubTypeOne::getGNSSes()
{
    return GNSSes;
}

void SubTypeOne::showInfo()
{
    std::cout << "=================================== SUBTYPE 1 ===================================" << std::endl;
    std::cout << "GPS epoch time:\t" << GPS_epoch_time << std::endl;
    std::cout << "SSR update interval:\t" << ssr_update_interval << std::endl;
    std::cout << "Multiple message indicator:\t" << multiple_message_indicator << std::endl;
    std::cout << "IOD:\t" << IOD << std::endl;
    std::cout << "Number of GNSS:\t" << number_of_GNSS << std::endl;
    
    // TODO
    std::cout << "GNSSes TBD" << std::endl;

    std::cout << "================================= SUBTYPE 1 END =================================" << std::endl << std::endl;
}