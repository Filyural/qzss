#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../SignalDecodingQZSSLib/framework.h"


using std::cout;
using std::endl;
using std::string;
using std::vector;
//using std::stoi;

unsigned char getByte(int pos); //read byte from bit <pos>
string byteToStringOfBits(int n);		//translate byte to string type
int getNumberFromBitString(string str, int n);

vector<int> prefixFunction(string pattern);
int kmp(string text, string pattern);

std::ifstream in;

int main()
{
	BitContainer asd(200000);

	for (size_t i = 0; i < 200000; i++)
	{
		asd.set(i, i + 1);
	}

	std::cout << "Memory usage: " << asd.getMemoryUsage() << " bytes" << std::endl;
	cout << endl;
	//std::size_t size = sizeof(asd.GetContainer());
	//std::cout << "Size of int: " << size << " bytes" << std::endl;
}
int main2()
{
	in.open("C:\\files\\lex_2023-03-06.log", std::ifstream::in | std::ifstream::binary);
	std::string bit_string;
	if (!in.is_open()) {
		cout << "File not found!" << endl;
		return 0;
	}
	cout << "File opened successfully!" << endl << endl;

	in.seekg(0, std::ios::end);
	long long size = in.tellg();    //number of bits (file size)

	cout << "File size: " << size << endl << endl;

	//Биты должны храниться в битах, а не в символах,
	//так что позже переделать из строк в битовые контейнеры и поменять все операции со строками на битовые
	//TODO

	//сохранение бит в строку
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 8; j++) {
			bit_string += byteToStringOfBits((int)getByte(i * 8))[j];
		}
	}

	//отладка
	//for (int i = 0; i < 1000; i++)
	//{
	//	cout << bit_string[i];
	//	if ((i + 1) % 100 == 0)
	//	{
	//		cout << endl;
	//	}
	//}

	cout << endl << endl;

	bool start_founded = false;

	size_t preamble_index = 0;		//индекс преамбулы RTCM
	size_t outer_MN;				//outer Message Number (==4050 ?)
	size_t message_length;
	size_t MN_index;				//Message Number (==4073 ?)
	size_t subtype_index;

	size_t index = 0;				//текущий индекс для поиска преамболы
	int tmp;
	bool enough_condition = false;
	bool subtype_found = false;
	//искать преамбулу, пока не выполнено условие, или не закончится файл
	while (!subtype_found || index < size)
	{
		tmp = kmp(bit_string.substr(index, size - index), "11010011");
		//поиск первого вхождения преамбулы от индекса до конца файла
		preamble_index = tmp + index;
		//пока без проверки QC definition
		index = preamble_index + 12;	//если нашли преамбулу,
		//то следующее сообщение начать искать на 1800 бит позже
		//(чтобы не искать его в этом же пакете)

		//TODO CRC-qc
		if (tmp == -1)
		{
			cout << "No preambols after index [" << preamble_index << "]" << endl;
			break;
		}
		/*cout << preamble_index << endl;
		cout << bit_string.substr(preamble_index + 1776, 24) << endl;*/

		message_length = getNumberFromBitString(bit_string.substr(preamble_index + 14, 10), 10);
		outer_MN = getNumberFromBitString(bit_string.substr(preamble_index + 24, 12), 12);
		bool is_MADOCA = bit_string.substr(preamble_index + 72, 3) == "010";	//redo
		MN_index = preamble_index + 81;	//индекс 4073 должен находиться через 81 бит, после премболы
		subtype_index = MN_index + 1;		//индекс subtype_id должен находиться сразу после message number

		enough_condition = bit_string.substr(preamble_index + 8, 6) == "000000" &&
			message_length == 219 &&
			outer_MN == 4050 && is_MADOCA;

		if (enough_condition) {
			index = preamble_index + 1800;
		}
		else {
			cout << "message_length: " << message_length << endl;
			cout << "outer_MN: " << outer_MN << endl;
			cout << "is_MADOCA: " << is_MADOCA << endl;
			continue;
		}
		cout << preamble_index << endl;
		cout << bit_string.substr(MN_index, 12) << endl;
		//вывод для отладки
		if (bit_string.substr(MN_index, 12) == "111111101001") {
			cout << "Message Number:\t" << bit_string.substr(MN_index, 12) << endl;
			cout << "Subtype ID:\t" << bit_string.substr(subtype_index, 4) << endl;
		}

		//если преамбола и индекс равны нужным значениям, то мы зацепились за начало нужного подкадра
		if (enough_condition &&
			bit_string.substr(MN_index, 12) == "111111101001" &&
			bit_string.substr(subtype_index, 4) == "0001")
		{
			subtype_found = true;
		}
		//else 
		//{
		//	cout << "\nSubtype: " << bit_string.substr(subtype_index, 4);
		//}
	}

	if (tmp == -1) {
		cout << "No correct preabola found" << endl;
		return 0;
	}


	cout << endl;
	cout << "Index of right preambola (where message number == 4073\nand message subtype_id == 1): " << preamble_index << endl;


	string subtype = bit_string.substr(subtype_index, 4);

	cout << endl;
	cout << "subtype: " << subtype;


	//string str_number_of_GNSS = bit_string.substr(preambola_index + 81, 4);
	//size_t number_of_GNSS = getNumberFromBitString(str_number_of_GNSS, 4);

	//vector<string> GNSS_IDs(number_of_GNSS);
	//for (size_t i = 0; i < number_of_GNSS; ++i)
	//{
	//	int tail = 0;
	//	GNSS_IDs[i] = bit_string.substr(preambola_index + 49 + i * 57 + tail, 4);
	//}

	//cout << "Number of GNSS (bits): " << str_number_of_GNSS << endl;
	//cout << "Number of GNSS: " << number_of_GNSS << endl;

	//cout << "GNSS IDs:" << endl;

	//for (size_t i = 0; i < number_of_GNSS; i++)
	//{
	//	cout << "\t" << GNSS_IDs[i] << "\t=\t" << getNumberFromBitString(GNSS_IDs[i], 4) << endl;
	//}


	in.close();
	return 0;
}

string byteToStringOfBits(int n)
{
	string ret = "        ";
	for (int i = 7; i >= 0; i--)
		ret[7 - i] = 48 + (n >> i) % 2;
	return ret;
}

int getNumberFromBitString(string str, int n)
{
	int result = 0;
	for (size_t i = 0; i < n; i++)
	{
		result += ((int)(str[i]) - 48) * pow(2, n - i - 1);
	}
	return result;
}

unsigned char getByte(int pos)
{
	in.seekg(pos / 8);                     // переместиться чтоб считать тот байт, где содержится запрашиваемый бит
	char c;
	in.get(c);                           // читаем
	if (pos % 8 == 0) return c;              // возвращаем сразу считанное, если считываем с начала байта
	char c1;
	in.get(c1);                          // иначе считываем следующий за ним байт
	return (c << (pos % 8)) + (c1 >> (8 - (pos % 8)));
}

vector<int> prefixFunction(string pattern) {
	int m = pattern.length();
	vector<int> prefix(m);
	prefix[0] = 0;
	int j = 0;
	for (int i = 1; i < m; i++) {
		while (j > 0 && pattern[i] != pattern[j]) {
			j = prefix[j - 1];
		}
		if (pattern[i] == pattern[j]) {
			j++;
		}
		prefix[i] = j;
	}
	return prefix;
}

int kmp(string text, string pattern) {
	int n = text.length();
	int m = pattern.length();
	vector<int> prefix = prefixFunction(pattern);
	int i = 0, j = 0;
	while (i < n) {
		if (pattern[j] == text[i]) {
			i++;
			j++;
		}
		if (j == m) {
			return i - j;
		}
		else if (i < n && pattern[j] != text[i]) {
			if (j > 0) {
				j = prefix[j - 1];
			}
			else {
				i++;
			}
		}
	}
	return -1;
}