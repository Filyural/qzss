#pragma once

class BitContainer;

class RTCM
{
protected:
    BitContainer* btc;
    size_t size;

public:
    RTCM() = default;
    RTCM(const RTCM& rtcm);
    RTCM(BitContainer& const btc);

    size_t getSize()
    {
        return size;
    }

    static BitContainer calculateCRC(BitContainer message, const BitContainer& const polynomial);
    static bool checkCRC(const BitContainer& const sequence, const BitContainer& const polynomial, size_t crc_index);

    virtual ~RTCM();
};
