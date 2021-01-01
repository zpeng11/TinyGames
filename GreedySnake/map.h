#ifndef MAP
#define MAP
#include "Types.h"
#include <vector>


MapLimit GetMapLimit(void);

class SnakeBody;


class Map
{
    friend class SnakeBody;
    friend void MoveForward(Map & m, SnakeBody & sb);
    friend void * KeyHandle(void * arg);
    private:
    MapLimit limits;
    std::vector<std::vector<SpaceType>> space;
    public:
    Map(int x=0, int y=0);
    void reflash();
    Posi GetPosi(Posi pos, Direction dir);
    SpaceType GetType(Posi pos);
    SpaceType SpaceSet(Posi pos, SpaceType st);//set the position to st and return the old space type
    Posi SpaceGen();
};


#endif

