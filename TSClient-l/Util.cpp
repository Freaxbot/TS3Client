#include "Util.h"
#include <random>
#include <iostream>
#include <ctime>
#include <regex>


int randInInterval(int min, int max) {
	int intervalLen = max - min + 1;
	int ceilingPowerOf2 = pow(2, ceil(log2(intervalLen)));
	int randomNumber = rand() % ceilingPowerOf2;
	if (randomNumber < intervalLen)
		return min + randomNumber;
	return randInInterval(min, max); 
}

int getUnixTime()
{
	std::time_t t = std::time(0);
	return t;
}

void insert_int(std::vector<unsigned char>* container, int integer)
{
	char* chars = reinterpret_cast<char*>(&integer);
	container->insert(container->end(), chars, chars + sizeof(int));
}

std::vector<unsigned char> string_to_char(std::string a)
{
	std::vector<unsigned char> v(a.begin(), a.end());
	return v;
}


char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',   'B','C','D','E','F' };

std::string byte_2_str_c(char* bytes, int size) {
	short iz = 0;
	std::string str;
	for (int i = 0; i < size; ++i) {
		if (iz >= 1)
		{
			str = str + "-";
			iz = 0;
		}
		const char ch = bytes[i];
		str.append(&hex[(ch & 0xF0) >> 4], 1);
		str.append(&hex[ch & 0xF], 1);
		iz++;

	}
	return str;
}

std::string byte_2_str(char* bytes, int size) {
	short iz = 0;
	std::string str;
	for (int i = 0; i < size; ++i) {

		const char ch = bytes[i];
		str.append(&hex[(ch & 0xF0) >> 4], 1);
		str.append(&hex[ch & 0xF], 1);
		iz++;

	}
	return str;
}

std::string getByte(char* bytes, int index, int leng)
{
	std::string a = byte_2_str(reinterpret_cast<char*>(bytes), leng);
	std::vector<unsigned char> v(a.begin(), a.end());
	std::vector<unsigned char> ta = { v[index * 2 - 1], v[index * 2 -2] };
		std::string s(ta.begin(), ta.end());
		return s;
}
