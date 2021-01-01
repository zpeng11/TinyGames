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
    ss<<y_save*string("\n");//clear space
    cout<<ss.str()<<endl;
    this->reflash();
}

void Map::reflash()
{
    cout<<"\033[?25l";//hide curser
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
                cout<<"\033[0;34;41m"<<"  ";//red target
                break;
                case Snakebody:
                cout<<"\033[0;34;44m"<<"  ";//blue body
                break;
                case Snakehead:
                cout<<"\033[0;34;42m"<<"HH";//green head
                break;
            }
        }
    }
    cout<<"\033[0m";//reset
    cout<<"\033[?25h";//unhide curser
}

Posi Map::GetPosi(Posi pos, Direction dir)
{
    assert(pos.x>= 0 && pos.x< this->limits.x && pos.y>= 0 && pos.y <this->limits.y && "Invalid position");
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
        pos.x = this->limits.x -1;
    if(pos.x>= this->limits.x)
        pos.x = 0;
    if(pos.y<0)
        pos.y = this->limits.y -1;
    if(pos.y>= this->limits.y)
        pos.y = 0;
        
    return pos;
}

SpaceType Map::GetType(Posi pos)
{
    assert(pos.x>= 0 && pos.x< this->limits.x && pos.y>= 0 && pos.y <this->limits.y && "Invalid position");
    return this->space[pos.y][pos.x];
}

Posi Map::SpaceGen()
{
    Posi pos;
    do{
        srand(time(NULL));
        int temp = rand()%(this->limits.x*this->limits.y);
        pos.x = temp%this->limits.x;
        pos.y = temp/this->limits.x;
    }while(this->GetType(pos) != Space);
    return pos;
}

SpaceType  Map::SpaceSet(Posi pos, SpaceType st)
{
    assert(pos.x>= 0 && pos.x< this->limits.x && pos.y>= 0 && pos.y <this->limits.y && "Invalid position");
    SpaceType oldst = this->space[pos.y][pos.x];
    this->space[pos.y][pos.x] = st;
    return oldst;
}

/*
int main()
{
    Map m(-1,-1);
    Posi find = m.GetPosi({0,0},Down);
    m.SpaceSet(find, Target);
     find = m.GetPosi({0,0},Left);
     m.SpaceSet(find, Snakehead);
     find = m.GetPosi({0,0},Up);
     m.SpaceSet(find, Snakebody);
     find = m.GetPosi({0,0},Right);
    m.SpaceSet(find, Target);
    m.reflash();
   
    while(1){
    find = m.SpaceGen();
    m.space[find.y][find.x] = Target;
    m.reflash();
    sleep(1);
    }
}
*/