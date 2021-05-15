#ifndef MAP_H
#define MAP_H

#include <assert.h>
#include <vector>
#include <memory>
#include "Types.h"

class MapObserver;
class Map
{
private:
    std::vector<std::vector<NumType>> mNums;
    std::vector<MapObserver *> mObservers;
    Direction mDirection;
    StateType mState = RunState;
    int mMoves =0;
    int mStartTime;

public:
    Map();
    Map(const Map &) = delete;
    void operator=(const Map &) = delete;
    ~Map() {}

    void Reset();
    StateType GetState(){return mState;}
    void SetState(StateType state){ mState = state;}
    const std::vector<std::vector<NumType>>& GetNums(){return mNums;}
    void SetDir(Direction d);
    Direction GetDir(){return mDirection;}
    void Move();
    bool GenNew();
    bool CheckWin();
    int GetMoves(){return mMoves;}
    int GetTime(){return mStartTime;}

    void AddObserver(MapObserver *observer);
    void RemoveObserver(MapObserver *observer);
    void UpdateObserver();
};

class MapObserver
{
private:
    std::shared_ptr<Map> mMap = nullptr;

public:
    //Constructor
    MapObserver() {}
    MapObserver(const MapObserver &) = delete;
    void operator=(const MapObserver &) = delete;

    //Update to be implement
    virtual void UpdateObserver() = 0;

    //Getter and Setter
    void SetMap(std::shared_ptr<Map> m)
    {
        mMap = m;
        mMap->AddObserver(this);
    }
    std::shared_ptr<Map> GetMap() { return mMap; }

    //Destructor
    ~MapObserver()
    {
        if (mMap != nullptr)
            mMap->RemoveObserver(this);
    }
};

#endif /*MAP_H*/
