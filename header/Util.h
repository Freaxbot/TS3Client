 
#include <random>
#include <iostream>
#include <ctime>

int randInInterval(int min, int max);
int getUnixTime();
void insert_int(std::vector<unsigned char>* container, int integer);
std::vector<unsigned char> string_to_char(std::string a);
std::string byte_2_str(char* bytes, int size);
std::string byte_2_str_c(char* bytes, int size);
std::string getByte(char* bytes, int index, int leng);