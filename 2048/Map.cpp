#include "Map.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>//sleep()
#include "Observers.h"
using namespace std;


Map::Map()
{
    Reset();
}

void Map::Reset()
{
    srand(time(nullptr)); 
    mNums = vector<vector<NumType>>(4, vector<NumType>(4, n0));
    mDirection = Up;
    this->GenNew();
    mState = RunState;
    mMoves = 0;
    mStartTime = time(NULL);
    this->UpdateObserver();
}

void Map::SetDir(Direction d)
{
    mDirection = d;
    mMoves +=1;
    this->Move();
    bool win = this->CheckWin();
    bool HaveSpace = this->GenNew();
    StateType &state = mState;
    if (win == true)
        state = WinState;
    else if (!HaveSpace)
        state = LossState;
    this->UpdateObserver();
}

void Map::Move()
{
    switch (mDirection)
    {
    case Up:
        for (int j = 0; j < 4; j++)
        {
            bool flag = true;
            for (int i = 3; i > 0; i--)
            {
                if (mNums[i][j] == mNums[i - 1][j] && flag && mNums[i - 1][j] != n0)
                {
                    mNums[i - 1][j] = (NumType)((int)mNums[i - 1][j] << 1);
                    mNums[i][j] = n0;
                    for (int k = i + 1; k < 4; k++)
                        swap(mNums[k][j], mNums[k - 1][j]);
                    flag = false;
                }
                else if (mNums[i - 1][j] == n0)
                {
                    swap(mNums[i - 1][j], mNums[i][j]);
                    for (int k = i + 1; k < 4; k++)
                        swap(mNums[k][j], mNums[k - 1][j]);
                }
            }
        }
        break;
    case Down:
        for (int j = 0; j < 4; j++)
        {
            bool flag = true;
            for (int i = 0; i < 3; i++)
            {
                if (mNums[i][j] == mNums[i + 1][j] && flag && mNums[i + 1][j] != n0)
                {
                    mNums[i + 1][j] = (NumType)((int)mNums[i + 1][j] << 1);
                    mNums[i][j] = n0;
                    for (int k = i - 1; k > -1; k--)
                        swap(mNums[k][j], mNums[k + 1][j]);
                    flag = false;
                }
                else if (mNums[i + 1][j] == n0)
                {
                    swap(mNums[i + 1][j], mNums[i][j]);
                    for (int k = i - 1; k > -1; k--)
                        swap(mNums[k][j], mNums[k + 1][j]);
                }
            }
        }
        break;
    case Left:
        for (int i = 0; i < 4; i++)
        {
            bool flag = true;
            for (int j = 3; j > 0; j--)
            {
                if (mNums[i][j] == mNums[i][j - 1] && flag && mNums[i][j - 1] != n0)
                {
                    mNums[i][j - 1] = (NumType)((int)mNums[i][j - 1] << 1);
                    mNums[i][j] = n0;
                    for (int k = j + 1; k < 4; k++)
                        swap(mNums[i][k], mNums[i][k - 1]);
                    flag = false;
                }
                else if (mNums[i][j - 1] == n0)
                {
                    swap(mNums[i][j - 1], mNums[i][j]);
                    for (int k = j + 1; k < 4; k++)
                        swap(mNums[i][k], mNums[i][k - 1]);
                }
            }
        }
        break;
    default: //Right
        for (int i = 0; i < 4; i++)
        {
            bool flag = true;
            for (int j = 0; j < 3; j++)
            {
                if (mNums[i][j] == mNums[i][j + 1] && flag && mNums[i][j + 1] != n0)
                {
                    mNums[i][j + 1] = (NumType)((int)mNums[i][j + 1] << 1);
                    mNums[i][j] = n0;
                    for (int k = j - 1; k > -1; k--)
                        swap(mNums[i][k], mNums[i][k + 1]);
                    flag = false;
                }
                else if (mNums[i][j + 1] == n0)
                {
                    swap(mNums[i][j + 1], mNums[i][j]);
                    for (int k = j - 1; k > -1; k--)
                        swap(mNums[i][k], mNums[i][k + 1]);
                }
            }
        }
        break;
    }
}

bool Map::CheckWin()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((int)mNums[i][j] >= n2048)
                return true;
        }
    }
    return false;
}

bool Map::GenNew()
{
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        sum += count(mNums[i].begin(), mNums[i].end(), n0);
    }
    int ran = sum ? rand() % sum : 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (mNums[i][j] == n0)
            {
                sum -= 1;
                if (sum == ran)
                {
                    mNums[i][j] = n2;
                    return true;
                }
            }
        }
    }
    return false;
}

void Map::AddObserver(MapObserver *observer)
{
    mObservers.push_back(observer);
}

void Map::RemoveObserver(MapObserver *observer)
{
    auto it = find(begin(mObservers), end(mObservers), observer);
    if (it != end(mObservers))
    {
        mObservers.erase(it);
    }
}

void Map::UpdateObserver()
{
    for (auto observer : mObservers)
    {
        observer->UpdateObserver();
    }
}

