#ifndef wait_h
#define wait_h

#include "EV3_Thread.h"

using namespace ev3_c_api;
using namespace std;

void wait(int time) {
    // время сна в миллисекундах
    EV3_Sleep(time);
}

#endif