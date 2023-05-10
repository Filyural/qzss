#pragma once
#include <vector>

class BitContainer;

struct GNSS_Mask
{
    unsigned char gnss_id;
    BitContainer sattelite_mask;
    BitContainer signal_mask;
    bool mask_availability;
    BitContainer cell_mask;
};

class SubTypeOne
{
private:
    BitContainer& message;
    static constexpr size_t indexes[7] = {0, 12, 16, 36, 40, 41, 45};
    static constexpr size_t sizes[7] = {12, 4, 20, 4, 1, 4, 4};
    unsigned int GPS_epoch_time;
    unsigned short ssr_update_interval;
    bool multiple_message_indicator;
    unsigned short IOD;
    unsigned short number_of_GNSS;
    std::vector<GNSS_Mask> GNSSes;

public:
    SubTypeOne(BitContainer& message_);

    BitContainer getMessage();
    std::vector<GNSS_Mask> getGNSSes();

    std::string getInfo();
};
