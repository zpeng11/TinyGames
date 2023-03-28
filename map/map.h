#pragma once
#include <unordered_set>
#include <string>
class Map{
    public:
    Map();
    ~Map();
    struct Point{
        int x;
        int y;
        Point left(){return {x - 1, y};}
        Point right(){return {x + 1, y};}
        Point up(){return {x, y - 1};}
        Point up(){return {x, y + 1};}
    };
    bool validPoint(const Point& p);
    typedef struct Point Size;
    inline const Size& getSize(){return this->size;}
    void setPoint(const Point& p, std::string str, int backgroundColor, int fontColor);
    void clearAll();
    void clearCol(int col);
    void clearRow(int row);
    void clearPoint(const Point& p);
    struct PointHash {
        size_t operator()(const Point &p){ return std::hash<std::string>()(std::to_string(p.x)+","+std::to_string(p.y));}
    };
    private:
    Size size;
    std::unordered_set<Point, PointHash> freePoints;
    std::unordered_set<Point, PointHash> usedPoints;
};