#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <sstream>


constexpr uint64_t SIZE = 1000000;

std::mutex mutex;

void connect(std::string &f1, std::string &f2, std::string &result) {
    
    std::ifstream file_1(f1, std::ios::binary);
    std::ifstream file_2(f2, std::ios::binary);
    std::ofstream result_(result, std::ios::binary);
    
    uint64_t first, second;
    
    file_1.read((char*)(&first), sizeof(first));
    file_2.read((char*)(&second), sizeof(second));
    
    while(!file_1.eof() && !file_2.eof()) {
        
        if (first > second) {
            result_.write((char*)(&second), sizeof(second));
            file_2.read((char*)(&second), sizeof(second));
        } else {
            result_.write((char*)(&first), sizeof(first));
            file_1.read((char*)(&first), sizeof(first));
        }
    }
    
    std::ifstream* answer;
    if (file_1.eof()) {
        
        answer = &file_2;
        std::swap(first, second);
    } else {
        answer = &file_1;
        
    }
    while(!(*answer).eof()) {
        result_.write((char*)(&first), sizeof(first));
        (*answer).read((char*)(&first), sizeof(first));
    }
    file_1.close();
    file_2.close();
    result_.close();
}

void process(std::ifstream &file, int id) {

    int count = 0;
    
    std::ostringstream os;
    os<<id<<"_";
    
    while(!file.eof()) {
        mutex.lock();
        
        uint64_t now;
        std::vector<uint64_t> v;
        
        for(int i = 0; i < SIZE; ++i) {
            file.read((char*)(&now), sizeof(now));
            if(file.eof()) {
                break;
            }
            v.push_back(now);
        }

        if (v.empty()) {
            break;
        }
        mutex.unlock();
        std::sort(v.begin(), v.end());
        
        std::ofstream out(os.str() + std::to_string(++count), std::ios::binary);
        out.write((char*)v.data(), v.size() * sizeof(uint64_t));
        
        std::this_thread::yield();
    }
    
    while (count > 1) {
        mutex.lock();
        
        std::string f_1 = os.str() + std::to_string(count--);
        
        std::string f_2 = os.str() + std::to_string(count);
        std::string now = os.str() + std::string("now");
        connect(std::ref(f_1), std::ref(f_2), std::ref(now));
        
        mutex.unlock();
        
        std::remove(f_1.c_str());
        std::remove(f_2.c_str());
        std::rename(now.c_str(), f_2.c_str());
        
        std::this_thread::yield();
    }
}

int main() {

    std::ifstream fin("hello", std::ios::binary);
    std::thread th_1(process, std::ref(fin), 1);
    std::thread th_2(process, std::ref(fin), 2);
    
    std::string part_1 = "1_1";
    std::string part_2 = "2_1";
    std::string answer("output");
    
    th_1.join();
    th_2.join();
    
    fin.close();
    
    std::ifstream p1(part_1);
    std::ifstream p2(part_2);
    
    if(p1.good() && p2.good()) {
        
        connect(part_1, part_2, answer);
        std::remove(part_1.c_str());
        std::remove(part_2.c_str());
        
    } else {
        if (p1.good()) {
            std::rename(part_1.c_str(), answer.c_str());
        } else {
            std::rename(part_2.c_str(), answer.c_str());
        }
    }
    return 0;
}