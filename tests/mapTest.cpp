#include <iostream>

#include "map.h"
#include<unistd.h>

int main(){
    Map m;
    std::cout<<m.topLeft()<<"XX";
    std::cout<<m.topRight()<<"XX";
    std::cout<<m.botLeft()<<"XX";
    std::cout<<m.botRight()<<"XX";
    std::cout.flush();

    m.setPoint(m.topLeft().right(), "yy", 42, 33);
    m.setPoint(m.topRight().left(), "yy", 42, 33);
    m.setPoint(m.botLeft().right(), "yy", 42, 33);
    m.setPoint(m.botRight().left(), "yy", 42, 33);
    m.clearRow(0);
    m.clearCol(1);
    std::cout<<std::endl;
    std::cout<<"Map size:"<<m.getSize().x<<" "<<m.getSize().y<<std::endl;
    std::cout<<m.getFreePoints().size()<<std::endl;
    std::cout<<m.getUsedPoints().size()<<std::endl;
    sleep(5);
}