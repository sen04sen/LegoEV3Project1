/*!
\file
\brief Файл содержит модуль sensors
*/

#ifndef sensors_h
#define sensors_h

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <set>
#include <map>

#include "EV3_Motor.h"
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include "EV3_Thread.h"
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"
#include "EV3_Sensor_Color.h"

#include "field.h"
#include "speed.h"

using namespace ev3_c_api;
using namespace std;

/*!
    \defgroup sensors Датчики
    \brief Модуль, содержащий функци и классы для работы с датчиками
*/

///@{

int s1() {
    return GetReflect(E_Port_1);
}

int s2() {
    return GetReflect(E_Port_2);
}

int s3() {
    return GetReflect(E_Port_3);
}

int s4() {
    return GetReflect(E_Port_4);
}

/*!
	\brief Цвет в формате RGB
*/

struct ColorRGB {
    int r;  ///< Красная составляющая
    int g;  ///< Зеленая составляющая
    int b;  ///< Голубая составляющая

    /*!
    \param _r Красная составляющая
    \param _g Зеленая составляющая
    \param _b Голубая составляющая
    \return Возвращает ColorRGB
    */
    ColorRGB(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {}
};

/*!
    \brief Цвет по моделе HSV
    Подробнее https://ru.wikipedia.org/wiki/HSV_(цветовая_модель)
*/
struct ColorHSV {
    int h;  ///< Цвет
    int s;  ///< Насыщенность
    int v;  ///< Яркость

    /*!
    \param col ColorRGB для перевода
    \return Возвращает ColorHSV
    */
    ColorHSV(ColorRGB col) {
        v = max(max(col.r, col.g), col.b);
        if (col.b <= col.r && col.b <= col.g) h = (col.r - col.b) / ((col.r - col.b) + (col.g - col.b)) * 100;
        if (col.r <= col.b && col.r <= col.g) h = (col.g - col.r) / ((col.g - col.r) + (col.b - col.r)) * 100;
        if (col.g <= col.r && col.g <= col.b) h = (col.r - col.g) / ((col.r - col.g) + (col.b - col.g)) * 100;
        int temp = min(min(col.r, col.g), col.b);
        if (v == 0) s = 0;
        else s = (v - temp) / v;
    }
};

/*!
    \brief Возвращает цвет с датчика в RGB
    \param port Порт
    \return ColorRGB
*/
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
        case 1:
            a = GetData_UART(E_Port_1, E_UART_Type_Color, 4);
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

/*!
    \brief Возвращает цвет с датчика в HSV
    \param port Порт
    \return ColorHSV
*/
ColorHSV getHSV(int port) { return ColorHSV(getRGB(port)); }

int gclr(int uy) {
    if (uy == 4) return GetColor(E_Port_4);
    else return GetColor(E_Port_3);
}

Color read_marker() {
    E_Color now = GetColor(E_Port_4);
    if (now == E_Color_Yellow || now == E_Color_Red || now == E_Color_Brown) return YELLOW;
    else if (now == E_Color_Blue) return BLUE;
    else if (now == E_Color_Green) return GREEN;
    else return NONE;
}

DoubleMarker read_home() {
    int dist = 360;
    read_marker();
    int home = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) -
                GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2;

    Speed_compiled compiled = Speed_compiled(READ, 350);
    map<Color, int> left;
    left[BLUE] = 0;
    left[YELLOW] = 0;
    left[GREEN] = 0;
    left[NONE] = 0;
    map<Color, int> right;
    right[BLUE] = 0;
    right[YELLOW] = 0;
    right[GREEN] = 0;
    right[NONE] = 0;
    int encoders = 0;
    while (encoders < dist) {
        encoders = abs((GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) -
                        GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2 - home);

        if (120 < encoders && encoders < 220) {
            left[read_marker()]++;
        } else if (240 < encoders && encoders < 360) {
            right[read_marker()]++;
        }

        int nowSpeed = compiled(encoders);
        SpeedMotor(E_Port_B, -nowSpeed);
        SpeedMotor(E_Port_C, nowSpeed);
    }

    stopBC();

    DoubleMarker ans;

    left[NONE] /= 10;
    int how = 0;
    for (map<Color, int>::iterator it = left.begin(); it != left.end(); ++it)
        if (it->second > how) {
            ans.left = it->first;
            how = it->second;
        }

    right[NONE] /= 10;
    how = 0;
    for (map<Color, int>::iterator it = right.begin(); it != right.end(); ++it)
        if (it->second > how) {
            ans.right = it->first;
            how = it->second;
        }
    write(1, 1, ans.left);
    write(20, 1, ans.right);
    return ans;
}

///}@

#endif