
#include "Crypto.h"
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include "PacketHandler.h"
#include "Packet.h"
#include <iostream>
#include <string>
#include <vector>
#include "Util.h"
#include <iomanip>





void IncommingPacket(char* data)
{

}


// ========== TS3INIT ==========
void StartingInitProcess()
{
	// [0]
	std::vector<unsigned char> init = BuildHeader(0x88);
	int version[5] = { 0x09, 0x83, 0x8c, 0xCF, 0x00 };
	
	for (int i = 0; i < 5; i++)
		init.push_back(version[i]);
	insert_int(&init, getUnixTime());
	for (int i = 0; i < 4; i++)
		init.push_back(randInInterval(0, 256));
	for (int i = 0; i < 8; i++)
		init.push_back(0x00);
	SendData(init, 34);
}

void StartToCalculateRSA(char* x, char* n )
{
	

}




void InitProcess(char * byte)
{
	//[1]
	if (getByte(byte, 12, 32) == "10")
	{
		std::vector<unsigned char> init1 = BuildHeader(0x88);
		int version[5] = { 0x09, 0x83, 0x8c, 0xCF, 0x02 };
		for (int i = 0; i < 5; i++)
			init1.push_back(version[i]);
		for (int i = 0; i < 16; i++)
			init1.push_back(byte[i + 12]);
		for (int i = 0; i < 4; i++)
			init1.push_back(byte[i + 28]);
		SendData(init1, 38);
	}

	//[3]
	if (getByte(byte, 12, 32) == "30")
	{
		std::vector<unsigned char> init1 = BuildHeader(0x88);
		int version[5] = { 0x09, 0x83, 0x8c, 0xCF, 0x04 };
		for (int i = 0; i < 5; i++)
			init1.push_back(version[i]);
		for (int i = 0; i < 232; i++)
			init1.push_back(byte[i + 12]);
	}

	

}
