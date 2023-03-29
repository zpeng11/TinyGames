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
        Point down(){return {x, y + 1};}
        bool operator==(const Point& otherPos) const
        {
            if (this->x == otherPos.x && this->y == otherPos.y) return true;
            else return false;
        }
        struct Hash {
            size_t operator()(const Point &p) const
            {
                return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y)<<1);
            }
        };
    };
    bool validPoint(const Point& p);
    typedef struct Point Size;
    inline const Size& getSize(){return this->size;}
    void setPoint(const Point& p, std::string str, int backgroundColor, int fontColor);
    void clearAll();
    void clearCol(int col);
    void clearRow(int row);
    void clearPoint(const Point& p);

    private:
    Size size;
    std::unordered_set<Point, Point::Hash> freePoints;
    std::unordered_set<Point, Point::Hash> usedPoints;
};