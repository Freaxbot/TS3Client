#include <iomanip>
#include <tomcrypt.h>
#include <tommath.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

//Debug
#include <typeinfo>

#include "header/PacketHandler.h"
#include "header/Packet.h"
#include "header/Util.h"


#define mp_toint(a, b) mp_toradix(a, b, 16)
#define mp_tointTest(a, b) mp_toradix(a, b, 10)

void InstallTomCrypt() 
{
    ltc_mp = ltm_desc;
    std::cout << "Libtomcrypt + Libtomath installed!" << std::endl;
}


void IncommingPacket(char* data)
{

}


std::string TS3InitMac = "TS3INIT1";

//Decrypt <3
void Decrypt(char* byte, int length)
{
   unsigned char Header[30], Data[4069], Mac[80];
       
        memcpy(Header, &byte[8], 3);
        memcpy(Mac, &byte[0], 8);
        memcpy(Data, &byte[8 + 3], length - 8 - 3);

      // std::cout << "Header :: " << byte_2_str(Header, 3) << std::endl;
        std::cout << "Mac :: " << byte_2_str((char*)Mac, 8) << std::endl;
        std::cout << "Data :: " << byte_2_str_c((char*)Data, length - 8 - 3) << std::endl;

        int err;
        eax_state eax;
        unsigned char pt[64], ct[64], tag[16];

        unsigned char nonce[16] = {0x6D, 0x5C, 0x66, 0x69, 0x72, 0x65, 0x77, 0x61, 0x6C, 0x6C, 0x33, 0x32, 0x2E, 0x63, 0x70, 0x6C};
        unsigned char key[16] = {0x63, 0x3A, 0x5C, 0x77, 0x69, 0x6E, 0x64, 0x6F, 0x77, 0x73, 0x5C, 0x73, 0x79, 0x73, 0x74, 0x65};


        if (register_cipher(&rijndael_desc) == -1) {
            printf("Error registering Rijndael");
            return;
        }

       if ((err = eax_init( &eax, /* context */
            find_cipher("rijndael"), /* cipher id */
            key,
            16,
            nonce, /* the nonce */
            16, /* nonce is 16 bytes */
            Mac, /* example header */
            8) /* header length */
            ) != CRYPT_OK) {
            printf("Error eax_init: %s", error_to_string(err));
            return;
        }

        eax_decrypt( &eax, reinterpret_cast<unsigned char*>(Data), ct,  sizeof(Data));
        std::cout << "Encrypt byte :: " << byte_2_str_c(reinterpret_cast<char*>(ct), sizeof(ct)) << std::endl;
        std::cout << "Encrypt text :: " << ct << std::endl;
    



}

std::string GenerateOmega();

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
//to util
int getHex(std::string hexstr) {
    return (int)strtol(hexstr.c_str(), 0, 16);
}

    //Base encoder helper
    std::string encodeBase64(const char* input, const unsigned long inputSize) {
        unsigned long outlen = inputSize + (inputSize / 3.0) + 16;
        char* outbuf = new char[outlen];
        base64_encode((unsigned char*) input, inputSize, outbuf, &outlen);
        std::string ret((char*) outbuf, outlen);
        delete[] outbuf;
        return ret;
    }

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

    std::string CommandEncoder(std::string cmd)
    {
        replaceAll( cmd, "\\", "\\\\");
        replaceAll( cmd, " ", "\\s");
        replaceAll( cmd, "/", "\\/");
        replaceAll( cmd, "|", "\\p");
        replaceAll( cmd, "\b", "\\b");
        replaceAll( cmd, "\f", "\\f");
        replaceAll( cmd, "\n", "\\n");
        replaceAll( cmd, "\r", "\\r");
        replaceAll( cmd, "\t", "\\t");
        replaceAll( cmd, "\v", "\\v");
        return cmd;
       
    }

    char* CommandDecoder(char* text)
    {
        //base32 + utf-8
    }



char* StartToCalculateRSA(std::string XR, std::string NR, std::string LVL )
{
        static char buff[4096];
        mp_int x, n, X, e, Xa;
        mp_init(&x);
        mp_init(&n);
        mp_init(&X);
        mp_init(&Xa);
        mp_init(&e);

        // Calculate RSA  x ^ (2 ^ level) % n
        mp_read_radix(&x, XR.c_str(), 16);
        mp_read_radix(&n, NR.c_str(), 16);
        mp_set(&e, 2);
        mp_expt_d_ex(&e, getHex(LVL) , &Xa, 1);
        mp_exptmod(&x, &Xa, &n, &X);
        mp_toint(&X, buff);
        printf("RSA :: %s\n", buff);
        mp_clear(&x);
        mp_clear(&n);
        mp_clear(&X);
        mp_clear(&Xa);
        mp_clear(&e);
        return buff;
}


void InitProcess(char* byte, int length)
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
        std::string oldBuf = byte_2_str(byte, length);
        //declaraion of the working vars.
        char vX[64], vN[64], vL[4], a2[100];
       
        memcpy(vX, &byte[12], 64);
        memcpy(vN, &byte[12 + 64], 64);
        memcpy(vL, &byte[12 + 64 + 64], 4);
        memcpy(a2, &byte[12 + 64 + 64 + 4], 100);
    
        auto y = StartToCalculateRSA(byte_2_str(vX, 64), byte_2_str(vN, 64), byte_2_str(vL, 4));
    
        std::cout << "X :: " << byte_2_str(vX, 64) << std::endl;
        std::cout << "N :: " << byte_2_str(vN, 64) << std::endl;
        std::cout << "LVL :: " << byte_2_str_c(vL, 4) << std::endl;
        std::cout << "YRAW :: " << y << std::endl;
        std::string xCommand = "clientinitiv alpha="+encodeBase64("hsidikgrt0", 10)+" omega=" + GenerateOmega() + " ot=1 ip=";

        int lCmd = xCommand.length();
        char* cmdBuf = strdup(xCommand.c_str());

        int int5 = 0;
        int i_auto;
        std::vector<unsigned char> init2 = BuildHeader(0x88);
        int version[5] = { 0x09, 0x83, 0x8c, 0xCF, 0x04 };
        for (int i = 0; i < 5; i++)
            init2.push_back(version[i]);
        for (int i = 0; i < 64; i++)
            init2.push_back(vX[i]);
        for (int i = 0; i < 64; i++)
            init2.push_back(vN[i]);
        for (int i = 0; i < 4; i++)
            init2.push_back(vL[i]);
        for (int i = 0; i < 100; i++)
            init2.push_back(a2[i]);
        for (int i = 0; i < 64; i++)
        {
            std::stringstream ss;
            ss<< "0x" <<  y[int5] << y[int5 + 1];
            std::string res ( ss.str() );
            i_auto = std::stoi (res,nullptr,0);
            init2.push_back(i_auto);
            int5 = int5 + 2;
        }
        for (int i = 0; i < lCmd; i++)
            init2.push_back(cmdBuf[i]);
        SendData(init2, 314 + lCmd);
    }
}




//Genarate the Identity ecc_key for the server...
std::string GenerateOmega()
{
    ecc_key mykey;
    prng_state prng;
    int err;
    unsigned char buf[128];
    unsigned long y;
    if (register_prng(&yarrow_desc) == -1) 
    {
        printf("Error registering Yarrow\n");
    }
    if ((err = rng_make_prng(128, find_prng("yarrow"), &prng, NULL))!= CRYPT_OK) 
    {
        printf("Error setting up PRNG, %s\n", error_to_string(err));
    }
    if ((err = ecc_make_key(&prng, find_prng("yarrow"), 32, &mykey)) != CRYPT_OK) 
    {
        printf("Error making key: %s\n", error_to_string(err));
    }
    y = sizeof(buf);
    ecc_export(buf, &y, PK_PUBLIC, &mykey);
    ecc_free(&mykey);
    return encodeBase64((const char*) buf, y);
 }

/*

*/