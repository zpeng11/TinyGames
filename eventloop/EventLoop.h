#pragma once

#include <sys/poll.h>
#include <signal.h>


class EventLoop{
    private:
    pollfd fds[3];
    void(*timerCallback)();
    void(*keyboardCallback)(unsigned char);
    void(*signalCallback)(int);
    public:
    EventLoop();
    ~EventLoop();
    void timer(int ms, void(*callback)());
    void keyboard(void(*callback)(unsigned char));
    void signal(const sigset_t *mask, void(*callback)(int));
    void run();
};