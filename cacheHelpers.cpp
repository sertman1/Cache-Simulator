#include <iostream>
#include <string.h>
#include <cmath>
using std::endl;
using std::cerr;
using std::string;

// Helper functions only used in this file:
bool pow_of_two(int num);
string hex_bit_to_binary(char bit);

bool valid_commandline_args(int argc, char* argv[]) {
    if (argc != 7) {
        cerr << "incorrect number of command line arguments" << endl;
        return false;
    }
    if (atoi(argv[1]) < 0 || !pow_of_two(atoi(argv[1]))) {
        cerr << "number of sets is not a positive power of 2" << endl;
        return false;
    } else if (atoi(argv[2]) < 0 || !pow_of_two(atoi(argv[2]))) {
        cerr << "number of blocks is not a positive power of 2" << endl;
        return false;
    } else if (atoi(argv[3]) < 4 || !pow_of_two(atoi(argv[3]))) {
        cerr << "number of bytes is not a positive power of 2 greater than or equal to 4" << endl;
        return false;
    } else if (std::log2(atoi(argv[1])) + std::log2(atoi(argv[3])) > 32) {
        cerr << "number of sets and bytes per block are too large to be represented as 32 bit memory addresses";
        return false;
    }
    if (strcmp(argv[4], "write-allocate") != 0 && strcmp(argv[4], "no-write-allocate") != 0) {
        cerr << "'write-allocate' or 'no-write-allocate' not properly specified" << endl;
        return false;
    }
    if (strcmp(argv[5], "write-through") != 0 && strcmp(argv[5], "write-back") != 0) {
        cerr << "'write-through' or 'write-back' not properly specified" << endl;
        return false;
    }
    if (strcmp(argv[4], "no-write-allocate") == 0 && strcmp(argv[5], "write-back") == 0) {
        cerr << "cannot simulator cache thats write-back and no-write-allocate" << endl;
        return false;
    }
    if (strcmp(argv[6], "lru") != 0 && strcmp(argv[6], "fifo") != 0) {
        cerr << "'lru' or 'fifo' not properly specified" << endl;
        return false;
    }
    return true;
}

string get_binary_address(string input) {
    string address;
    int i = 2;
    while (input.at(i - 2) != '0' && input.at(i - 1) != 'x') {
        i++;
    }
    int counter = 0; 
    while (input.at(i + counter) != ' ') { // i equals index of first hex bit //
        counter++; // count number of hex bits in representation
    }
    if (counter != 8) { // add leading, padding zeros to binary representation if not 8 hex bits
        while (counter != 8) {
            address += "0000";
            counter++;
        }
    }
    while (input.at(i) != ' ') {
        address += hex_bit_to_binary(input.at(i));
        i++;
    }
    return address;
}

// Implementation of functions only used in this file:
bool pow_of_two(int num) {
    if (num == 1) {
        return true;
    } else if (num % 2 != 0) {
        return false;
    }
    int power = 1;
    while(power < num) {
        power = power << 1;
        if (num / power == 0) {
            return false;
        }
    }
    return true;
}

string hex_bit_to_binary(char bit) {
    switch (bit) {
        case '0':
            return "0000";
        case '1':
            return "0001";
        case '2':
            return "0010";
        case '3':
            return "0011";
        case '4':
            return "0100";
        case '5':
            return "0101";
        case '6':
            return "0110";
        case '7':
            return "0111";
        case '8':
            return "1000";
        case '9':
            return "1001";
        case 'a':
        case 'A':
            return "1010";
        case 'b':
        case 'B':
            return "1011";
        case 'c':
        case 'C':
            return "1100";
        case 'd':
        case 'D':
            return "1101";
        case 'e':
        case 'E':
            return "1110";
        case 'f':
        case 'F':
            return "1111";
    }
    return NULL; // this will never be reached!
}
