#include <iostream>
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif

int main(){
    printf("\x1b[1049h");
    printf("\x1b[32mHello, World\n");
    printf("\x1b[0m");
    printf("\x1b[1049l");
}