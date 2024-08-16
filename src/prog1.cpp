#include <thread>
#include <iostream>
#include <exception>
#include <string>
#include <mutex>
#include <cstdio>
#include <vector>
#include <algorithm>

std::exception_ptr eptr;
std::mutex data_mutex{};
std::string data;

void thread2() {
    while(true) {
        std::cout << "hi from t2" << std::endl;
        std::lock_guard<std::mutex> lock(data_mutex);
        if(eptr) std::rethrow_exception(eptr);
        std::cout << "t2 owns mutex" << std::endl;
        data.clear();
    }
    
    

}

void thread1() {
    try {
        while(true) {
            while(!data.empty());
            std::cout << "hi from t1" << std::endl;
            std::lock_guard<std::mutex> lock(data_mutex);
            std::cout << "t1 owns mutex" << std::endl;
            char c;
            while( (c = getchar()) != '\n') {
                if(!std::isdigit(c) ) {
                    throw std::runtime_error {"The input string must not contain letters."};
                } 
                data.push_back(c);
            }
            if(data.size() >= 64) {
                throw std::runtime_error {"The maximum size of the input string cannot exceed 64."};
            }
            std::sort(data.begin(), data.end());
            auto itr = data.begin();
            auto begin = data.begin();
            auto end = data.end();
            while(itr != end) {
                if ((*itr - '0') % 2 == 0) {
                    int i = itr - begin;
                    data = data.replace(i, 1, "KB");
                    end = data.end();
                    itr = begin + i + 1;
                }
                itr++;
            }
            std::cout << data << std::endl;
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