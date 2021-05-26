/*!
\file
\brief Файл содержит модуль motors
*/

#ifndef motors_h
#define motors_h

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <set>
#include <functional>

#include "EV3_Motor.h"
#include "EV3_Thread.h"
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"

#include "utils.h"
#include "speed.h"
#include "constants.h"

using namespace ev3_c_api;
using namespace std;

/*!
    \defgroup motors Моторы
    \brief Модуль, содержащий Функции по работе с моторами
*/

///@{

void moveA(int uy) {
    static double stadegd = GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium);
    double dist;
    if (uy == 1) {
        dist = stadegd - 80;
    } else if (uy == 0) {
        dist = stadegd - 45;
    } else {
        dist = stadegd - 120;
    }
    double st = dist - GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium);
    if (st >= 0) {
        SpeedMotor(E_Port_A, 20);
        while (GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium) < dist);
    } else {
        SpeedMotor(E_Port_A, -20);
        while (GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium) > dist);
    }

    SpeedMotor(E_Port_A, 0);
}

void stopB() {
    StopMotor(E_Port_B, 1);
}

void stopC() {
    StopMotor(E_Port_C, 1);
}

void stopD() {
    StopMotor(E_Port_D, 1);
}

void stopBC() {
    StopMotor(E_Port_BC, 1);
}

void stopA() {
    StopMotor(E_Port_A, 1);
}

void goB(int sp) {
    SpeedMotor(E_Port_B, -1 * (sp));
}

void goC(int sp) {
    SpeedMotor(E_Port_C, sp);
}

void goD(int sp) {
    SpeedMotor(E_Port_D, sp);
}

void goA(int sp) {
    SpeedMotor(E_Port_A, sp);
}


void goBC(int sp, int uy = 0) {
    if (uy == 0) {
        SpeedMotor(E_Port_B, -1 * (sp));
        SpeedMotor(E_Port_C, sp);
    } else if (uy == 1) {
        SpeedMotor(E_Port_B, -1 * (sp));
        SpeedMotor(E_Port_C, -1 * sp);
    } else {
        SpeedMotor(E_Port_B, sp);
        SpeedMotor(E_Port_C, sp);
    }
}

void moveONEMOTOR(E_Motor_Port motor, Speed p, int dist) {
    int home = abs(GetMotor_RotationAngle(motor, E_MotorType_Medium));

    Speed_compiled speed = Speed_compiled(p, dist);

    double st = GetMotor_RotationAngle(motor, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(motor, E_MotorType_Medium) - st) < dist) {

        int encoders = abs(GetMotor_RotationAngle(motor, E_MotorType_Medium));

        int nowSpeed = speed(encoders);

        SpeedMotor(motor, nowSpeed);
    }
}

void moveBNEW(Speed p, int dist) { moveONEMOTOR(E_Port_B, p, dist); }

void moveB(int sp, int dist, bool stop = 1) {
    if (sp * dist > 0) {
        moveBNEW(ONEMOTOR, abs(dist));
    } else {
        SpeedMotor(E_Port_B, -1 * (sp));
        double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < dist);
    }
    if (stop)
        stopBC();
}

void moveCNEW(Speed p, int dist) { moveONEMOTOR(E_Port_C, p, dist); }

void moveC(int sp, int dist, bool stop = 1) {
    if (sp * dist > 0) {
        moveCNEW(ONEMOTOR, abs(dist));
    } else {
        SpeedMotor(E_Port_C, sp);
        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dist);
    }
    if (stop)
        stopBC();
}

void moveD(int sp, int dist) {
    dist *= -1;
    static double stadegd = GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium);
    dist = (double) dist + stadegd;
    double st = dist - GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium);
    if (st >= 0) {
        SpeedMotor(E_Port_D, sp);
        while (GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium) < dist);
    } else {
        SpeedMotor(E_Port_D, -sp);
        while (GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium) > dist);
    }
    goD(0);
}

void moveA(int sp, int dist) {
    static double stadegd = GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium);
    dist = (double)dist + stadegd;
    double st = dist - GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium);
    if (st >= 0) {
        SpeedMotor(E_Port_A, sp);
        while (GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium) < dist);
    }
    else {
        SpeedMotor(E_Port_A, -sp);
        while (GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium) > dist);
    }
    goA(0);
}

/*!
    \brief Основная функция проезда вперед с разгоном и торможением
    \param p Настройка скорости (класс Speed)
    \param dist Расстояние
    \param stop Тормозить ли моторы в конце (по умолчанию true)

    Идея в том, что и движение вперед и линия и поворот могли работать из одного и того же объекто класс Speed
    \todo воплотить ее (полсе вторника)
*/
void moveBCNEW(Speed p, int dist) {
    int home = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) -
                GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2;

    Speed_compiled speed = Speed_compiled(p, dist);

    bool stop = 0; // флаг завершения
    while (!stop) {

        int encoders = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) -
                        GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2 - home;

        if (encoders >= dist) stop = 1;

        int nowSpeed = speed(encoders);

        SpeedMotor(E_Port_B, -1 * nowSpeed);
        SpeedMotor(E_Port_C, nowSpeed);
    }
}

/*!
    \brief Обертка для функции проезда вперед
    \param s Скорость (на данный момент параметр фиктивный и ни на что не влияеет)
    \param dist Расстояние
    \param stop Тормозить ли моторы в конце (по умолчанию true)
    \todo Убрать это костыль (после вторника)
    Вперед едет с ускорением, назад без него
*/
void moveBC(int s, int dist, bool stop = 1) {
    if (dist > 0 && s > 0) {
        moveBCNEW(MOVEBC, dist);
    } else {
        if (dist < 0) {
            dist *= -1;
            s *= -1;
        }
        SpeedMotor(E_Port_B, -1 * s);
        SpeedMotor(E_Port_C, s);
        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(dist));
    }
    if (stop)
        stopBC();
}


void moveBTime(int sp, int time) {
    SpeedMotor_Time(E_Port_B, sp, time);
    wait(time);
    stopB();
}

void moveCTime(int sp, int time) {
    SpeedMotor_Time(E_Port_C, sp, time);
    wait(time);
    stopC();
}

void moveDTime(int sp, int time) {
    SpeedMotor_Time(E_Port_D, sp, time);
    wait(time);
    stopD();
}

void moveBCTime(int sp, int time) {
    SpeedMotor_Time(E_Port_BC, sp, time);
    wait(time);
    stopBC();
}

///}@

#endif