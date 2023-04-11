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

void BitContainer::add(BitContainer sequence)
{
    //можно было вызвать add(size_t), но лучше не заполнять новые позиции сначала нулями, а потом нужными значениями
    std::size_t num_bits = sequence.size();
    std::size_t old_size = size_;
    size_ += num_bits;
    bits_.resize(NumLongsNeeded(size_));
    for (size_t i = old_size; i < size_; i++)
    {
        set(i, sequence.get(i - old_size));
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
BitContainer BitContainer::subContainer(size_t start_index, size_t length)
{
    if (start_index >= size() || length + start_index > size())
    {
        throw std::out_of_range("BitContainer::get() out of range");
    }

    BitContainer result(length);

    for (size_t i = 0; i < length; ++i)
    {
        result.set(i, this->get(start_index + i));
    }

    return result;
}

void BitContainer::trimLeadingZeros()
{
    if (get(0))
    {
        return;
    }

    size_t i = 0;
    bool bit = get(0);
    while (!bit)
    {
        ++i;
        //возможно проверку на нулевую строку стоит вынести в отдельную функцию
        if (i == size())
        {
            bits_.resize(NumLongsNeeded(0));
            size_ = 0;
            return;
        }

        bit = get(i);
    }

    for (size_t j = 0; j < size() - i; ++j)
    {
        set(j, get(j + i));
    }

    bits_.resize(NumLongsNeeded(size() - i));
    size_ = size() - i;
}
void BitContainer::show(size_t num_bits) const
{
    for (size_t i = 0; i < num_bits; i++)
    {
        if (i % 100 == 0)
        {
            std::cout << std::endl;
        }
        std::cout << get(i);
    }
    std::cout << std::endl;
}

bool BitContainer::equals(BitContainer& sequence)
{
    size_t i = 0, j = 0;
    bool bit = get(i);
    while (!bit)
    {
        ++i;
        if (i == size())
        {
            break;
        }
        bit = get(i);
    }
    bit = sequence.get(j);
    while (!bit)
    {
        ++j;
        if (j == sequence.size())
        {
            break;
        }
        bit = sequence.get(j);
    }
    if (i == size() && j == sequence.size())
    {
        return true;
    }
    else if (i == size() || j == sequence.size())
    {
        return false;
    }
    return subContainer(i, this->size() - i) == sequence.subContainer(j, sequence.size() - j);
}

BitContainer BitContainer::operator^(const BitContainer& container) const
{
    bool this_smaller = this->size() < container.size();
    size_t smallest_size = this_smaller ? this->size() : container.size();
    size_t biggest_size = this_smaller ? container.size() : this->size();
    BitContainer result(biggest_size);

    //заполняем первые биты, которых нет в меньшем контейнере
    //битами из бОльшего контейнера
    if (this_smaller)
    {
        for (size_t i = 0; i < biggest_size - smallest_size; i++)
        {
            result.set(i, container.get(i));
        }
    }
    else
    {
        for (size_t i = 0; i < biggest_size - smallest_size; i++)
        {
            result.set(i, this->get(i));
        }
    }

    //заполняем остальные биты
    for (size_t i = 0; i < smallest_size; ++i)
    {
        result.set(biggest_size - i - 1, this->get(this->size() - i - 1) ^ container.get(container.size() - i - 1));
    }

    return result;
}

bool BitContainer::operator==(const BitContainer& container) const
{
    if (size() != container.size())
    {
        return false;
    }
    for (size_t i = 0; i < size(); i++)
    {
        if (get(i) != container.get(i))
        {
            return false;
        }
    }
    return true;
}

} // namespace Bits