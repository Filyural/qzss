#pragma once
#include <vector>
#include <string>
#include <fstream>

namespace Bits
{
	class BitContainer
	{
	public:
		BitContainer(int size);
		bool get(int index) const;
		void set(int index, bool value);
		void toggle(int index);
		void clear();
		int size() const;

	private:
		std::vector<bool> m_data;
	};

    //KMP
    static std::vector<int> prefixFunction(const BitContainer& pattern) {
        const int n = pattern.size();
        std::vector<int> failure_table(n + 1);
        failure_table[0] = -1;
        int j = -1;
        for (int i = 0; i < n; ++i) {
            while (j >= 0 && pattern.get(j) != pattern.get(i)) {
                j = failure_table[j];
            }
            ++j;
            failure_table[i + 1] = j;
        }
        return failure_table;
    }

    static std::vector<int> KMP(const BitContainer& bitstring, const BitContainer& substring) {
        const int n = bitstring.size();
        const int m = substring.size();
        std::vector<int> failure_table = prefixFunction(substring);
        std::vector<int> matches;
        int i = 0;
        int j = 0;
        while (i < n) {
            if (bitstring.get(i) == substring.get(j)) {
                ++i;
                ++j;
                if (j == m) {
                    matches.push_back(i - m);
                    j = failure_table[j];
                }
            }
            else {
                j = failure_table[j];
                if (j < 0) {
                    ++i;
                    ++j;
                }
            }
        }
        return matches;
    }

    static BitContainer ReadFileToBC(std::string path)
    {
        std::ifstream bit_file;
        bit_file.open("C:\\files\\lex_2023-03-06.log", std::ifstream::in | std::ifstream::binary);
        if (!bit_file.is_open()) {
            throw std::invalid_argument("Invalid file path or file does not exist");
        }

        std::vector<bool> result();
        unsigned char buffer;
        while (bit_file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {   //Read a byte from a file and check if end of file is received
            //Read bits from file
            for (int i = 0; i < 8; i++) {
                bool bit = buffer & (1 << i);
            }
        }

        bit_file.close();
    }
}