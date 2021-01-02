#ifndef TYPES_H
#define TYPES_H

typedef enum SpaceType{
    Target,
    Space
 }SpaceType;

typedef enum ColorType{
    Red=0,
    Green,
    Blue
 }ColorType;

typedef enum TetrominoType{
    Stype=0,
    Ztype,
    Ltype,
    Jtype,
    Itype,
    Otype,
    Ttype
}TetrominoType;

typedef enum Direction{
    Left=0,
    Up,
    Right,
    Down
 }Direction;

typedef struct Posi
{
    int x;
    int y;
}Posi;

typedef enum StateType{
    WorkState=1,
    ResetState,
    EndState
 }StateType;


#define  MapLimit Posi

#endif