#include "pch.h"
#include "SubTypeOne.h"
#include "SignalDecondingQZSSLib.h"

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

        //BitContainer gnss_bits = message_.subContainer(index, )
        //GNSS_Mask gnss = ;
        //GNSSes.push_back();
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
