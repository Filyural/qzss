#pragma once
// Container class
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
    std::string getInfo(size_t num_bits) const;
    std::string getInfo() const;
    bool equals(BitContainer& sequence);
    std::string toString();

    BitContainer operator^(const BitContainer& container) const;
    bool operator==(const BitContainer& container) const;
};
