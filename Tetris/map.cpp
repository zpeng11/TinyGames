#include "map.h"
#include <stdlib.h>//srand() rand()
#include <time.h>//time()
#include <unistd.h>//sleep()
#include <sys/ioctl.h>//ioctl()
#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>//assert()



using namespace std;


string operator*(int n, string str)
{
    string ret;
    for(int i =0 ;i<n ;i++)
        ret+=str;
    return ret;
}



MapLimit GetMapLimit(void)
{
    MapLimit limits;
    struct winsize ws;
    ioctl(0,TIOCGWINSZ,&ws ); 
    limits.x = ws.ws_col>>1;
    limits.y = ws.ws_row;
    return limits;
}





Map::Map(int x, int y)
{
    this->limits = GetMapLimit();//get map info
    stringstream ss;
    int y_save = this->limits.y;
    ss<<"Got X:"<<this->limits.x<<" Y:"<<this->limits.y<<endl;
    
    if( x> this->limits.x || y> this->limits.y)//check input
    {
        cout<<"Set x or y failed with x="<<x<<",y="<<y<<" ,thus set to map size"<<endl;
        x = this->limits.x;
        y = this->limits.y;
    }

    if(x < 0 || y < 0 )//when get -1 or lower input
    {
        this->limits.x = this->limits.y = min(this->limits.x, this->limits.y);//we make space a square
        cout<<"Set to x="<<this->limits.x<<" ,y="<<this->limits.y<<endl;
    }
    else if(x != 0 || y != 0  )
    {
        this->limits.x = x;
        this->limits.y= y;
        cout<<"Set to x="<<this->limits.x<<" ,y="<<this->limits.y<<endl;
    }
    
    this->space = vector<vector<SpaceType> >(this->limits.y, vector<SpaceType>(this->limits.x, Space));
    this->colormap = vector<vector<ColorType> >(this->limits.y, vector<ColorType>(this->limits.x, Red));
    ss<<y_save*string("\n");//clear space
    cout<<ss.str()<<endl;
    this->reflash();
}

void Map::reflash()
{
    cout<<"\033[0;34;47m";
    for(int i = 0 ; i< this->limits.y; i++)
    {
        cout<<"\033["<< i+1<<";"<<0<<'H';
        for(int j = 0 ; j< this->limits.x ; j++)
        {
            switch(this->space[i][j])
            {
                case Space:
                cout<<"\033[0;34;47m"<<"  ";//SAME
                break;
                case Target:
                int c;
                switch(this->colormap[i][j])
                {
                    case Red:
                    c = 41;
                    break;
                    case Green:
                    c = 42;
                    break;
                    case Blue:
                    c = 44;
                    break;
                }
                cout<<"\033[0;34;"<<c<<"m"<<"  ";//colored target
                break;
            }
        }
    }
    cout<<"\033[0m";//reset
    cout.flush();
}


Posi Map::GetPosi(Posi pos, Direction dir)
{
    if(!VALIDPOSI(pos))
        return {-1,-1};
    switch(dir)
    {
        case Up:
        pos.y -=1;
        break;
        case Down:
        pos.y +=1;
        break;
        case Left:
        pos.x -=1;
        break;
        case Right:
        pos.x +=1;
        break;
    }
    if(pos.x<0)
        pos.x = -1;
    if(pos.x>= this->limits.x)
        pos.x = -1;
    if(pos.y<0)
        pos.y = -1;
    if(pos.y>= this->limits.y)
        pos.y = -1;
        
    return pos;
}

SpaceType Map::GetType(Posi pos)
{
    assert(pos.x>= 0 && pos.x< this->limits.x && pos.y>= 0 && pos.y <this->limits.y && "Invalid position");
    return this->space[pos.y][pos.x];
}

ColorType Map::GetColor(Posi pos)
{
    assert(pos.x>= 0 && pos.x< this->limits.x && pos.y>= 0 && pos.y <this->limits.y && "Invalid position");
    return this->colormap[pos.y][pos.x];
}

SpaceType  Map::SpaceSet(Posi pos, SpaceType st)
{
    assert(pos.x>= 0 && pos.x< this->limits.x && pos.y>= 0 && pos.y <this->limits.y && "Invalid position");
    SpaceType oldst = this->space[pos.y][pos.x];
    this->space[pos.y][pos.x] = st;
    return oldst;
}
ColorType Map::ColorSet(Posi pos, ColorType ct)
{
    assert(pos.x>= 0 && pos.x< this->limits.x && pos.y>= 0 && pos.y <this->limits.y && "Invalid position");
    ColorType oldct = this->colormap[pos.y][pos.x];
    this->colormap[pos.y][pos.x] = ct;
    return oldct;
}

/*
int main()
{
    Map m(10,20);
}
*/