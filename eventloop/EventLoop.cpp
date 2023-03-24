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
#include <assert.h>


#include <iostream>
using namespace std;

#define TIME_POSI 0 
#define SIGNAL_POSI 1
#define KEYBOARD_POSI 2

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


EventLoop::EventLoop():timerCallback(nullptr),keyboardCallback(nullptr), signalCallback(nullptr){
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
    assert(this->timerCallback == nullptr && "Already have timer handler");
    this->timerCallback = callback;
    struct itimerspec spec;
    memset(&spec, 0, sizeof(spec));
    spec.it_value.tv_sec = ms / 1000;
    spec.it_value.tv_nsec = (ms - (ms / 1000 * 1000)) * 1000;
    spec.it_interval = spec.it_value;
    int tfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
    if(tfd < 0)
        handle_error("Create: ");
    if(timerfd_settime(tfd, 0, &spec, nullptr ) < 0)
        handle_error("Set timerfd: ");
    this->fds[TIME_POSI].fd = tfd;
    this->fds[TIME_POSI].events |= POLLIN;
}


struct termios orig_termios;
void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}
class ProcessConio{
    public:
    ProcessConio(){
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    new_termios.c_lflag &= (~ICANON);
    new_termios.c_lflag &= (~ECHO);
    new_termios.c_cc[VTIME] = 0;
    new_termios.c_cc[VMIN] = 1;
    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    tcsetattr(0, TCSANOW, &new_termios);
    }
} processConio;

void EventLoop::keyboard(void(*callback)(unsigned char)){
    assert(this->keyboardCallback == nullptr && "Already have keybaord handler");
    this->keyboardCallback = callback;
    int flag = fcntl(STDIN_FILENO, F_GETFL, 0);
    if(flag < 0)
        handle_error("Get STDIN fd:");
    if(fcntl(STDIN_FILENO, F_SETFL, (flag|O_NONBLOCK)))
        handle_error("Set STDIN NONBLOCK error:");
    this->fds[KEYBOARD_POSI].fd = STDIN_FILENO;
    this->fds[KEYBOARD_POSI].events = POLLIN;
}

void EventLoop::signal(const sigset_t *mask, void(*callback)(int)){
    assert(this->signalCallback == nullptr && "Already have signal handler");
    this->signalCallback = callback;
    if (sigprocmask(SIG_BLOCK, mask, NULL) == -1)
        handle_error("Mask signal: ");
    int sfd = signalfd(-1, mask, SFD_NONBLOCK);
    if(sfd < 0)
        handle_error("Signal fd create:");
    this->fds[SIGNAL_POSI].fd = sfd;
    this->fds[SIGNAL_POSI].events = POLLIN;
}

void EventLoop::run(){
    if(this->fds[TIME_POSI].fd == -1 && this->fds[SIGNAL_POSI].fd == -1 && this->fds[KEYBOARD_POSI].fd == -1)
        return;
    for(int i = 0 ; i < 3; i++)
        this->fds[i].revents = 0;
    while(1){
        int ret = poll(this->fds, 3, -1);
        if(ret < 0)
            handle_error("Poll :");
        if(this->fds[KEYBOARD_POSI].fd != -1 && this->fds[KEYBOARD_POSI].revents != 0){
            if(~this->fds[KEYBOARD_POSI].revents & POLLIN)
                handle_error("Keyboard fd unexpected event");
            unsigned char c;
            if ((read(this->fds[KEYBOARD_POSI].fd, &c, sizeof(c))) < 0) {
                handle_error("Read Keyboard:");
            } else if(this->keyboardCallback != nullptr){
                this->keyboardCallback(c);
            }
        }
        if(this->fds[SIGNAL_POSI].fd != -1 && this->fds[SIGNAL_POSI].revents != 0){
            if(~this->fds[SIGNAL_POSI].revents & POLLIN)
                handle_error("Signal fd unexpected event");
            struct signalfd_siginfo fdsi;
            if(read(this->fds[SIGNAL_POSI].fd, &fdsi, sizeof(fdsi)) != sizeof(fdsi)){
                handle_error("Signalfd read:");
            }
            if(this->signalCallback != nullptr){
                this->signalCallback(fdsi.ssi_signo);
            }
        }
        if(this->fds[TIME_POSI].fd != -1 && this->fds[TIME_POSI].revents != 0){
            if(~this->fds[TIME_POSI].revents & POLLIN){
                handle_error("Timer fd unexpected event");
            }
                
            uint64_t ret_time;
            if(read(this->fds[TIME_POSI].fd, &ret_time, sizeof(uint64_t)) != sizeof(uint64_t)){
                handle_error("Timerfd read:");
            }
            if(this->timerCallback != nullptr){
                this->timerCallback();
            }
        }
    }
}