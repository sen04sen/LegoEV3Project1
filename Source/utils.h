#ifndef utils_h
#define utils_h

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

using namespace ev3_c_api;
using namespace std;

void wait(int time) {
    // время сна в миллисекундах
    EV3_Sleep(time);
}

template<class T>
string str(T forConvert) {
    ostringstream ss;
    ss << forConvert;
    return ss.str();
}

class Exception {
    // Тут просто класс наших исключений
private:
    std::string m_error;

public:
    Exception(string error) : m_error(error) {}

    Exception(int line) : m_error(str(line)) {}

    const char *what() { return m_error.c_str(); }
};

void* okonchanie(void *lpvoid) {
    while (!isBrickButtonPressed(E_BTN_ESC));
    StopMotorAll();
    wait(500);
    exit(0);
}

void write(int x, int y, int uy) {
    stringstream s;
    s << uy;
    string a;
    s >> a;
    Draw_Text(x, y, E_Font_Normal, 0, &(a[0]));
}

#endif