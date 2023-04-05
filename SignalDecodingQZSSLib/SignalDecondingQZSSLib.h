#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

namespace Bits
{

class BitContainer
{
private:
    std::vector<long> bits_;
    std::size_t size_;

    static constexpr int kBitsPerLong = sizeof(long) * 8;
    static std::size_t NumLongsNeeded(std::size_t num_bits);

public:
    BitContainer(std::size_t num_bits);
    std::size_t size() const;
    void set(std::size_t index, bool value);
    bool get(std::size_t index) const;
    void clear();
    void add(std::size_t num_bits);
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
    bit_file.open(path, std::ifstream::in | std::ifstream::binary);
    if (!bit_file.is_open())
    {
        throw std::invalid_argument("Invalid file path or file does not exist");
    }

    BitContainer result(100000000);
    unsigned char buffer;
    int j = 0;
    while (bit_file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer)))
    { // Read a byte from a file and check if end of file is received
        // Read bits from file
        for (int i = 0; i < 8; i++)
        {
            result.set(j * 8 + i, buffer & (1 << 7 - i));
        }
        ++j;
    }

    bit_file.close();

    return result;
}
} // namespace Bits