#include <thread>
#include <iostream>
#include <exception>
#include <string>
#include <cstdio>
#include <algorithm>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 4096
int sock;
struct sockaddr_in addr;

std::exception_ptr eptr;
std::string data;

void thread2() {
    while(true) {
        int sum{};
        while(data.empty());
        if(eptr) std::rethrow_exception(eptr);
        std::string str(data);
        data.clear();
        std::cout << str << std::endl;
        for(char c : str) {
            if(std::isdigit(c)) {
                sum += c - '0';
            }
        }
        send(sock, &sum, sizeof(sum), 0);
    }
}

void thread1() {
    try {
        while(true) {
            std::string str;
            str.reserve(128);
            while(!data.empty());
            char c;
            while( (c = getchar()) != '\n') {
                if(!std::isdigit(c) ) {
                    throw std::runtime_error {"The input string must not contain letters."};
                } 
                str.push_back(c);
            }
            if(str.size() > 64) {
                throw std::runtime_error {"The maximum size of the input string cannot exceed 64."};
            }
            std::sort(str.begin(), str.end());
            auto itr = str.begin();
            auto begin = str.begin();
            auto end = str.end();
            while(itr != end) {
                if ((*itr - '0') % 2 == 0) {
                    int i = itr - begin;
                    str.replace(i, 1, "KB");
                    end = str.end();
                    itr = begin + i + 1;
                }
                itr++;
            }
            data = str;
        }
    } catch (...) {
        eptr = std::current_exception();
    }
}

int main() {
    data.reserve(128);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    std::thread thr1(thread1);
    std::thread thr2(thread2);
    while(!eptr);
    std::rethrow_exception(eptr);
    thr1.join();
    thr2.join();
}