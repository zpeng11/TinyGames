#include "map.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

const std::string ESC_PREFFIX = "\x1b[";

const std::string ESC_RESET_COLER = "\x1b[0m";
const std::string ESC_BACKGROUND_COLER = "\x1b[47m";

std::ostream &operator<<(std::ostream &os, const Map::Point &p){
    os<<ESC_PREFFIX<<(p.x + 1)<<";"<<(p.y * 2)<<"f";
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
    std::cout<<"\033[?25l";//hide curser
    std::cout<<ESC_BACKGROUND_COLER;
    for(int i = 0; i < size.x; i++){
        for(int j = 0; j < size.y; j++){
            Map::Point p{i, j};
            this->freePoints.insert(p);
            std::cout<<p<<"  ";
        }
    }
    std::cout<<ESC_RESET_COLER;
    //Scroll down to get a clear space

    std::cout<<"\x1b[3J";
}

Map::~Map(){
    std::cout<<ESC_RESET_COLER;
    this->clearAll();
    std::cout<<"\033[?25h";//show curser
}

bool Map::validPoint(const Map::Point& p){
    return p.x > 0 && p.y >0 && p.x <= this->size.x && p.y <= this->size.y;
}

void Map::clearAll(){
    std::cout<<"\x1b[2J";//Clear all content
    while(this->usedPoints.size()){
        this->freePoints.insert(*this->usedPoints.begin());
        this->usedPoints.erase(this->usedPoints.begin());
    }
}

void Map::clearCol(int col){
    for(int i = 0 ; i < )
}