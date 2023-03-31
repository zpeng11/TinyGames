#include "map.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cassert>

const std::string ESC_PREFFIX = "\x1b[";

const std::string ESC_RESET_COLER = "\x1b[0m";
const std::string ESC_BACKGROUND_COLER = "\x1b[47m";

std::ostream &operator<<(std::ostream &os, const Map::Point &p){
    os<<ESC_PREFFIX<<(p.y + 1)<<";"<<(p.x * 2 + 1)<<"H";
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
    std::cout<<"\x1b[?25l";//hide curser
    for(int i = 0; i < size.x; i++){
        for(int j = 0; j < size.y; j++){
            Map::Point p{i, j};
            this->freePoints.insert(p);
            this->set(p, "  ", 47, -1);
        }
    }
    //Scroll down to get a clear space
}

Map::~Map(){
    std::cout<<ESC_RESET_COLER;
    this->clearAll();
    std::cout<<"\033[?25h";//show curser
}

bool Map::validPoint(const Map::Point& p){
    return p.x >= 0 && p.y >= 0 && p.x < this->size.x && p.y < this->size.y;
}

void Map::clearAll(){
    std::cout<<"\x1b[2J";//Clear all content
    while(this->usedPoints.size()){
        this->freePoints.insert(*this->usedPoints.begin());
        this->usedPoints.erase(this->usedPoints.begin());
    }
}

void Map::clearCol(int col){
    for(int i = 0 ; i < this->size.y; i++){
        Map::Point p{col, i};
        clearPoint(p);
    }
}

void Map::clearRow(int row){
    for(int i = 0 ; i < this->size.x; i++){
        Map::Point p{i, row};
        clearPoint(p);
    }
}

void Map::setPoint(const Map::Point& p, std::string str, int backgroundColor, int fontColor){
    this->set(p, str, backgroundColor, fontColor);
    auto found = this->freePoints.find(p);
    if(found != this->freePoints.end()){
        this->usedPoints.insert(*found);
        this->freePoints.erase(*found);
    }
}

void Map::clearPoint(const Map::Point& p){
    this->set(p, "  ", 47, -1);
    auto found = this->usedPoints.find(p);
    if(found != this->usedPoints.end()){
        this->freePoints.insert(*found);
        this->usedPoints.erase(*found);
    }
}

void Map::set(const Map::Point& p, std::string str, int backgroundColor, int fontColor){
    if(this->validPoint(p) == false){
        assert(this->validPoint(p) == true);
    }
    std::cout<<p; //Move to position
    if(fontColor > 0)
        std::cout<<ESC_PREFFIX<<"0;"<<backgroundColor<<';'<<fontColor<<'m';
    else
        std::cout<<ESC_PREFFIX<<"0;"<<backgroundColor<<'m';
    std::cout<<str<<ESC_RESET_COLER;
    std::cout.flush();
}