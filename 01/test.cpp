#include <iostream>
#include <stdio.h>
#include "numbers.dat"

bool simple(int n) {
    
    if (n == 1) {return false;}
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
        
    }
    return true;
}

bool search(int index, int end) {
    int count = 0;
    while (index < Size && Data[index] <= end) {
        
        if (simple(Data[index]))
            count++;
        index++;
    }
    
    if (Data[index - 1] != end) {
        std::cout << 0 << std::endl;
        return false;
    }
    
    std::cout << count << std::endl;
    return true;
    
}

int main(int argc, char* argv[])
{

    if (argc <= 1 || argc % 2 == 0) {
        return -1;
    }
    
    for (int i = 1; i <= (argc + 1) / 2; i += 2) {
        int start, end;
        sscanf(argv[i], "%d", &start);
        sscanf(argv[i + 1], "%d", &end);
        int index = -1;
        
        for (int j = 0; j < Size; j++) {
            if (Data[j] == start) {
                index = j;
                break;
            }
        }
        
        if (index == -1) {
            std::cout << 0 << std::endl;
            return 0;
        }
        
        if (!search(index, end))
            return 0;
            
    }

    return 0;
}