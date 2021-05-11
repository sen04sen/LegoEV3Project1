#ifndef sensors_h
#define sensors_h

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <set>

#include "EV3_Motor.h"
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include "EV3_Thread.h"
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"

using namespace ev3_c_api;
using namespace std;

int s2() {
    return GetReflect(E_Port_2);
}

int s3() {
    return GetReflect(E_Port_3);
}


struct ColorRGB {
    int r, g, b;

    ColorRGB(int newR, int newG, int newB) : r(newR), g(newG), b(newB) {}
};


ColorRGB getRGB(int port) {
    const void *a;
    switch (port) {
        case 3:
            a = GetData_UART(E_Port_3, E_UART_Type_Color, 4);
            break;
        case 4:
            a = GetData_UART(E_Port_4, E_UART_Type_Color, 4);
            break;
        case 2:
            a = GetData_UART(E_Port_2, E_UART_Type_Color, 4);
            break;
        default:
            throw Exception("invalid port parameter (need 2-4), you gave " + str(port));
            break;
    }
    unsigned char *d = reinterpret_cast<unsigned char *>(const_cast<void *>(a));
    int r = d[0];
    int g = d[2];
    int b = d[4];
    ColorRGB color = ColorRGB(r, g, b);
    return color;
}

int gclr(int uy) {
    if (uy == 4) {
        return GetColor(E_Port_4);
    } else {
        return GetColor(E_Port_3);
    }
}

#endif