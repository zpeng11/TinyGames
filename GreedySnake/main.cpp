#include <iostream>

#include "EventLoop.h"
#include "map.h"
#include "snake.h"
#include <unistd.h>
#include <time.h>

Map * m;
EventLoop * loop;
Snake*  snake;
Map::Point food = {-1, -1};

void kbevent(unsigned char c){
    if(c == 'q'){
        loop->end();
    }
    else if(c == 'a'){
        snake->turn(Snake::Left);
    }
    else if(c == 'd'){
        snake->turn(Snake::Right);
    }
    else if(c == 'w'){
        snake->turn(Snake::Up);
    }
    else if(c == 's'){
        snake->turn(Snake::Down);
    }
}

void sigevent(int sig){
    loop->end();
}

void foodGen();
void tmrevent(){
    try{
        snake->step();
        if(snake->head() == food){
            snake->swallow();
            foodGen();
        }
    }
    catch(Snake::SnakeCollisionError& err){
        loop->end();
    }
}

void foodGen(){
    const auto freeSet = m->getFreePoints();
    int foodIdx = rand()% freeSet.size();
    int i = 0;
    for(const auto p : freeSet){
        if(i == foodIdx){
            food = p;
            break;
        }
        else{
            i++;
        }
    }
    m->setPoint(food, "QQ", 45, 41);
}

int main(int argc, char *argv[])
{
    int intervalInMs = argc > 1? std::stoi(argv[1]) : 999;
    std::cout<<argc<<"  "<<intervalInMs<<std::endl;
    loop = new EventLoop();
    m = new Map();
    snake = new Snake(m);
    srand(time(NULL));
    foodGen();
    

    loop->keyboard(kbevent);
    sigset_t sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGINT);
    loop->signal(&sigs, sigevent);
    loop->timer(intervalInMs, tmrevent);
    loop->run();
    delete snake;
    delete loop;
    delete m;
}