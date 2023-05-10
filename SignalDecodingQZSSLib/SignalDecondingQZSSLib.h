#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

class BitContainer;

// KMP
std::vector<std::size_t> prefixFunction(const BitContainer& pattern);
std::vector<std::size_t> KMP(const BitContainer& text, const BitContainer& pattern);
bool KMP(const BitContainer& text, const BitContainer& pattern, size_t& index);

// read file
BitContainer readFileToBC(std::string path, size_t& message);
size_t getFileSize(std::string path);

// CRC
BitContainer calculateCRC(BitContainer message, BitContainer polynomial);
bool checkCRC(BitContainer sequence, BitContainer polynomial, size_t crc_index);

// QZSS
bool getHeaderInfo(BitContainer message);