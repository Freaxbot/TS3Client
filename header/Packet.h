#include <iostream>
#include <string>
#include <vector>

std::vector<unsigned char> BuildHeader(char packettype);
void GetPacket(char* data, int lengh); 
char* BuildHeaderNew (char packettype);