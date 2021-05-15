#include "Map.h"
#include "Observers.h"
#include <iostream>
#include <termios.h>

using namespace std;

void funcAtExit()
{
    cout<<"\033[0m"<<endl;//reset
    cout<<"\033[?25h";//unhide curser
    cout<<"\033["<< 13<<";"<<1<<"H"<<endl<<"End of Game"<<endl;
}

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

int main()
{
    atexit(funcAtExit);
    shared_ptr<Map> m(new Map());
    {
        InfoBoardObserver info;
        MapDispObserver disp;
        info.SetMap(m);
        disp.SetMap(m);
        m->UpdateObserver();
        while(1)
        {
            int ctl = scanKeyboard();//scan a key press
            if(ctl == 'q')//quit
            {
                break;
            }
            else if(ctl =='e')//Reset
                m->Reset();

            else if(m->GetState() == RunState)
            {
                if(ctl == 'w')
                    m->SetDir(Up);
                else if(ctl == 's')
                    m->SetDir(Down);
                else if(ctl == 'a')
                    m->SetDir(Left);
                else 
                    m->SetDir(Right);
            }
            
        }
    }
}