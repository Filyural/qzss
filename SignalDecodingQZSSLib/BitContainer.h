#include <vector>

#pragma once
class BitContainer
{
private:
    std::vector<unsigned long> bits_;
    std::size_t size_;

    static constexpr int kBitsPerUnsignedLong = sizeof(unsigned long) * 8;
    static size_t NumLongsNeeded(std::size_t num_bits);

public:
    BitContainer();
    BitContainer(const BitContainer& container);
    BitContainer(const std::size_t num_bits);
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
    unsigned long getNum(size_t start_index, size_t length);
    void trimLeadingZeros();
    std::string getInfo(size_t num_bits) const;
    bool equals(BitContainer& sequence);
    std::string toString() const;

    BitContainer& operator=(const BitContainer& container);
    BitContainer operator^(const BitContainer& container) const;
    bool operator==(const BitContainer& container) const;

};

