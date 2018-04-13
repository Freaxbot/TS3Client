

#include <iomanip>
#include <tomcrypt.h>
#include <tommath.h>

#include "header/PacketHandler.h"
#include "header/Packet.h"
#include "header/Util.h"

void InstallTomCrypt() 
{
ltc_mp = ltm_desc;
}


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



        rsa_key key;
        const char* n = "12195768979127026417187603714036391958048299327164883087603312522730977833600860696919578595717794043319551957556836036894416267108725971086946438691747253";
        unsigned char n_buf[1024];
        unsigned long n_len = sizeof(n_buf);

        const char* e = "12195768979127026417187603714036391958048299327164883087603312522730977833600860696919578595717794043319551957556836036894416267108725971086946438691747253";
        unsigned char e_buf[1024];
        unsigned long e_len = sizeof(e_buf);


        const char* d = "12195768979127026417187603714036391958048299327164883087603312522730977833600860696919578595717794043319551957556836036894416267108725971086946438691747253";
        unsigned char d_buf[1024];
        unsigned long d_len = sizeof(d_buf);

        if (radix_to_bin(n, 10, n_buf, &n_len) != CRYPT_OK) exit(-1);
        if (radix_to_bin(e, 10, e_buf, &e_len) != CRYPT_OK) exit(-1);
        if (radix_to_bin(d, 10, d_buf, &d_len) != CRYPT_OK) exit(-1);


        if(rsa_set_key(n_buf, n_len, e_buf, e_len, d_buf, d_len, &key) != CRYPT_OK) exit(-1);
    }



}
