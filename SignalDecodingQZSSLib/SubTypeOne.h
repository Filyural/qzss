#pragma once

using namespace Bits;

class BitContainer;

struct GNSS_Mask
{

};

class SubTypeOne
{
private:
    Bits::BitContainer message;
    static constexpr size_t indexes[7] = {0, 12, 16, 36, 40, 41, 45};


public:
    SubTypeOne(Bits::BitContainer message);
};
