// SignalDecodingQZSSLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"

namespace Bits
{

BitContainer::BitContainer(size_t num_bits) : size_{num_bits}
{
    bits_.resize(NumLongsNeeded(num_bits));
}

BitContainer::BitContainer(const std::string& str)
{
    fromString(str);
}

std::size_t BitContainer::NumLongsNeeded(size_t num_bits)
{
    return (num_bits + kBitsPerLongLong - 1) / kBitsPerLongLong;
}

std::size_t BitContainer::size() const
{
    return size_;
}

void BitContainer::set(std::size_t index, bool value)
{
    if (index >= size_)
    {
        throw std::out_of_range("BitContainer::set() out of range");
    }
    std::size_t long_index = index / kBitsPerLongLong;
    std::size_t bit_index = index % kBitsPerLongLong;
    long long mask = 1LL << (kBitsPerLongLong - bit_index - 1);
    if (value)
    {
        bits_[long_index] |= mask;
    }
    else
    {
        bits_[long_index] &= ~mask;
    }
}

bool BitContainer::get(std::size_t index) const
{
    if (index >= size_)
    {
        throw std::out_of_range("BitContainer::get() out of range");
    }
    std::size_t long_index = index / kBitsPerLongLong;
    std::size_t bit_index = index % kBitsPerLongLong;
    long long mask = 1LL << (kBitsPerLongLong - bit_index - 1);
    return (bits_[long_index] & mask) != 0;
}

void BitContainer::clear()
{
    bits_.clear();
    size_ = 0;
}

void BitContainer::add(std::size_t num_bits)
{
    std::size_t old_size = size_;
    size_ += num_bits;
    bits_.resize(NumLongsNeeded(size_));
    for (std::size_t i = old_size; i < size_; ++i)
    {
        set(i, false);
    }
}

void BitContainer::fromString(const std::string& str)
{
    size_ = str.size();
    bits_.resize(NumLongsNeeded(str.size()));
    for (std::size_t i = 0; i < size(); ++i)
    {
        set(i, str[i] == '1');
    }
}
} // namespace Bits