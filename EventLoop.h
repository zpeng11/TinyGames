#pragma once

#include <sys/poll.h>
class EventLoop{
    private:
    pollfd fds[3];
    void(*timerCallback)();
    void(*keyboardCallback)(char);
    void(*signalCallback)(int);
    public:
    EventLoop();
    ~EventLoop();
    void timer(int ms, void(*callback)());
    void keyboard(void(*callback)(char));
    void signal(void(*callback)(int));
    void run();
};