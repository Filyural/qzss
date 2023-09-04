#include "../pch.h"
#include "../Tests/BitContainerTest.h"
#include "BitContainer.h"

/*
 * ����������� �� ���������
 * ------------------------
 * Default constructor
 */
BitContainer::BitContainer() : size_{0}
{
    bits_.resize(0);
}

/*
 * ����������� �����������
 * -----------------------
 * Copy constructor
 */
BitContainer::BitContainer(const BitContainer& container)
{
    bits_ = container.bits_;
    size_ = container.size_;
}

/*
 * ����������� ��� ������������� ���������� �������� <num_bits> ���
 * ----------------------------------------------------------------
 * Constructor to initialize a container of size <num_bits> bits
 */
BitContainer::BitContainer(const size_t num_bits) : size_{num_bits}
{
    bits_.resize(NumLongsNeeded(num_bits));
}

/*
 * ����������� ��� �������� ���������� �� ������ (����� ������������ ��� ������)
 * -----------------------------------------------------------------------------
 * Constructor for creating a container from a string (better used for tests)
 */
BitContainer::BitContainer(const std::string& str)
{

    fromString(str);
}

/*
 * ���������� unsigned long'�� ����������� ��� <num_bits> ���
 * -----------------------------------------------------
 * Number of unsigned longs needed for <num_bits> bits
 */
std::size_t BitContainer::NumLongsNeeded(size_t num_bits)
{
    return (num_bits + kBitsPerUnsignedLong - 1) / kBitsPerUnsignedLong;
}

/*
 * ������ �������
 * --------------
 * Size getter
 */
std::size_t BitContainer::size() const
{
    return size_;
}

/*
 * ���������� � ��� <index> �������� <value>
 * -----------------------------------------
 * Set bit <index> to <value>
 */
void BitContainer::set(std::size_t index, bool value)
{
    if (index >= size_)
    {
        throw std::out_of_range("BitContainer::set() out of range");
    }
    std::size_t long_index = index / kBitsPerUnsignedLong;
    std::size_t bit_index = index % kBitsPerUnsignedLong;
    unsigned long mask = 1LL << (kBitsPerUnsignedLong - bit_index - 1);
    if (value)
    {
        bits_[long_index] |= mask;
    }
    else
    {
        bits_[long_index] &= ~mask;
    }
}

/*
 * ������� �������� ���� <index>
 * -----------------------------
 * Return value of bit <index>
 */
bool BitContainer::get(std::size_t index) const
{
    if (index >= size_)
    {
        throw std::out_of_range("BitContainer::get() out of range");
    }
    std::size_t long_index = index / kBitsPerUnsignedLong;
    std::size_t bit_index = index % kBitsPerUnsignedLong;
    unsigned long mask = 1LL << (kBitsPerUnsignedLong - bit_index - 1);
    return (bits_[long_index] & mask) != 0;
}

/*
 * �������� ���������
 * ------------------
 * Clear container
 */
void BitContainer::clear()
{
    bits_.clear();
    size_ = 0;
}

/*
 * �������� � ��������� <num_bits> ������� ���
 * -------------------------------------------
 * Add zero bits to container <num_bits>
 */
void BitContainer::add(std::size_t num_bits)
{
    // std::size_t old_size = size_;
    size_ += num_bits;
    bits_.resize(NumLongsNeeded(size_));
}

/*
 * �������� � ���������� <sequence> ������������������ ������ (� �����)
 * --------------------------------------------------------------------
 * Add a sequence to the right (end) of the <sequence> container
 */
void BitContainer::add(BitContainer sequence)
{
    // ����� ���� ������� add(size_t), �� ����� �� ��������� ����� ������� ������� ������, � ����� ������� ����������
    std::size_t num_bits = sequence.size();
    std::size_t old_size = size_;
    add(num_bits);
    size_t long_index = NumLongsNeeded(old_size) - 1;
    size_t shift = old_size % kBitsPerUnsignedLong;
    size_t longs_size = NumLongsNeeded(size_);
    if (!shift)
    {
        for (size_t i = long_index + 1, j = 0; i < longs_size; ++i, ++j)
        {
            bits_[i] = sequence.bits_[j];
        }
        return;
    }

    bits_[long_index] |= sequence.bits_[0] >> shift;
    for (size_t i = long_index + 1, j = 0; i < longs_size - 1; ++i, ++j)
    {
        bits_[i] = (sequence.bits_[j] << (kBitsPerUnsignedLong - shift)) | (sequence.bits_[j + 1] >> shift);
    }

    // �������, ��� �� ��������, ���� �������� ��� ����������
    if ((static_cast<int>(longs_size) - static_cast<int>(long_index) - 2) >= 0)
    {
        bits_[longs_size - 1] = sequence.bits_[longs_size - long_index - 2] << (kBitsPerUnsignedLong - shift);
        if (static_cast<int>(longs_size) - static_cast<int>(long_index) - 1 < NumLongsNeeded(num_bits))
        {
            bits_[longs_size - 1] |= sequence.bits_[longs_size - long_index - 1] >> shift;
        }
    }

    // ����� �� ��������� ����� ������ �����
    size_t double_shift = (kBitsPerUnsignedLong - (size_ % kBitsPerUnsignedLong)) % kBitsPerUnsignedLong;
    unsigned long mask = (/*0xffffffffUL*/ ULONG_MAX >> double_shift) << double_shift;
    bits_[longs_size - 1] &= mask;

    // for (size_t i = old_size; i < size_; i++)
    //{
    //     set(i, sequence.get(i - old_size));
    // }
}

/*
 * ��������� �������� ������������������ �� ������ �� ������ (����� ������������ ��� ������)
 * -----------------------------------------------------------------------------------------
 * Completely replaced by a sequence of data from a string (preferable for tests)
 */
void BitContainer::fromString(const std::string& str)
{
    size_ = str.size();
    bits_.resize(NumLongsNeeded(size()));
    for (std::size_t i = 0; i < size(); ++i)
    {
        set(i, str[i] == '1');
    }
}

/*
 * ���������� ��������� �� ������ ������� ������ ������� � ������� <start_index> ������ � <length> ���
 * ---------------------------------------------------------------------------------------------------
 * Returns a substring from the given bit string starting at index <start_index> with a length of <length> bits
 */
BitContainer BitContainer::subContainer(size_t start_index, size_t length)
{
    if (start_index >= size() || length + start_index > size() || length <= 0)
    {
        throw std::out_of_range("BitContainer::get() out of range");
    }

    BitContainer result(length);

    size_t shift = start_index % kBitsPerUnsignedLong;
    size_t long_start_index = NumLongsNeeded(start_index + 1) - 1;
    size_t longs_result_size = NumLongsNeeded(length);
    if (!shift)
    {
        for (size_t i = 0, j = long_start_index; i < longs_result_size; ++i, ++j)
        {
            result.bits_[i] = bits_[j];
        }
    }
    else
    {
        for (size_t i = 0, j = long_start_index; i < longs_result_size - 1; ++i, ++j)
        {
            result.bits_[i] = (bits_[j] << shift) | (bits_[j + 1] >> (kBitsPerUnsignedLong - shift));
        }
        result.bits_[longs_result_size - 1] = bits_[longs_result_size + long_start_index - 1] << shift;
        /*
         * � ������, ���� shift == kBitsPerUnsignedLong, �� ����� ���� ��������,
         * ��� � ��������� UL ��������� ������ ����� ������, ������� ����� ����� �� ��������� �� ������
         * ��������� ����� �� ���������� UL (���� ������ �� ������� �� ������� - ����� ����� �������� �� ����������)
         */
        if (longs_result_size + long_start_index < NumLongsNeeded(size_))
        {
            result.bits_[longs_result_size - 1] |= bits_[longs_result_size + long_start_index] >> (kBitsPerUnsignedLong - shift);
        }
    }

    size_t double_shift = (kBitsPerUnsignedLong - (length % kBitsPerUnsignedLong)) % kBitsPerUnsignedLong;

    unsigned long mask = (/*0xffffffffUL*/ ULONG_MAX >> double_shift) << double_shift;
    result.bits_[longs_result_size - 1] &= mask;

    // for (size_t i = 0; i < length; ++i)
    //{
    //     result.set(i, this->get(start_index + i));
    // }
    return result;
}
/*
 * ���� <length> ����� <size_> - ������ ��������� ������ ��������
 * ���� <length> ������ <size_> - ������ ��������� ������� <length>, ����������� ��������������� <length - size_> ������ � ������
 * ������������������
 * ���� <length> ������ <size_> - ������ ��������� �� ������ ������� ������ ������� � ������� 0, �������� <length>
 * -------------------------------------------------------------------------------------------------------------
 * If <length> is equal to <size_> - will return a container equal to the current one
 * If <length> is greater than <size_> - will return a container of size <length> filled with extra <length - size_> zeros at the START of
 * the sequence If <length> is less than <size_> - will return a substring from the given bit string starting from index 0, of size <length>
 */
BitContainer BitContainer::toLength(size_t length)
{
    if (length == size_)
    {
        return *this;
    }
    else if (length > size_)
    {
        BitContainer result(length - size_);
        result.add(*this);
        return result;
    }
    else
    {
        return subContainer(0, length);
    }
}

/*
 * �������� ����� ���������� � ���� unsigned long ������� � <start_index> ������ <length>
 * --------------------------------------------------------------------------------------
 * Get part of container as unsigned long starting at <start_index> of length <length>
 */
unsigned long BitContainer::getNum(size_t start_index, size_t length)
{
    if (start_index + length > size() || length > kBitsPerUnsignedLong || length <= 0 || start_index < 0)
    {
        throw std::out_of_range("BitContainer::getNum() out of range");
    }

    // unsigned long result = 0;
    // size_t prefix_length = NumLongsNeeded(start_index) * kBitsPerUnsignedLong - start_index;
    // size_t postfix_length = kBitsPerUnsignedLong - prefix_length;

    // for (size_t i = start_index; i < start_index + length; i++)
    //{
    //     result += get(i) * pow(2, (start_index + length - i - 1));
    // }

    size_t long_index = NumLongsNeeded(start_index + 1) - 1;
    size_t shift = start_index % kBitsPerUnsignedLong;
    unsigned long result;
    if (shift + length > kBitsPerUnsignedLong)
    {
        result = (bits_[long_index] << shift) | (bits_[long_index + 1] >> (kBitsPerUnsignedLong - shift));
    }
    else
    {
        result = bits_[long_index] << shift;
    }
    result >>= kBitsPerUnsignedLong - length;
    return result;
}

void BitContainer::trimLeadingZeros()
{
    if (size_ <= 0 || get(0))
    {
        return;
    }

    size_t i = 0;
    while (!bits_[i])
    {
        ++i;
        if (i == bits_.size())
        {
            bits_.resize(0);
            size_ = 0;
            return;
        }
    }
    i *= kBitsPerUnsignedLong;
    bool bit = get(i);
    while (!bit)
    {
        ++i;
        if (i == size())
        {
            bits_.resize(NumLongsNeeded(0));
            size_ = 0;
            return;
        }

        bit = get(i);
    }

    *this = subContainer(i, size() - i);
}

/*
 * ���������� ��������� � ���� ������ ������� <num_bits> � ��������� �� ������ ������ 100 ��������
 * -----------------------------------------------------------------------------------------------
 * Returns the container as a string of size <num_bits> with a line break every 100 characters
 */
std::string BitContainer::getInfo(size_t num_bits) const
{
    std::string result;
    if (num_bits > size())
    {
        throw std::out_of_range("BitContainer::show() out of range");
    }
    for (size_t i = 0; i < num_bits; i++)
    {
        if (i % 100 == 0)
        {
            result += "\n";
        }
        result += std::to_string(get(i));
    }
    result += "\n";
    return result;
}

/*
 * ���������� true, ���� <this> ����� <sequence> � ������ ������������� ���� ����� � ������ �� ������ ���������� �������
 * -----------------------------------------------------------------------------------------------------------------------------
 * Returns true if <this> is equal to <sequence>, taking into account ignoring all leading zeros up to the first encountered one
 */
bool BitContainer::equals(BitContainer sequence)
{
    size_t i = 0, j = 0;
    while (!bits_[i])
    {
        ++i;
    }
    i *= kBitsPerUnsignedLong;

    while (!sequence.bits_[j])
    {
        ++j;
    }
    j *= kBitsPerUnsignedLong;

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

/*
 * ����������� ����� ����� �� ������ � ������
 * ------------------------------------------
 * Converts a signed integer to a string
 */
std::string BitContainer::toString() const
{
    std::string result{};
    for (size_t i = 0; i < size(); i++)
    {
        result += get(i) ? "1" : "0";
    }
    return result;
}

/*
 * �������� ������������
 * ---------------------
 * Assignment operator
 */
BitContainer& BitContainer::operator=(const BitContainer& container)
{
    this->bits_.resize(NumLongsNeeded(container.size()));
    size_ = container.size();
    for (size_t i = 0; i < bits_.size(); ++i)
    {
        bits_[i] = container.bits_[i];
    }
    return *this;
}

/*
* ��������� XOR � ���� ����������� ����������� ������� (���� ������� ������, �� ����������� ���������� <invalid_argument>
* ------------------------------------------------------------------------------------------------------------------------------------
* Applies XOR to the volume of containers of the same size (if the sizes are different, then throws the constraints <invalid_argument>
*/
BitContainer BitContainer::operator^(const BitContainer& container) const
{
    //bool this_smaller = this->size() < container.size();
    //size_t smallest_size = this_smaller ? this->size() : container.size();
    //size_t biggest_size = this_smaller ? container.size() : this->size();
    //BitContainer result(biggest_size);

    //// ��������� ������ ����, ������� ��� � ������� ����������
    //// ������ �� �������� ����������
    //if (this_smaller)
    //{
    //    for (size_t i = 0; i < biggest_size - smallest_size; i++)
    //    {
    //        result.set(i, container.get(i));
    //    }
    //}
    //else
    //{
    //    for (size_t i = 0; i < biggest_size - smallest_size; i++)
    //    {
    //        result.set(i, this->get(i));
    //    }
    //}

    //// ��������� ��������� ����
    //for (size_t i = 0; i < smallest_size; ++i)
    //{
    //    result.set(biggest_size - i - 1, this->get(this->size() - i - 1) ^ container.get(container.size() - i - 1));
    //}    if (size_ != container.size_)
    if (size_ != container.size_)
    {
        throw std::invalid_argument("BitContainers must have the same size for bitwise XOR operation");
    }

    BitContainer result(size_);

    for (std::size_t i = 0; i < bits_.size(); ++i)
    {
        result.bits_[i] = bits_[i] ^ container.bits_[i];
    }

    return result;
}

/*
 * ���������� ��� ���������� �� ������ ��������� (������� ����� � �� ����������� ��������� ����)
 * -------------------------------------------------------------------------------------------------------
 * Tests compares two containers for complete equality (including length and does not discard leading zeros)
 */
bool BitContainer::operator==(const BitContainer& container) const
{
    if (size() != container.size())
    {
        return false;
    }
    if (size() == 0)
    {
        return true;
    }
    for (size_t i = 0; i < bits_.size() - 1; i++)
    {
        if (bits_[i] != container.bits_[i])
        {
            return false;
        }
    }
    // ��������� unsigned long ������������ �� �����
    for (size_t i = (bits_.size() - 1) * kBitsPerUnsignedLong; i < size(); ++i)
    {
        if (get(i) != container.get(i))
        {
            return false;
        }
    }
    return true;
}

bool BitContainer::testPass()
{
    return BitContainerTest::test_ALL();
}
