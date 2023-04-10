#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

namespace Bits
{
class BitContainer;

static std::vector<std::size_t> prefixFunction(const BitContainer& pattern);
static std::vector<std::size_t> KMP(const BitContainer& text, const BitContainer& pattern);

static BitContainer readFileToBC(std::string path);
static size_t getFileSize(std::string path);

class BitContainer
{
private:
    std::vector<long long> bits_;
    std::size_t size_;

    static constexpr int kBitsPerLongLong = sizeof(long long) * 8;
    static size_t NumLongsNeeded(std::size_t num_bits);

public:
    BitContainer(std::size_t num_bits);
    BitContainer(const std::string& str);

    std::size_t size() const;
    void set(std::size_t index, bool value);
    bool get(std::size_t index) const;
    void clear();
    void add(std::size_t num_bits);
    void fromString(const std::string& str);
    BitContainer subContainer(size_t start_index, size_t length);
    void trimLeadingZeros();

    BitContainer operator^(const BitContainer& container) const;
};

// KMP
static std::vector<std::size_t> prefixFunction(const BitContainer& pattern)
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

static std::vector<std::size_t> KMP(const BitContainer& text, const BitContainer& pattern)
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

// read file

static BitContainer readFileToBC(std::string path)
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

static size_t getFileSize(std::string path)
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

static bool CalculateCRC(BitContainer message, BitContainer polynomial)
{
    size_t initial_length = message.size();
    message.add(polynomial.size() - 1);

    BitContainer sub_message = message.subContainer(0, polynomial.size());

    for (size_t i = 0; i < initial_length; ++i)
    {
        sub_message = sub_message ^ polynomial;
    }
    //while (difference != 0)
    //{
    //    difference = message.size() - polynomial.size();
    //    polynomial.add(difference);
    //}
    return false;
}
} // namespace Bits