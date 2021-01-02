#include "Types.h"
#include "tetromino.h"
#include "map.h"
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <pthread.h>
#include <algorithm>
using namespace std;

Map m(15,20);
Tetromino ttm(m);
StateType state;

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


bool move(Direction dir);
bool rotate(void);
bool validate();

bool move(Direction dir)
{
    assert(dir != Up && "Invalid move dir");
    ttm.unsecure_space(m);
    Posi n_center = m.GetPosi(ttm.center,dir);
    if(!VALIDPOSI(n_center)|| m.GetType(n_center) == Target)
    {
        ttm.secure_space(m);
        return false;
    }
    vector<Posi> n_posis = posisGen(m,n_center,ttm.type,ttm.dir);
    for(int i = 0 ; i < 3 ; i++)
    {
        if(!VALIDPOSI(n_posis[i]) || m.GetType(n_posis[i]) == Target)
        {
            ttm.secure_space(m);
            return false;
        }
    }
    ttm.center = n_center;
    ttm.secure_space(m);
    return true;
}

bool rotate(void)
{
    ttm.unsecure_space(m);
    Direction n_dir = static_cast<Direction>((ttm.dir+1)%4);
    vector<Posi> n_posis = posisGen(m,ttm.center,ttm.type,n_dir);
    for(int i = 0 ; i < 3 ; i++)
    {
        if(!VALIDPOSI(n_posis[i]) || m.GetType(n_posis[i]) == Target)
        {
            ttm.secure_space(m);
            return false;
        }
    }
    ttm.dir = n_dir;
    ttm.secure_space(m);
    return true;
}

void * KeyHandle(void * arg)
{   
    while(state == WorkState)
    {
        char c = scanKeyboard();
        switch(c)
        {
            case 'a':
            move(Left);
            break;
            case 'd':
            move(Right);
            break;
            case 's':
            move(Down);
            break;
            case 'q':
            state = EndState;
            break;
            case 'r':
            state = ResetState;
            break;
            default:
            rotate();
            break;
        }
        m.reflash();
    }
    pthread_exit(NULL);
}

void check_clear_line(void)
{
    for(unsigned i = 0  ; i< m.space.size(); i++)
    {
        if( find(m.space[i].begin(), m.space[i].end(),Space) == m.space[i].end())//no space in the line
        { 
            int j = i;
            for(;0<j;j--)
            {
                m.space[j] = m.space[j-1];
                m.colormap[j] = m.colormap[j-1];
            }
            m.space[j] = vector<SpaceType>(m.limits.x,Space);
            m.colormap[j] = vector<ColorType>(m.limits.x, Red);
        }
    }
}

int main()
{
    cout<<"\033[?25l";//hide curser
    reset:
    state = WorkState;
    m = Map(15,20);
    pthread_t tid;
    pthread_create(&tid, NULL, KeyHandle, NULL);
    land:
    check_clear_line();
    ttm = Tetromino(m);
    m.reflash();
    while(state == WorkState)
    {
    usleep(500000);
    bool res = move(Down);
    m.reflash();
    if(res == false)
        goto land;  
    }
    pthread_join(tid,NULL);
    if(state == ResetState)
        goto reset;
    cout<<"\033[0m"<<endl;//reset
    cout<<"\033[?25h";//unhide curser
    cout<<"\033["<< m.limits.y<<";"<<0<<"H"<<endl<<"End of Game"<<endl;
}