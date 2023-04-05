#include "map.h"
#include <list>
#include <exception>

class Snake{
    public:
    struct SnakeError : public std::exception {
        const char * what () const throw () {
            return "SnakeError";
        }
    };
    struct SnakeCollisionError : public SnakeError {
        const char * what () const throw () {
            return "Snake Collided with itself";
        }
    };
    /*
    struct SnakeWallError : public SnakeError {
        const char * what () const throw () {
            return "Snake Collided with wall";
        }
    };
    */
    Snake(Map * m);
    ~Snake();
    Map::Point head(){return this->body.front();}
    Map::Point tail(){return this->body.back();}
    void step();
    enum Direction{
        Up, Down, Left, Right
    };
    void turn(Snake::Direction dir);
    void swallow() {this-> swallowed = true;}
    private:
    bool swallowed; 
    std::list<Map::Point> body;
    Direction dir;
    Direction backDir;
    Map * m;
};