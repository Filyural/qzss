#pragma once
#include "RTCM.h"
#include "HeaderRTCM.h"

class RTCM4050 : public RTCM
{
private:
    HeaderRTCM hrt;

public:
    RTCM4050() = default;
    RTCM4050(const RTCM4050& rtcm);
    RTCM4050(BitContainer& const btc);
};
