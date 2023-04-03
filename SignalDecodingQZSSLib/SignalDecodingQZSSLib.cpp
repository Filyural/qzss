// SignalDecodingQZSSLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"

BitContainer::BitContainer(size_t initialSize) {
    size = initialSize;
    bits.resize((size + sizeof(int) - 1) / sizeof(int));
}

bool BitContainer::get(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    int word = bits[index / (sizeof(int) * 8)];
    int mask = 1 << (index % (sizeof(int) * 8));
    return (word & mask) != 0;
}

void BitContainer::set(size_t index, bool value) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    int& word = bits[index / (sizeof(int) * 8)];
    int mask = 1 << (index % (sizeof(int) * 8));
    if (value) {
        word |= mask;
    }
    else {
        word &= ~mask;
    }
}

void BitContainer::add(bool value) {
    if (size >= bits.size() * sizeof(int) * 8) {
        size_t newSize = size * 2;
        bits.resize((newSize + sizeof(int) - 1) / sizeof(int));
    }
    set(size, value);
    size++;
}

size_t BitContainer::getMemoryUsage() const {
    return sizeof(bits);
}
