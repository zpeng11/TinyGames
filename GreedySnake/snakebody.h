#ifndef SB
#define SB

#include "Types.h"
#include "map.h"
#include <list>
//rand srand time




class SnakeBody
{
    friend void MoveForward(Map & m, SnakeBody & sb);
    friend void * KeyHandle(void * arg);
    private:
    std::list<Posi> body;
    public:
    Direction dir;
    SnakeBody( Map &m, int len = 2);
};


#endif