#ifndef MAP
#define MAP
#include "Types.h"
#include <vector>


MapLimit GetMapLimit(void);


#define VALIDPOSI(Position) ((Position).x != -1 && (Position).y != -1)

class Map
{
    private:
    public:
    std::vector<std::vector<SpaceType>> space;
    std::vector<std::vector<ColorType>> colormap;
    
    MapLimit limits;
    Map(int x=0, int y=0);
    void reflash();
    Posi GetPosi(Posi pos, Direction dir);//Dont gerentee to get valid posi, need the macro to validate
    SpaceType GetType(Posi pos);//require Posi to be valid
    ColorType GetColor(Posi pos);
    SpaceType SpaceSet(Posi pos, SpaceType st);//requir posi to be valid set the position to st and return the old space type
    ColorType ColorSet(Posi pos, ColorType ct);
};


#endif

