#pragma once
#include <unordered_set>
#include <string>
class Map{
    public:
    Map(int bgcolor = 47);
    ~Map();
    struct Point{
        int x;
        int y;
        Point left() const{return {x - 1, y};}
        Point right() const{return {x + 1, y};}
        Point up() const{return {x, y - 1};}
        Point down() const{return {x, y + 1};}
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
        friend std::ostream &operator<<(std::ostream &os, const Point &p);
    };
    bool validPoint(const Point& p);
    typedef struct Point Size;
    inline const Size& getSize(){return this->size;}
    void setPoint(const Point& p, std::string str, int backgroundColor, int fontColor);
    void clearAll();
    void clearCol(int col);
    void clearRow(int row);
    void clearPoint(const Point& p);
    Point topLeft(){return Point{0, 0};}
    Point topRight(){return Point{size.x - 1, 0};}
    Point botLeft(){return Point{0 , size.y - 1};}
    Point botRight(){return Point{size.x - 1 , size.y - 1};}
    Point center(){return Point{size.x /2 , size.y / 2};}
    const std::unordered_set<Point, Point::Hash> & getFreePoints(){return freePoints;}
    const std::unordered_set<Point, Point::Hash> & getUsedPoints(){return usedPoints;}

    private:
    Size size;
    std::unordered_set<Point, Point::Hash> freePoints;
    std::unordered_set<Point, Point::Hash> usedPoints;
    void set(const Point& p, std::string str, int backgroundColor, int fontColor = -1);
    int bgcolor;
};