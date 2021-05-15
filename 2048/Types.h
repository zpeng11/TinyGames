#ifndef TYPES_H
#define TYPES_H

typedef enum ColorType{
    Black = 40,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
 }ColorType;

typedef enum NumType{
    n0 = 0,
    n2 = 2,
    n4 = 4,
    n8 = 8,
    n16 = 16,
    n32 = 32,
    n64 = 64,
    n128 = 128,
    n256 = 256,
    n512 = 512,
    n1024 = 1024,
    n2048 = 2048
}NumType;

typedef enum Direction{
    Left,
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
    RunState=1,
    WinState,
    LossState,
    EndState
 }StateType;


typedef struct Limit
{
    int xLimit;
    int yLimit;
}Limit;

#endif