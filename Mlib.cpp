#include "Mlib.h"   

int main(){
    Cmd cmd;
    cmd.mlib_rebuild();
    cmd.mlib_append("g++", "main.cpp", "-o", "main");
    cmd.addPath("./main.cpp");
    cmd.mlib_watch();
    return 0;
}