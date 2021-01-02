#include "tetromino.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
using namespace std;

Tetromino::Tetromino(Map & m):posis(3)
{
    this->center.x = m.limits.x>>1;
    this->center.y = 1;
    srand(time(NULL));
    this->dir = static_cast<Direction> (rand()%4);
    this->type = static_cast<TetrominoType>(rand()%7);
    this->color = static_cast<ColorType>(rand()%3);
    secure_space(m);
}

std::vector<Posi> posisGen(Map &m, Posi center, TetrominoType type, Direction dir)//derive other positions when center, shape, and direction are dicided
{
    std::vector<Posi> posis(3);
    switch(type)
    {
        case Itype:
            switch(dir)
            {
                case Up:
                case Down:
                posis[0] = m.GetPosi(center,Up);
                posis[1] = m.GetPosi(center,Down);
                posis[2] = m.GetPosi(posis[1],Down);
                break;
                case Left:
                case Right:
                posis[0] = m.GetPosi(center,Left);
                posis[1] = m.GetPosi(center,Right);
                posis[2] = m.GetPosi(posis[1],Right);
                break;
            }
        //cout<<"Itype"<<endl;
        break;
        case Jtype:
            switch(dir)
            {
                case Up:
                posis[0] = m.GetPosi(center,Up);
                posis[1] = m.GetPosi(center,Down);
                posis[2] = m.GetPosi(posis[1],Left);
                break;
                case Down:
                posis[0] = m.GetPosi(center,Down);
                posis[1] = m.GetPosi(center,Up);
                posis[2] = m.GetPosi(posis[1],Right);
                break;
                case Left:
                posis[0] = m.GetPosi(center,Left);
                posis[1] = m.GetPosi(center,Right);
                posis[2] = m.GetPosi(posis[1],Down);
                break;
                case Right:
                posis[0] = m.GetPosi(center,Right);
                posis[1] = m.GetPosi(center,Left);
                posis[2] = m.GetPosi(posis[1],Up);
                break;
            }
        //cout<<"Jtype"<<endl;
        break;
        case Ltype:
            switch(dir)
            {
                case Up:
                posis[0] = m.GetPosi(center,Up);
                posis[1] = m.GetPosi(center,Down);
                posis[2] = m.GetPosi(posis[1],Right);
                break;
                case Down:
                posis[0] = m.GetPosi(center,Down);
                posis[1] = m.GetPosi(center,Up);
                posis[2] = m.GetPosi(posis[1],Left);
                break;
                case Left:
                posis[0] = m.GetPosi(center,Left);
                posis[1] = m.GetPosi(center,Right);
                posis[2] = m.GetPosi(posis[1],Up);
                break;
                case Right:
                posis[0] = m.GetPosi(center,Right);
                posis[1] = m.GetPosi(center,Left);
                posis[2] = m.GetPosi(posis[1],Down);
                break;
            }
        //cout<<"Ltype"<<endl;
        break; 
        case Otype:
            switch(dir)
            {
                default:
                posis[0] = m.GetPosi(center,Right);
                posis[1] = m.GetPosi(center,Down);
                posis[2] = m.GetPosi(posis[1],Right);
                break;
            }
        //cout<<"Otype"<<endl;
        break;
        case Stype:
            switch(dir)
            {
                case Up:
                case Down:
                posis[0] = m.GetPosi(center,Up);
                posis[1] = m.GetPosi(center,Right);
                posis[2] = m.GetPosi(posis[1],Down);
                break;
                case Left:
                case Right:
                posis[0] = m.GetPosi(center,Right);
                posis[1] = m.GetPosi(center,Down);
                posis[2] = m.GetPosi(posis[1],Left);
                break;
            }
        //cout<<"Stype"<<endl;
        break;
        case Ztype:
            switch(dir)
            {
                case Up:
                case Down:
                posis[0] = m.GetPosi(center,Down);
                posis[1] = m.GetPosi(center,Right);
                posis[2] = m.GetPosi(posis[1],Up);
                break;
                case Left:
                case Right:
                posis[0] = m.GetPosi(center,Left);
                posis[1] = m.GetPosi(center,Down);
                posis[2] = m.GetPosi(posis[1],Right);
                break;
            }
        //cout<<"Ztype"<<endl;
        break;
        case Ttype:
            switch(dir)
            {
                case Up:
                posis[0] = m.GetPosi(center,Up);
                posis[1] = m.GetPosi(center,Left);
                posis[2] = m.GetPosi(center,Right);
                break;
                case Down:
                posis[0] = m.GetPosi(center,Down);
                posis[1] = m.GetPosi(center,Right);
                posis[2] = m.GetPosi(center,Left);
                break;
                case Left:
                posis[0] = m.GetPosi(center,Left);
                posis[1] = m.GetPosi(center,Up);
                posis[2] = m.GetPosi(center,Down);
                break;
                case Right:
                posis[0] = m.GetPosi(center,Right);
                posis[1] = m.GetPosi(center,Up);
                posis[2] = m.GetPosi(center,Down);
                break;
            }
        //cout<<"Ttype"<<endl;
        break;
    }
    return posis;
}

void Tetromino::secure_space(Map &m)
{
    this->posis = posisGen(m,this->center,this->type,this->dir);
    assert(m.GetType(this->center) == Space && "The space is occupied");
    m.SpaceSet(this->center,Target);
    m.ColorSet(this->center,this->color);
    for(int i = 0 ; i<3; i++)
    {
        if(VALIDPOSI(this->posis[i]))
        {
            assert(m.GetType(this->posis[i]) == Space && "The space is occupied");
            m.SpaceSet(this->posis[i], Target);
            m.ColorSet(this->posis[i],this->color);
        }
    }
}

void Tetromino::unsecure_space(Map &m)
{
    m.SpaceSet(this->center, Space);
    for(int i = 0 ; i < 3 ; i++)
    {
        if(VALIDPOSI(this->posis[i]))
        {
            m.SpaceSet(this->posis[i], Space);
        }
    }
}

