#include <iomanip>
#include <tomcrypt.h>
#include <tommath.h>

#include "header/PacketHandler.h"
#include "header/Packet.h"
#include "header/Util.h"


#define mp_toint(a, b) mp_toradix(a, b, 10)

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

void StartToCalculateRSA(char* xsdg, char* nfsdf )
{
        char buff[4096];
        mp_int x, n, X, e, Xa;
        mp_init(&x);
        mp_init(&n);
        mp_init(&X);
        mp_init(&Xa);
        mp_init(&e);
        int level = 10000;

        // Calculate RSA  x ^ (2 ^ level) % n

        mp_read_radix(&x, "32878006774570359216307190512414453734815024711845858985814302013359906676224571864057517441898232179839316906470039235887748992902358250854400787652810336070530280967470770997285440429053186803879037", 10);
        mp_read_radix(&n, "77898130960070341501772069500669364440519531421534783575397763758775619778096560479521554583192022357575799725548012588149166448319424189949242058358050730052508358466295626425829884371399991831978634", 10);
        mp_set(&e, 2);

        mp_expt_d_ex(&e, level , &Xa, 1);
        mp_exptmod(&x, &Xa, &n, &X);

        mp_toint(&X, buff);
        printf("RSA :: %s\n", buff);

        mp_clear(&x);
        mp_clear(&n);
        mp_clear(&X);
        mp_clear(&Xa);
        mp_clear(&e);

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

     std::cout << sizeof(byte) << std::endl;
        std::vector<unsigned char> init1 = BuildHeader(0x88);
        int version[5] = { 0x09, 0x83, 0x8c, 0xCF, 0x04 };
        for (int i = 0; i < 5; i++)
            init1.push_back(version[i]);
        for (int i = 0; i < 64; i++)
            init1.push_back(byte[i + 12]);
        for (int i = 0; i < 64; i++)
            init1.push_back(byte[i + 12 + 64]);
        for (int i = 0; i < 4; i++)
            init1.push_back(byte[i + 12 + 64 + 64]);

  
     std::cout << getByte(byte, 250, 32) << std::endl;


        std::cout << "CODE :: "<< byte_2_str_c(reinterpret_cast<char*>(init1.data()), sizeof(init1)) << std::endl;


    }



}
