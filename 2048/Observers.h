#ifndef OBSERVERS_H
#define OBSERVERS_H

#include "Map.h"



class InfoBoardObserver:public MapObserver
{
    public:
    InfoBoardObserver();
    InfoBoardObserver(const InfoBoardObserver &) = delete;
    void operator=(const InfoBoardObserver &) = delete;
    void UpdateObserver()override;
};

class MapDispObserver:public MapObserver
{
    public:
    MapDispObserver();
    MapDispObserver(const MapDispObserver &) = delete;
    void operator=(const MapDispObserver &) = delete;
    void UpdateObserver()override;
};
#endif