#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstdio>
#include <errno.h>
#include "../MySocket/my_socket.hpp"


int main() {
    Socket socket{};
    socket.bind_to();
    while(true) {
        int x;
        if(socket.recv_from(&x, sizeof(x)) > sizeof(int)) {
            throw std::runtime_error("Incorrect data read.");
        }
        if( (x / 100 > 0) && (x % 32 == 0) )
            std::cout << "Data received. " << x << std::endl;
        else 
            std::cout << "The data does not meet the requirements. " << x << std::endl;
    }
}