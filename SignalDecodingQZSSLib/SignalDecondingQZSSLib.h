#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

namespace Bits
{
class BitContainer;

//KMP
std::vector<std::size_t> prefixFunction(const BitContainer& pattern);
std::vector<std::size_t> KMP(const BitContainer& text, const BitContainer& pattern);

//read file
BitContainer readFileToBC(std::string path);
size_t getFileSize(std::string path);

//CRC
BitContainer CalculateCRC(BitContainer message, BitContainer polynomial);
bool CheckCRC(BitContainer sequence, BitContainer polynomial, size_t crc_index);

//Container class
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
    void add(BitContainer sequence);
    void fromString(const std::string& str);
    BitContainer subContainer(size_t start_index, size_t length);
    BitContainer toLength(size_t length);
    unsigned int getNum(size_t start_index, size_t length);
    unsigned int getNum();
    void trimLeadingZeros();
    void show(size_t num_bits) const;
    bool equals(BitContainer& sequence);
    std::string toString();

    BitContainer operator^(const BitContainer& container) const;
    bool operator==(const BitContainer& container) const;
};
} // namespace Bits