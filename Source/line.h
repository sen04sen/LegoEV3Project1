/*!
\file
\brief Файл содержит модуль line
*/

#ifndef line_h
#define line_h

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <stdio.h>
#include <stdlib.h>

#include "EV3_Motor.h"
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include "EV3_Thread.h"
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"

#include "speed.h"
#include "constants.h"
#include "utils.h"
#include "sensors.h"

using namespace ev3_c_api;
using namespace std;

/*!
    \defgroup line Линия
    \brief Модуль, содержащий функцию линии
*/

///@{

/*!
    \brief Основная функция моей линии
    \param p Настройка скорости (класс Speed)
    \param dist Предполагаемое расстояние до конца линии
    \param type Тип линии (тут лучше шарит создатель)
    \warning Просто надо помнить, что эта линия не предполагает движения назaд
*/

void lineNEW(Speed p, int dist, int type, bool uping = true, bool downing = true,
             int end_preview = standart_line_preview_looking) {
    // Просто надо помнить, что эта линия не предполагает движения назaд
    // Какая-то типизация
    /*if (type == 4) {
        getRGB(3);
        EV3_Sleep(50);
    }
    if (type == 4) getRGB(3);

    int home = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) -
                GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2;

    Speed_compiled speed = Speed_compiled(p, dist, uping, downing);

    int way = dist + home;

    vector<int> errors = vector<int>(lineArrayLen, 0);

    bool stop = 0; // флаг завершения
    for (int count = 0; !stop; count++) {

        int encoders = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) -
                        GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2 - home;

        if (encoders < way - end_preview) {
            if (type == 0 || type == 6 || type == 8) {
                if (encoders >= way) stop = 1;
            } else {
                if (type == 1 && s2() < black && s3() < black) {
                    stop = 1;
                } else if (type == 2 && s3() < black) {
                    stop = 1;
                } else if (type == 3 && s2() < black) {
                    stop = 1;
                }
                else if (type == 4) {
                    ColorRGB color = getRGB(3);
                    if (color.r - color.g > 70) stop = 1;
                } else if (type == 5 && s2() > bluck) {
                    stop = 1;
                } else if (type == 7 && s3() < black) {
                    stop = 1;
                } else if (type == 9 && s3() > bluck) stop = 1;
            }
        }

        int error = 0;
        switch (type) {
            case 10:
                error = (double)(s3() - s2() - deltaSensors);
                break;
            case 5:
                error = (double) (s3() - bley) * 3 / 3;
                break;
            case 6:
                error = (double) (s3() - bley) * 3 / 3;
                break;
            case 4:
                error = (double) (grey - s2()) * 2 / 3;
                break;
            case 7:
                error = (double) (s2() - grey) * 3 / 3;
                break;
            case 8:
                error = (double) (s2() - grey) * 3 / 3;
                break;
            case 9:
                error = (double) (bley - s2()) * 3 / 3;
                break;
            default:
                error = (double) (s3() - s2() - deltaSensors);
                break;
        } // подсчет ошибки

        int nowSpeed = speed(encoders - home);

        double kP = p.p * ((double) nowSpeed / (double) p.max_sp);
        double kD = p.d * ((double) nowSpeed / (double) p.max_sp);

        SpeedMotor(E_Port_B, -1 * (nowSpeed - error * kP - (error - errors[count % lineArrayLen]) * kD));
        SpeedMotor(E_Port_C, nowSpeed + error * kP + (error - errors[count % lineArrayLen]) * kD);

        errors[count % lineArrayLen] = error; // для d составляющей
    }
    if (type == 4) s3(); // Какая-то типизация
}

/*!
    \brief Обертка для функции линии
    \param p Настройка скорости (класс Speed)
    \param dist Предполагаемое расстояние до конца линии
    \param type Тип линии (тут лучше шарит создатель)
    \todo Убрать это костыль (после вторника)
*/
void line(int speed, int dist, int type, bool uping = true, bool downing = true,
          int end_preview = standart_line_preview_looking) {
    lineNEW(ZERO, dist, type, uping, downing, end_preview);
}

///}@

#endif