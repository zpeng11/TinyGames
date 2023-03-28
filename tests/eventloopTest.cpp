#include <iostream>

#include "EventLoop.h"
using namespace std;
void kbevent(unsigned char c){
    cout<<c<<endl;
}

void sigevent(int sig){
    cout<<"Got signal"<<endl;
}

void tmrevent(){
    cout<<"Timer Up"<<endl;
}

int main(){
    EventLoop el;
    el.keyboard(kbevent);
    sigset_t sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGINT);
    el.signal(&sigs, sigevent);
    el.timer(500, tmrevent);
    el.run();
}