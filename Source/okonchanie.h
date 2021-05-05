#ifndef okonchanie_h
#define okonchanie_h

#include <EV3_Motor.h>
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "EV3_Thread.h"
#include <sstream>
#include <set>
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"
#include <functional>

using namespace ev3_c_api;
using namespace std;

void* okonchanie(void *lpvoid) {
    while (!isBrickButtonPressed(E_BTN_ESC));
    StopMotorAll();
    wait(500);
    exit(0);
}

#endif