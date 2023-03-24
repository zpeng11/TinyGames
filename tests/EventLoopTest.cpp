#include "EventLoop.h"
#include <iostream>
using namespace std;

void tmrcb(){
    cout<<"2000 ms Time Up!"<<endl;
}

void signalcb(int sig){
    cout<<"Got Signal!"<<endl;
}  

void keyboardcb(unsigned char c){
    cout<<"'"<<(char )c<<"'"<<endl;
}

int main(){
    EventLoop loop;
    loop.timer(2000, tmrcb);
    loop.keyboard(keyboardcb);
    sigset_t sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGINT);
    loop.signal(&sigs, signalcb);
    loop.run();
}