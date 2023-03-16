#include "EventLoop.h"
#include "sys/poll.h"
#include "sys/signalfd.h"
#include "sys/signal.h"
#include <sys/termios.h>
#include "unistd.h"
#include "sys/timerfd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TIME_POSI 0 
#define SIGNAL_POSI 1
#define KEYBOARD_POSI 2
EventLoop::EventLoop():timerCallback(nullptr), signalCallback(nullptr), keyboardCallback(nullptr){
    for(int i = 0 ; i < 3; i++){
        fds[i] = {0};
        fds[i].fd = -1;
    }
}

EventLoop::~EventLoop(){
    for(int i = 0 ; i < 3; i++){
        if(fds[i].fd == -1){
            close(fds[i].fd);
        }
    }
}

void EventLoop::timer(int ms, void(*callback)()){
    this->timerCallback = callback;
    struct itimerspec spec;
    memset(&spec, 0, sizeof(spec));
    spec.it_value.tv_sec = ms / 1000;
    spec.it_value.tv_nsec = (ms - (ms / 1000 * 1000)) * 1000;
    spec.it_interval = spec.it_value;
    int tfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
    if(tfd < 0){
        perror("Create: ");
        exit(EXIT_FAILURE);
    }
    if(timerfd_settime(tfd, 0, &spec, nullptr ) < 0){
        perror("Set timerfd: ");
        exit(EXIT_FAILURE);
    }
    this->fds[TIME_POSI].fd = tfd;
    this->fds[TIME_POSI].events |= POLLIN;
}


struct termios orig_termios;
void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}
class KeyboardConio{
    KeyboardConio()
    {
        struct termios new_termios;
        /* take two copies - one for now, one for later */
        tcgetattr(0, &orig_termios);
        memcpy(&new_termios, &orig_termios, sizeof(new_termios));

        /* register cleanup handler, and set the new terminal mode */
        atexit(reset_terminal_mode);
        cfmakeraw(&new_termios);
        tcsetattr(0, TCSANOW, &new_termios);
    }
} keyboardConio;

void EventLoop::keyboard(void(*callback)(char)){
    this->keyboardCallback = callback;
    this->fds[KEYBOARD_POSI].fd = STDIN_FILENO;
    
}