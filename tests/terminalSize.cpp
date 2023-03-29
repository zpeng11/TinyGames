#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

int main (int argc, char **argv)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col/2);
    for(int i = 0 ;i <w.ws_row + 1; i++){
        printf("\n");
    }

    // for(int i = 1 ; i <= w.ws_row; i++){
    //     for(int j = 0 ; j < w.ws_col/2; j++){
    int i = w.ws_row+1; 
    int j = w.ws_col;
            printf("\x1b[%i;%if", 1 + i, j * 2);
            printf("\x1b[0;47m");
            printf("XX");
            printf("\x1b[0m");
    //     }
    // }
    printf("\x1b[0m");
    //printf("\x1b[2J");
    return 0;  // make sure your main returns int
}