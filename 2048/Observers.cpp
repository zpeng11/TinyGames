#include "Observers.h"
#include <sys/ioctl.h> //ioctl()
#include "assert.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

static Limit GetCLILimit(void)
{
    Limit limits;
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);
    limits.xLimit = ws.ws_col;
    limits.yLimit = ws.ws_row;
    return limits;
}

string operator*(string s, int n)
{
    string res = "";
    for (int i = 0; i < n; i++)
        res += s;
    return res;
}

static ColorType Num2Col(NumType n)
{
    if ((int)n <= 0)
        return White;
    else if ((int)n <= 2)
        return Cyan;
    else if ((int)n <= 8)
        return Magenta;
    else if ((int)n <= 32)
        return Blue;
    else if ((int)n <= 128)
        return Yellow;
    else if ((int)n <= 512)
        return Green;
    else
        return Red;
}

InfoBoardObserver::InfoBoardObserver()
{
    Limit l = GetCLILimit();
    if (l.xLimit < 35 || l.yLimit < 12)
    {
        assert(false && "Window size is not capable");
    }
}

void InfoBoardObserver::UpdateObserver()
{
    assert(GetMap() != nullptr);
    std::shared_ptr<Map> m = GetMap();
    const std::vector<std::vector<NumType>> &nums = m->GetNums();

    cout << "\033[?25l"; //hide curser
    for (int i = 0; i < 12; i++)
    {
        cout << "\033[" << i + 1 << ";" << 25 << 'H';
        for (int j = 0; j < 10; j++)
        {
            cout << "\033[0;30;47m"
                 << " "; //Background color
        }
    }
    cout << "\033[" << 1 << ";" << 25 << 'H';
    cout << "\033[0;30;47m"
         << "Last Move:";

    cout << "\033[" << 2 << ";" << 27 << 'H';
    string s;
    switch (m->GetDir())
    {
    case Up:
        s = "Up";
        break;
    case Down:
        s = "Down";
        break;
    case Left:
        s = "Left";
        break;
    default:
        s = "Right";
        break;
    }
    cout << "\033[0;31;47m"
         << s;

    cout << "\033[" << 4 << ";" << 26 << 'H';
    cout << "\033[0;30;47m"
         << "Status:";

    cout << "\033[" << 5 << ";" << 25 << 'H';
    if (m->GetState() == RunState)
    {
        cout << "\033[0;31;47m"
             << m->GetMoves() << " Move";
    }
    else if (m->GetState() == WinState)
    {
        cout << "\033[0;31;47m"
             << "You Won";
        cout << "\033[" << 6 << ";" << 25 << 'H';
        cout << "\033[0;31;47m"
             << "\'e\' or \'q\'";
    }
    else
    {
        cout << "\033[0;31;47m"
             << "You Lost";
        cout << "\033[" << 6 << ";" << 25 << 'H';
        cout << "\033[0;31;47m"
             << "\'e\'or\'q\'";
    }
    cout << "\033[" << 7 << ";" << 27 << 'H';
    cout << "\033[0;30;47m"
         << "Max:";

    cout << "\033[" << 8 << ";" << 27 << 'H';
    int max = 0;
    for(int i =0;i<4;i++)
    {
        for_each(nums[i].begin(), nums[i].end(),[&max](NumType n){max = (int)n>max?n:max;});
    }
    cout << "\033[0;31;47m"
         << max;

    cout << "\033[" << 10 << ";" << 25 << 'H';
    cout << "\033[0;30;47m"
         << "Used Time";

    cout << "\033[" << 11 << ";" << 27 << 'H';
    int t = time(NULL)-m->GetTime();
    string min = to_string(t/60);
    min = min.length()>=2?min:"0"+min;
    string sec = to_string(t%60);
    sec = sec.length()>=2?sec:"0"+sec;
    cout << "\033[0;31;47m"
         << min<<":"<<sec;

    cout << "\033[0m" << endl; //reset
    cout << "\033[?25h";       //unhide curser
}

MapDispObserver::MapDispObserver()
{
    Limit l = GetCLILimit();
    if (l.xLimit < 30 || l.yLimit < 12)
    {
        assert(false && "Window size is not capable");
    }
}

void MapDispObserver::UpdateObserver()
{
    assert(GetMap() != nullptr);
    std::shared_ptr<Map> m = GetMap();
    const std::vector<std::vector<NumType>> &nums = m->GetNums();

    cout << "\033[?25l"; //hide curser
    for (int i = 0; i < 12; i++)
    {
        cout << "\033[" << i + 1 << ";" << 1 << 'H';
        for (int j = 0; j < 24; j++)
        {
            cout << "\033[0;34;47m"
                 << " "; //Background color
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ColorType col = Num2Col(nums[i][j]);
            cout << "\033[" << i * 3 + 1 << ";" << j * 6 + 1 << 'H';
            cout << "\033[0;34;" << col << "m"
                 << "      "; //six spaces

            cout << "\033[" << i * 3 + 2 << ";" << j * 6 + 1 << 'H';
            string s = to_string((int)nums[i][j]);
            int n = 5 - s.length();
            string spaces = string(" ") * n;
            cout << "\033[0;30;" << col << "m"
                 << " " << s << spaces; //six spaces in total

            cout << "\033[" << i * 3 + 3 << ";" << j * 6 + 1 << 'H';
            cout << "\033[0;34;" << col << "m"
                 << "      "; //six spaces
        }
    }

    cout << "\033[0m" << endl; //reset
    cout << "\033[?25h";       //unhide curser
}