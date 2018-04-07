#include "Packet.h"
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include "PacketHandler.h"
#include <iostream>
#include <string>
#include <vector>
#include "Util.h"
#include "Crypto.h"
#include "Packethandler.h"


class Packet {
public:
	int PackeiId;
	int ClientId;
	int PacketType;
	std::string Data;
	std::string Header;
};

std::vector<unsigned char> BuildHeader(char packettype)
{
	std::vector<unsigned char> h = string_to_char("TS3INIT1");
	
	h.push_back(0x00);
	h.push_back(0x65);
	h.push_back(0x00);
	h.push_back(0x00);
	h.push_back(packettype);
	return h;
}

void GetPacket(char* data, int lengh)
{
	std::string  type = getByte(data, 11, lengh);

	if (type == "88") {
		std::cout << "Package INIT1" << std::endl;
		InitProcess(data);
	}

}
