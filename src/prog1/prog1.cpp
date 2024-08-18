#include <thread>
#include <iostream>
#include <exception>
#include <string>
#include <mutex>
#include <cstdio>
#include <vector>
#include <algorithm>


#define PORT 4096

std::exception_ptr eptr;
std::mutex data_mutex;
std::string data;

void thread2() {
    while(true) {
        int count{};
        while(data.empty());
        if(eptr) std::rethrow_exception(eptr);
        std::string str(data);
        data.clear();
        std::cout << str << std::endl;
        for(char c : str) {
            if(std::isdigit(c)) {
                count++;
            }
        }
        
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
            if(str.size() >= 64) {
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
    std::thread thr1(thread1);
    std::thread thr2(thread2);
    while(!eptr);
    std::rethrow_exception(eptr);
    thr1.join();
    thr2.join();
}