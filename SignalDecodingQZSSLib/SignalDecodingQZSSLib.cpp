// SignalDecodingQZSSLib.cpp : Определяет функции для статической библиотеки.
//
#include "pch.h"
#include "BitContainer/BitContainer.h"
#include "SignalDecondingQZSSLib.h"


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

bool KMP(const BitContainer& text, const BitContainer& pattern, size_t& index)
{
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
            index = i - m + 1;
            return true;
        }
    }
    return false;
}

BitContainer readFileToBC(std::string path, size_t& file_size)
{
    std::ifstream bit_file;
    file_size = getFileSize(path);

    bit_file.open(path, std::ifstream::in | std::ifstream::binary);
    if (!bit_file.is_open())
    {
        throw std::invalid_argument("Invalid file path or file does not exist");
    }

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