#include <thread>
#include <iostream>
#include <exception>
#include <string>
#include <cstdio>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include "../MySocket/my_socket.hpp"


std::exception_ptr eptr;


struct DataProcessor {
    DataProcessor(size_t reserved_b=128) {
        data.reserve(reserved_b);
    }

    int get_sum(std::string& str) {
        int sum{};
        for(char c : str) {
            if(std::isdigit(c)) {
                sum += c - '0';
            }
        }
        return sum;
    }

    void find_letter(std::string& str) {
        for(auto c : str) {
            if(!std::isdigit(c) ) {
                throw std::runtime_error {"The input string must not contain letters."};
            } 
        }
    }

    void sort_data(std::string& str) {
        std::sort(str.begin(), str.end());
    }

    void replace_even_with(std::string& str, const char* substr = "KB") {
        auto itr = str.begin();
        auto begin = str.begin();
        auto end = str.end();
        while(itr != end) {
            if ((*itr - '0') % 2 == 0) {
                int i = itr - begin;
                str.replace(i, 1, substr);
                end = str.end();
                itr = begin + i + 1;
            }
            itr++;
        }
    }
    std::string& get_data() {
        return data;
    }
    void clear_data() {
        data.clear();
    }
    void set_data(std::string& str) {
        data = str;
    }
    void show_data(std::ostream& stream=std::cout) {
        stream << data << "\n";
    }
    std::mutex data_mutex;
    std::condition_variable cv;
    private:
        std::string data;
};



void thread2(DataProcessor& dp) {
    Socket socket{};
    
    while(true) {        
        std::unique_lock lock(dp.data_mutex);
        dp.cv.wait(lock);
        if(eptr) std::rethrow_exception(eptr);
        std::string str(dp.get_data());
        dp.clear_data();
        dp.cv.notify_all();
        std::cout << str << "\n";
        int sum = dp.get_sum(str);
        socket.connect_to();
        socket.send_to(sum);
    }
}

void thread1(DataProcessor& dp) {
    try {
        while(true) {
            if(eptr) std::rethrow_exception(eptr);
            std::string str;
            std::cin >> str;
            int str_size = str.size();
            if(str_size > 64) {
                throw std::runtime_error{"The input string must not be greater than 64. "};
            }
            str.reserve(128);
            dp.sort_data(str);
            dp.find_letter(str);
            dp.replace_even_with(str);
            std::unique_lock lock(dp.data_mutex);
            dp.set_data(str); 
            dp.cv.notify_all();
            dp.cv.wait(lock);   
        }
    } catch (...) {
        eptr = std::current_exception();
    }
}

int main() {
    DataProcessor dp{};
    std::thread thr1(thread1, std::ref(dp));
    std::thread thr2(thread2, std::ref(dp));
    while(!eptr);
    std::rethrow_exception(eptr);
    thr1.join();
    thr2.join();
}