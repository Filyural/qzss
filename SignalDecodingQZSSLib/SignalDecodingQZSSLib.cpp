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

BitContainer BitContainer::toLength(size_t length)
{
    size_t size_difference = length - size();
    BitContainer result(length);
    if (size_difference >= 0)
    {
        for (size_t i = size_difference; i < length; i++)
        {
            result.set(i, get(i - size_difference));
        }
        return result;
    }
    return subContainer(0, length);
}

unsigned int BitContainer::getNum(size_t start_index, size_t length)
{
    if (start_index + length > size())
    {
        throw std::out_of_range("BitContainer::getNum() out of range"); 
    }
    unsigned int result = 0;
    for (size_t i = start_index; i < start_index + length; i++)
    {
        result += get(i) * pow(2, (length - i - 1));
    }
    return result;
}

unsigned int BitContainer::getNum()
{
    return getNum(0, size());
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

std::string BitContainer::toString()
{
    std::string result{};
    for (size_t i = 0; i < size(); i++)
    {
        result += get(i) ? "1" : "0";
    }
    return result;
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

std::vector<std::size_t> prefixFunction(const BitContainer& pattern)
{
    std::size_t m = pattern.size();
    std::vector<std::size_t> pi(m);
    std::size_t k = 0;
    for (std::size_t q = 1; q < m; ++q)
    {
        while (k > 0 && pattern.get(k) != pattern.get(q))
        {
            k = pi[k - 1];
        }
        if (pattern.get(k) == pattern.get(q))
        {
            ++k;
        }
        pi[q] = k;
    }
    return pi;
}

std::vector<std::size_t> KMP(const BitContainer& text, const BitContainer& pattern)
{
    std::vector<std::size_t> matches;
    std::size_t n = text.size();
    std::size_t m = pattern.size();
    auto pi = prefixFunction(pattern);
    std::size_t q = 0;
    for (std::size_t i = 0; i < n; ++i)
    {
        while (q > 0 && pattern.get(q) != text.get(i))
        {
            q = pi[q - 1];
        }
        if (pattern.get(q) == text.get(i))
        {
            ++q;
        }
        if (q == m)
        {
            matches.push_back(i - m + 1);
            q = pi[q - 1];
        }
    }
    return matches;
}

BitContainer readFileToBC(std::string path)
{
    std::ifstream bit_file;
    size_t file_size = getFileSize(path);

    bit_file.open(path, std::ifstream::in | std::ifstream::binary);
    if (!bit_file.is_open())
    {
        throw std::invalid_argument("Invalid file path or file does not exist");
    }

    std::cout << "File size (in bits):\t" << file_size << std::endl;

    BitContainer result(file_size);
    unsigned char buffer;
    int j = 0;
    while (bit_file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer)))
    { // Read a byte from a file and check if end of file is received
        // Read bits from file
        for (int i = 0; i < 8; i++)
        {
            bool value = buffer & (1 << 7 - i);
            result.set(j * 8 + i, buffer & (1 << 7 - i));
            // DEBUG
            // std::cout << "j * 8 + i = " << j * 8 + i << " \tvalue = " << value << std::endl;
        }
        ++j;
    }
    // std::cout << "SIZE ===== " << j * 8;

    bit_file.close();

    return result;
}

size_t getFileSize(std::string path)
{
    std::ifstream bit_file;
    bit_file.open(path, std::ifstream::in | std::ifstream::binary | std::ios::ate);
    if (!bit_file.is_open())
    {
        throw std::invalid_argument("Invalid file path or file does not exist");
    }
    std::streampos file_size = bit_file.tellg();
    bit_file.close();
    return file_size * 8;
}

BitContainer calculateCRC(BitContainer message, BitContainer polynomial)
{
    message.add(polynomial.size() - 1);
    size_t new_length = message.size();

    BitContainer sub_message = message.subContainer(0, polynomial.size());

    size_t size_difference;
    for (size_t i = polynomial.size();;)
    {
        sub_message = sub_message ^ polynomial;
        sub_message.trimLeadingZeros();
        size_difference = polynomial.size() - sub_message.size();

        if (i + size_difference > new_length)
        {
            if (i >= new_length)
            {
                return sub_message.toLength(polynomial.size() - 1);
            }
            sub_message.add(message.subContainer(i, message.size() - i));
            return sub_message.toLength(polynomial.size() - 1);
        }
        sub_message.add(message.subContainer(i, size_difference));
        i += size_difference;
    }
}

bool checkCRC(BitContainer sequence, BitContainer polynomial, size_t crc_index)
{
    BitContainer message = sequence.subContainer(0, crc_index);
    BitContainer crc = sequence.subContainer(crc_index, sequence.size() - crc_index);

    BitContainer crc_calc = calculateCRC(message, polynomial);
    return crc == (crc_calc);
}

bool getHeaderInfo(BitContainer message)
{
    //TODO
    return false;
}
} // namespace Bits