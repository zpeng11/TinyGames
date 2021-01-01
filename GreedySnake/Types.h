#ifndef TYPES

#define TYPES
typedef enum SpaceType{
    Snakebody=1,
    Snakehead,
    Target,
    Space
 }SpaceType;

typedef enum StateType{
    Work=1,
    End
 }StateType;


typedef struct Posi
{
    int x;
    int y;
}Posi;


typedef enum Direction{
    Left=0,
    Right=1,
    Up=2,
    Down=3
 }Direction;

#define  MapLimit Posi

#endif