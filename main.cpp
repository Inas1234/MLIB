#include <iostream>
#include <vector>
#include <string>
#include "Mlib.h"


int main(int argc, char** argv){
    std::string contents = mlib_read_file("./main.cpp");
    
    std::string args = mlib_get_args(argc, argv);
    std::cout << "Geto  are: " << args << std::endl;
    return 0;
}