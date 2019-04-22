#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> status;
int count = 1000000;

void ping() {
    
    while(count) {
        
        if (status) {
            count--;
            std::cout << "ping" << std::endl;
            status = 0;
        }
    }
}

void pong() {

    while(count) {
        if (!status) {
            count--;
            std::cout << "pong" << std::endl;
            status = 1;
        }
    }
}

int main() {
    
    status = 1;
    std::thread p1(ping);
    std::thread p2(pong);
    
    p1.join();
    p2.join();
    
}
