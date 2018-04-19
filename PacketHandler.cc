#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>


#include "header/Util.h"
#include "header/crypto.h"
#include "header/Packet.h"
#include "header/PacketHandler.h"



#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT 9987


void die(char *s)
{
    std::cout << s << std::endl;
    exit(1);
}


struct sockaddr_in si_other;
int s, i, slen=sizeof(si_other);
char buf[BUFLEN];
char message[BUFLEN];


void startingUDPSocket(int port, std::string adress)
{

    std::cout << "Connection (...)" << std::endl;
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }


    StartingInitProcess();
    FetchData();
}


void SendData(std::vector<uint8_t> buff, int leng) {
    sendto(s, reinterpret_cast<const char*>(buff.data()), leng, 0, (struct sockaddr *) &si_other, slen);
    std::cout << "[OUT] :: "<< byte_2_str_c(reinterpret_cast<char*>(buff.data()), leng) << std::endl;
}


void FetchData()
{

    std::cout << "Starting to fetch message ... " << std::endl;
    while (1)
    {
        memset(buf,'\0', BUFLEN);
        recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, reinterpret_cast<socklen_t *>(&slen));
        std::cout << "[IN] :: "<< byte_2_str_c(reinterpret_cast<char*>(buf), sizeof(buf)) << std::endl;
        GetPacket(buf, sizeof(buf));
    }
}
 
