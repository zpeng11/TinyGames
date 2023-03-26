#include <iostream>

#include "EventLoop.h"
using namespace std;
void kbevent(unsigned char c){
    cout<<c<<endl;
}

int main(){
    EventLoop el;
    el.keyboard(kbevent);
    el.run();
}