//
// Created by ПК on 05.05.2021.
//

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

void moveBC(int sp, int dist, bool stop = true) {
    SpeedMotor(E_Port_B, -1 * (sp));
    SpeedMotor(E_Port_C, sp);
    double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dist);
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