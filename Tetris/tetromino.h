#ifndef TETROMINO
#define TETROMINO

#include "Types.h"
#include "map.h"
#include <vector>
class Tetromino
{
    public:
    TetrominoType type;
    Posi center;
    std::vector<Posi> posis;
    Direction dir;
    ColorType color;
    Tetromino(Map & m);
    void secure_space(Map &m);//call after center is decided in proper place, it secure shape in map and color it 
    void unsecure_space(Map &m);//do the reverse as above, it unsecure shape from the map 
};

std::vector<Posi> posisGen(Map &m, Posi center, TetrominoType type, Direction dir);//derive other positions when center, shape, and direction are dicided
#endif