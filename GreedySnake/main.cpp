#include "map.h"
#include "snakebody.h"
#include "Types.h"
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <pthread.h>

using namespace std;

Posi TargetPos; 
StateType State;
Direction * dir;
Map m(-1,-1);
SnakeBody sb(m,5);


int scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ECHO);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);

    in = getchar();

    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}

void MoveForward(Map & m, SnakeBody & sb)
{
    switch(m.GetType(m.GetPosi(sb.body.front(), sb.dir)))
    {
        case Space:
        m.SpaceSet(sb.body.back(),Space);
        sb.body.pop_back();
        m.SpaceSet(sb.body.front(),Snakebody);
        sb.body.push_front(m.GetPosi(sb.body.front(), sb.dir));
        m.SpaceSet(sb.body.front(),Snakehead);
        break;
        case Target:
        m.SpaceSet(sb.body.front(),Snakebody);
        sb.body.push_front(m.GetPosi(sb.body.front(), sb.dir));
        m.SpaceSet(sb.body.front(),Snakehead);
        TargetPos = m.SpaceGen();
        break;
        default:

        break;
    }
}

bool operator!=(Posi l, Posi r)
{
    if(l.x == r.x && l.y == r.y)
        return false;
    else
        return true;
}

void * KeyHandle(void * arg)
{   
    while(State == Work)
    {
        char c = scanKeyboard();
        auto it = sb.body.begin();
        it++;
        switch(c)
        {
            case 'w':
            if(m.GetPosi(sb.body.front(),Up)  !=  *it)
                *dir = Up;
            break;
            case 'a':
            if(m.GetPosi(sb.body.front(),Left)  !=  *it)
                *dir = Left;
            break;
            case 'd':
            if(m.GetPosi(sb.body.front(),Right)  !=  *it)
                *dir = Right;
            break;
            case 's':
            if(m.GetPosi(sb.body.front(),Down)  !=  *it)
                *dir = Down;
            break;
            case 'q':
            State = End;
            break;
            case 'r':
            State = Reset;
            break;
            default:
            *dir = Up;
            break;
        }
    }
    pthread_exit(NULL);
}

int main()
{
    cout<<"\033[?25l";//hide curser
    reset:
    m = Map(-1,-1);
    sb = SnakeBody(m, 5);
    TargetPos = m.SpaceGen();
    m.SpaceSet(TargetPos,Target);
    m.reflash();
    dir = & sb.dir;
    pthread_t tid;
    pthread_create(&tid, NULL, KeyHandle, NULL);
    State = Work;
    while(State == Work)
    {
        MoveForward(m, sb);
        m.SpaceSet(TargetPos,Target);
        m.reflash();
        usleep(200000);
    }
    pthread_join(tid,NULL);
    if(State == Reset)
        goto reset;
    cout<<"\033[0m"<<endl;//reset
    cout<<"\033[?25h";//unhide curser
    cout<<"\033["<< m.limits.y<<";"<<0<<"H"<<endl<<"End of Game"<<endl;
}