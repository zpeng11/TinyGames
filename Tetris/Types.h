#ifndef TYPES_H
#define TYPES_H

typedef enum SpaceType{
    Target,
    Space,
    Shadow
 }SpaceType;


typedef enum Direction{
    Left=0,
    Right=1,
    Up=2,
    Down=3
 }Direction;

typedef struct Posi
{
    int x;
    int y;
}Posi;

#define  MapLimit Posi

#endif