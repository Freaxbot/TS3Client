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

void InitCrypto();

static short InitFinish = 0;
static short IsConnected = 0;
static long unsigned int taglength = 8;
static int packetid = 0;
static char PIDC[2];
static char* CIDC = "0001";




void InstallTomCrypt() 
{
    ltc_mp = ltm_desc;
    std::cout << "Libtomcrypt + Libtomath installed!" << std::endl;
    InitCrypto();
}


void IncommingPacket(char* data)
{

}
    unsigned char nonceD[16] = {0x6D, 0x5C, 0x66, 0x69, 0x72, 0x65, 0x77, 0x61, 0x6C, 0x6C, 0x33, 0x32, 0x2E, 0x63, 0x70, 0x6C};
    unsigned char keyD[16] = {0x63, 0x3A, 0x5C, 0x77, 0x69, 0x6E, 0x64, 0x6F, 0x77, 0x73, 0x5C, 0x73, 0x79, 0x73, 0x74, 0x65};
    unsigned char key[16], nonce[16];

static eax_state eax;

void InitCrypto()
{
    if (register_cipher(&rijndael_desc) == -1) {
        printf("Error registering Rijndael");
        return;
    }
}

char* GetPacketId()
{

    if (packetid >= 65535)
    {
        printf("[ERROR] Overflow PacketId counter higher (max 65535) \n");
        return NULL;
    }
    PIDC[1] = packetid & 255;
    PIDC[0] = (packetid >> 8) & 255;
    std::cout << "[PacketID] :: "  <<byte_2_str_c(reinterpret_cast<char*>(PIDC), sizeof(PIDC)) << std::endl;
    packetid++;
    return PIDC;
}



void Decrypt(char* byte, int length)
{
    int dataLen = length-11;
    int macLen = 8;
    int state = 2;


    std::cout << "lhjkL::: " << length << std::endl;
    
    unsigned char pt[dataLen + macLen -18];
    unsigned char header[3], mac[8], data[dataLen];
    memcpy(header, &byte[8], 3);
    memcpy(mac, &byte[0], 8);
    memcpy(data, &byte[11], length-11);
    if (InitFinish == 0){

        eax_decrypt_verify_memory(
            find_cipher("rijndael"),
            keyD, 16,
            nonceD, 16,
            reinterpret_cast<unsigned char*>(header), 3,
            reinterpret_cast<unsigned char*>(data), dataLen,
            pt, reinterpret_cast<unsigned char*>(mac), 8, &state
        );

        std::cout << "[ENCODER] :: "<< byte_2_str_c(reinterpret_cast<char*>(pt), sizeof(pt)) << std::endl;

        std::cout << "State :: " << state << std::endl;
        std::cout << "PlainText :: " << pt << std::endl;

    }else{
        //ToDo
    }

}


unsigned char* Encrypt(char* data, int length, short type)
{  
  
    int pTypeInt = 0 << type;
    char Ptype[1];
    Ptype[0] = pTypeInt & 255;

//Build encrypt Header 
 //int MAC2[8] = {0xA4, 0x7B, 0x47, 0x94, 0xDB, 0xA9, 0x6A, 0xC5};

  unsigned char ct[64], tag[8];
  

    char* header[5];
    memcpy(header, &GetPacketId(), 2);
    memcpy(header, &CIDC, 2);
    memcpy(header, reinterpret_cast<char*>(Ptype), 1);
   
    unsigned char nonce[16] = {0x6D, 0x5C, 0x66, 0x69, 0x72, 0x65, 0x77, 0x61, 0x6C, 0x6C, 0x33, 0x32, 0x2E, 0x63, 0x70, 0x6C};
    unsigned char key[16] = {0x63, 0x3A, 0x5C, 0x77, 0x69, 0x6E, 0x64, 0x6F, 0x77, 0x73, 0x5C, 0x73, 0x79, 0x73, 0x74, 0x65};

    eax_encrypt_authenticate_memory(
        find_cipher("rijndael"),
        key, 16,
        nonce, 16,
        reinterpret_cast<unsigned char*>(header), 5,
        reinterpret_cast<unsigned char*>(data), length,
        ct, tag, &taglength  );

        std::cout << "Ack byte :: " << byte_2_str_c(reinterpret_cast<char*>(ct), 2) << std::endl;
        std::cout << "Mac byte :: " << byte_2_str_c(reinterpret_cast<char*>(tag), 8) << std::endl;
        std::cout << "Header byte :: " << byte_2_str_c(reinterpret_cast<char*>(header), 5) << std::endl;

     std::vector<unsigned char> outH;

    for(int i = 0; i < 8; i++){
        outH.push_back(tag[i]);
    }  
    for(int i = 0; i < 5; i++){
        outH.push_back(reinterpret_cast<unsigned char*>(header)[i]);
    }             
    for(int i = 0; i < 2; i++){
        outH.push_back(ct[i]);
    }     

       SendData(outH, 15);

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
//ToDo to util
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

        std::cout <<xCommand << std::endl;

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
    unsigned char buf[128], buff_p[128];
    unsigned long y, x;
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
    x = sizeof(buff_p);
    ecc_export(buf, &y, PK_PUBLIC, &mykey);
    ecc_export(buff_p, &x, PK_PRIVATE, &mykey);
    ecc_free(&mykey);
    std::cout << "Private Key :: " << encodeBase64((const char*) buff_p, x) << std::endl;
    return encodeBase64((const char*) buf, y);
 }

/*

*/