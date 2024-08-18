#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstdio>
#include <errno.h>

#define PORT 4096
int sock;
struct sockaddr_in addr;

int main() {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    while(true) {
        int x;
        if(recvfrom(sock, &x, sizeof(int), 0, NULL, NULL) > sizeof(int)) {
            throw std::runtime_error("Incorrect data read.");
        }
        if( (x / 100 > 0) && (x % 32 == 0) )
            std::cout << "Data received. " << x << std::endl;
        else 
            std::cout << "The data does not meet the requirements. " << x << std::endl;
    }
}