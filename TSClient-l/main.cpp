#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")

#include "PacketHandler.h"
#include <winsock2.h>
#include <string>
#include <iostream>


int main()
{
	std::cout << "Starting TS3Client ";
	startingUDPSocket(9987, "127.0.0.1");

  return 0;
}

