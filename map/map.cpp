#include "map.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

const std::string ESC_PREFFIX = "\x1b[";

const std::string ESC_RESET_COLER = "\x1b[0m";
const std::string ESC_BACKGROUND_COLER = "\x1b[47m";

std::ostream &operator<<(std::ostream &os, const Map::Point &p){
    os<<ESC_PREFFIX<<p.x<<";"<<p.y<<"f";
}



Map::Map(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    this->size.x = w.ws_col/2;
    this->size.y = w.ws_row;
    //Scroll down to get a clear space
    for(int i = 0 ; i < size.y; i++){
        std::cout<<'\n';
    }
    for(int i = 0; i < size.x; i++){
        for(int j = 0; j < size.y; j++){
            this->freePoints.insert({i, j});
        }
    }
    //Scroll down to get a clear space

    std::cout<<"\x1b[3J";
}

