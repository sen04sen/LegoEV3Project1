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

using namespace ev3_c_api;
using namespace std;


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

void goB(int sp) {
    SpeedMotor(E_Port_B, -1 * (sp));
}

void goC(int sp) {
    SpeedMotor(E_Port_C, sp);
}

void goD(int sp) {
    SpeedMotor(E_Port_D, sp);
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

void moveB(int sp, int dist, bool stop = true) {
    SpeedMotor(E_Port_B, -1 * (sp));
    double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < dist);
    if (stop)
        stopB();
}

void moveC(int sp, int dist, bool stop = true) {
    SpeedMotor(E_Port_C, sp);
    double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dist);
    if (stop)
        stopC();
}

void moveD(int sp, int dist) {
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

void moveBC(SpeedProfileName speed, int dist, bool stop = true) {
    Speed p; // Извлечение настроек
    try {
        p = Speed::speeds[speed];
    } catch (...) {
        throw Exception("No speed Profile");
    }

    if (dist > 0) {
        int home = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) +
                   GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);

        double kUpDist = 0.5 * (p.maxS / p.sEnc), kDownDist = 0.5 * (p.maxS / p.eEnc);

        int upDist, downDist;
        if (p.sEnc > 0) upDist = ((int) (p.sEnc * 2)) + home;
        else upDist = -2147483648;
        if (p.eEnc > 0) downDist = ((int) ((dist - p.zEnc - p.eEnc) * 2)) + home;
        else downDist = 2147483647;

        int way = dist * 2 + home, encoders = home;

        bool stop = 0; // флаг завершения
        for (int count = 0; !stop; count++) {

            encoders = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) +
                       GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);

            int nowSpeed;
            if (encoders > downDist) nowSpeed = p.maxS - (encoders - downDist) * kDownDist;
            else if (encoders < upDist) nowSpeed = (encoders - home) * kUpDist;
            else nowSpeed = p.maxS;
            if (encoders > way || nowSpeed < p.minS) nowSpeed = p.minS;

            if (encoders >= way) stop = 1;

            SpeedMotor(E_Port_B, -1 * nowSpeed);
            SpeedMotor(E_Port_C, nowSpeed);
        }
    } else if (dist < 0) {
        SpeedMotor(E_Port_B, -1 * p.maxS);
        SpeedMotor(E_Port_C, p.maxS);
        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dist);
    }

    if (stop) stopBC(); // финальное торможение
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

#endif