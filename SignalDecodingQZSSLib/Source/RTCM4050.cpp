#include "pch.h"
#include "../Headers/RTCM4050.h"
#include "../Headers/BitContainer.h"

/*
 * Конструктор копирования
 * -----------------------
 * Copy constructor
 */
RTCM4050::RTCM4050(const RTCM4050& rtcm) : RTCM(rtcm)
{
    this->hrt = rtcm.hrt;
}

RTCM4050::RTCM4050(BitContainer& const btc) : RTCM(btc)
{
    if (btc.getNum(24, 12) != 4050)
    {
        throw std::invalid_argument("RTCM4050 must have message number = 4050");
    }
    hrt = HeaderRTCM(btc);
}