#include "snakebody.h"
#include <assert.h>

SnakeBody::SnakeBody( Map &m, int len):body(),dir(Left)
{
    assert(len < m.limits.y && "Too long start");
    body.push_back( m.SpaceGen());
    m.SpaceSet(body.front(), Snakehead);
    for(int i = 0 ; i < len -1; i++)
    {
        body.push_back(m.GetPosi(body.back(), Down));
        m.SpaceSet(body.back(), Snakebody);
    }
}
/*
int main()
{
    Map m (-1,-1);
    SnakeBody sb(m, 5);
    m.SpaceSet(m.SpaceGen(),Target);
    m.reflash();
}*/