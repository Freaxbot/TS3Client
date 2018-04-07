#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")

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
#include "Crypto.h"

SOCKET s;
SOCKADDR_IN addr;
long rc;

void StartingInitProcess();

int startWinsock(void)
{
	WSADATA wsa;
	return WSAStartup(MAKEWORD(2, 0), &wsa);
}
	char buf[256];

	SOCKADDR_IN remoteAddr;
	int			remoteAddrLen = sizeof(SOCKADDR_IN);

void startingUDPSocket(int port, std::string adress)
{

	rc = startWinsock();
	if (rc != 0)
	{
		MessageBoxA(NULL, "Failed to start WinSocket :" + rc, "Error", MB_OK | MB_ICONERROR);
		return;
	}
	else
	{
		std::cout << "Winsocket started..." << std::endl;
	}

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
	{
		MessageBoxA(NULL, "Failed to start udp client: " + WSAGetLastError() + rc, "Error", MB_OK | MB_ICONERROR);
		return;
	}

	std::cout << "UDP Socket erstellt!" << std::endl;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9987);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	std::cout << "Connected!" << std::endl;

	StartingInitProcess();
	FetchData();
}


void SendData(std::vector<uint8_t> buff, int leng)
{
	rc = sendto(s, reinterpret_cast<const char*>(buff.data()), leng, 0, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (rc == SOCKET_ERROR)
		std::cout << "Socket error. Restart application :" << WSAGetLastError();
	std::cout << "[OUT] :: "<< byte_2_str_c(reinterpret_cast<char*>(buff.data()), leng) << std::endl;
}


void FetchData() 
{
	while (1)
	{
		char buf[256];
		rc = recvfrom(s, buf, 256, 0, (SOCKADDR*)&remoteAddr, &remoteAddrLen);
		if (rc == SOCKET_ERROR)
		{
			printf("Fehler: recvfrom, fehler code: %d\n", WSAGetLastError());
			return;
		}
		else
		{
			std::cout << "[IN] :: " << byte_2_str_c(reinterpret_cast<char*>(buf), rc) << std::endl;
			GetPacket(buf, rc);
		}
	}
}
