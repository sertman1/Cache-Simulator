#ifndef CSF_ASSIGN03_CACHEHELPERS_H
#define CSF_ASSIGN03_CACHEHELPERS_H

#include <string>

// Ensures that the user's command line args allow
// for a valid cache to be simulated
int valid_commandline_args(int argc, char** argv);

// Used to convert the hex instructions provided in 
// the data files into binary before passing them to 
// the cache for interpretation
std::string get_binary_address(std::string input);

#endif 
