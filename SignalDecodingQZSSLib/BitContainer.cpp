#include "pch.h"
#include "BitContainer.h"

/*
*  онструктор по умолчанию
* ------------------------
* Default constructor
*/
BitContainer::BitContainer() : size_{0}
{
    bits_.resize(0);
}

/*
*  онструктор копировани€
* -----------------------
* Copy constructor
*/
BitContainer::BitContainer(const BitContainer& container)
{
    bits_ = container.bits_;
    size_ = container.size_;
}

/*
*  онструктор дл€ инициализации контейнера размером <num_bits> бит
* ----------------------------------------------------------------
* Constructor to initialize a container of size <num_bits> bits
*/
BitContainer::BitContainer(const size_t num_bits) : size_{num_bits}
{
    bits_.resize(NumLongsNeeded(num_bits));
}

/*
*  онструктор дл€ создани€ контейнера из строки (лучше использовать дл€ тестов)
* -----------------------------------------------------------------------------
* Constructor for creating a container from a string (better used for tests)
*/
BitContainer::BitContainer(const std::string& str)
{
    fromString(str);
}

/*
*  оличество longlong'ов необходимое дл€ <num_bits> бит
* -----------------------------------------------------
* Number of longlongs needed for <num_bits> bits
*/
std::size_t BitContainer::NumLongsNeeded(size_t num_bits)
{
    return (num_bits + kBitsPerLongLong - 1) / kBitsPerLongLong;
}

/*
* √еттер размера 
* --------------
* Size getter
*/
std::size_t BitContainer::size() const
{
    return size_;
}

/*
* ”становить в бит <index> значение <value>
* -----------------------------------------
* Set bit <index> to <value>
*/
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

/*
* ¬ернуть значение бита <index>
* -----------------------------
* Return value of bit <index>
*/
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

/*
* ќчистить контейнер
* ------------------
* Clear container
*/
void BitContainer::clear()
{
    bits_.clear();
    size_ = 0;
}

/*
* ƒобавить в контейнер <num_bits> нулевых бит
* -------------------------------------------
* Add zero bits to container <num_bits>
*/
void BitContainer::add(std::size_t num_bits)
{
    std::size_t old_size = size_;
    size_ += num_bits;
    bits_.resize(NumLongsNeeded(size_));
}

void BitContainer::add(BitContainer sequence)
{
    // можно было вызвать add(size_t), но лучше не заполн€ть новые позиции сначала нул€ми, а потом нужными значени€ми
    std::size_t num_bits = sequence.size();
    std::size_t old_size = size_;
    size_ += num_bits;
    bits_.resize(NumLongsNeeded(size_));
    for (size_t i = old_size; i < size_; i++)
    {
        set(i, sequence.get(i - old_size));
    }
}

/*
* ѕолностью замен€ет последовательность на данные из строки (лучше использовать дл€ тестов)
* -----------------------------------------------------------------------------------------
* Completely replaced by a sequence of data from a string (preferable for tests)
*/
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

long long BitContainer::getNum(size_t start_index, size_t length)
{
    if (start_index + length > size() || length > kBitsPerLongLong || length <= 0 || start_index < 0)
    {
        throw std::out_of_range("BitContainer::getNum() out of range");
    }
    long long result = 0;
    size_t prefix_length = NumLongsNeeded(start_index) * kBitsPerLongLong - start_index;
    size_t postfix_length = kBitsPerLongLong - prefix_length;

    // todo переделать чтобы получать число через сдвиг
    //TODO

    for (size_t i = start_index; i < start_index + length; i++)
    {
        result += get(i) * pow(2, (start_index + length - i - 1));
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
        // возможно проверку на нулевую строку стоит вынести в отдельную функцию
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


/*
* ¬озвращает контейнер в виде строки размера <num_bits> с переходом на строку каждые 100 символов
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

/*
* ѕреобразует целое число со знаком в строку
* ------------------------------------------
* Converts a signed integer to a string
*/
std::string BitContainer::toString()
{
    std::string result{};
    for (size_t i = 0; i < size(); i++)
    {
        result += get(i) ? "1" : "0";
    }
    return result;
}

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

BitContainer BitContainer::operator^(const BitContainer& container) const
{
    bool this_smaller = this->size() < container.size();
    size_t smallest_size = this_smaller ? this->size() : container.size();
    size_t biggest_size = this_smaller ? container.size() : this->size();
    BitContainer result(biggest_size);

    // заполн€ем первые биты, которых нет в меньшем контейнере
    // битами из бќльшего контейнера
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

    // заполн€ем остальные биты
    for (size_t i = 0; i < smallest_size; ++i)
    {
        result.set(biggest_size - i - 1, this->get(this->size() - i - 1) ^ container.get(container.size() - i - 1));
    }

    return result;
}

/*
* ѕровер€ет сравнивает два контейнера на полное равенство (включа€ длину и не отбрасывает начальные нули)
* -------------------------------------------------------------------------------------------------------
* Tests compares two containers for complete equality (including length and does not discard leading zeros)
*/
bool BitContainer::operator==(const BitContainer& container) const
{
    if (size() != container.size())
    {
        return false;
    }
    for (size_t i = 0; i < bits_.size() - 1; i++)
    {
        if (bits_[i] != container.bits_[i])
        {
            return false;
        }
    }
    //последний long long сравниваетс€ по битам
    for (size_t i = (bits_.size() - 1) * kBitsPerLongLong; i < size(); ++i)
    {
        if (get(i) != container.get(i))
        {
            return false;
        }
    }
    return true;
}