#include "Cache.h"
#include "cacheHelpers.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (!valid_commandline_args(argc, argv)) {
        return -1;
    }   
    Cache cache = Cache(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), argv[4], argv[5], argv[6]);

    std::string inputBuff;
    std::string memoryAddress;
    while (std::getline(std::cin, inputBuff)) {
        memoryAddress = get_binary_address(inputBuff);
        if (inputBuff.at(0) == 's') {  
            cache.store(memoryAddress);
        } else { 
            cache.load(memoryAddress);
        }
    }

    cache.print_data();
    return 0;
}
