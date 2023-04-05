#include "snake.h"
#include <algorithm>

#define SNAKE_HEAD_STR "HH"
#define SNAKE_BODY_STR "  "
#define SNAKE_TAIL_STR "TT"
#define SNAKE_BGCOLOR 42
#define SNAKE_FTCOLOR 105
Snake::Snake(Map* m){
    this->m = m;
    this->dir = Snake::Up;
    this->backDir = Snake::Down;
    this->body = std::list<Map::Point>();
    const auto head = m->center();
    this->swallowed = false;
    m->setPoint(head, SNAKE_HEAD_STR, SNAKE_BGCOLOR, SNAKE_FTCOLOR);
    this->body.push_front(head);
    const auto tail = head.down();
    m->setPoint(tail, SNAKE_TAIL_STR, SNAKE_BGCOLOR, SNAKE_FTCOLOR);
    this->body.push_back(tail);
}

void Snake::turn(Snake::Direction dir){
    if(dir != this->backDir){  
        this->dir = dir;
        if(this->dir == Snake::Direction::Up)
            this->backDir = Snake::Direction::Down;
        else if(this->dir == Snake::Direction::Down)
            this->backDir = Snake::Direction::Up;
        else if(this->dir == Snake::Direction::Left)
            this->backDir = Snake::Direction::Right;
        else
            this->backDir = Snake::Direction::Left;
    }
}

void Snake::step(){
    auto curHead = this->head();
    auto nextHead = Map::Point{-1, -1};
    if(this->dir == Snake::Direction::Up)
        nextHead = curHead.up();
    else if(this->dir == Snake::Direction::Down)
        nextHead = curHead.down();
    else if(this->dir == Snake::Direction::Left)
        nextHead = curHead.left();
    else
        nextHead = curHead.right();
    
    auto collideFound = std::find(this->body.begin(), this->body.end(), nextHead);
    if(collideFound != this->body.end()){
        throw SnakeCollisionError();
    }
    if(!m->validPoint(nextHead)){
        //IF making walls, throw error here
        nextHead.x = nextHead.x < 0 ? m->getSize().x - 1 : nextHead.x;
        nextHead.y = nextHead.y < 0 ? m->getSize().y - 1 : nextHead.y;
        nextHead.x = nextHead.x % m->getSize().x;
        nextHead.y = nextHead.y % m->getSize().y;
    }
    m->setPoint(curHead, SNAKE_BODY_STR, SNAKE_BGCOLOR, SNAKE_FTCOLOR);
    m->setPoint(nextHead, SNAKE_HEAD_STR, SNAKE_BGCOLOR, SNAKE_FTCOLOR);
    this->body.push_front(nextHead);
    auto curTail = this->tail();
    if(this->swallowed){
        this->swallowed = false;
    }
    else{
        m->clearPoint(curTail);
        this->body.pop_back();
        m->setPoint(this->tail(), SNAKE_TAIL_STR, SNAKE_BGCOLOR, SNAKE_FTCOLOR);
    }
}

Snake::~Snake(){

}