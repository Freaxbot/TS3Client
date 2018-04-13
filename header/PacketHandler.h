#include <vector>
#include <string>


void startingUDPSocket(int port, std::string adress);
void SendData(std::vector<uint8_t> buff, int leng);
void FetchData();