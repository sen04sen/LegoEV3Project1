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



void lineNEW(Speed p, int dist, int type) {
    // Просто надо помнить, что эта линия не предполагает движения назaд

    // Какая-то типизация
    if (type == 4) {
        getRGB(3);
        EV3_Sleep(50);
    }
    if (type == 4) getRGB(3);


    int home =
        GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) * -1 + GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    double kUpDist = 0.5 * (p.maxS / p.sEnc), kDownDist = 0.5 * (p.maxS / p.eEnc);

    int upDist, downDist;
    if (p.sEnc > 0) upDist = ((int)(p.sEnc * 2)) + home;
    else upDist = -2147483648;
    if (p.eEnc > 0) downDist = ((int)((dist - p.zEnc - p.eEnc) * 2)) + home;
    else downDist = 2147483647;

    int way = dist * 2 + home, encoders = home, error = 0, errors[lineArrayLen];

    for (int i = 0; i < lineArrayLen; i++) errors[i] = error; // заполнить массив ошибок

    T_TimerId taa = Timer_Start();
    bool stop = 0; // флаг завершения
    int count = 0;
    for (; !stop; count++) {

        encoders = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) * -1 +
            GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);

        int nowSpeed;
        if (encoders > downDist) nowSpeed = p.maxS - (encoders - downDist) * kDownDist;
        else if (encoders < upDist) nowSpeed = (encoders - home) * kUpDist;
        else nowSpeed = p.maxS;
        if (encoders > way - linePreviewLooking * 2 || nowSpeed < p.minS) nowSpeed = p.minS;

        if (type == 0 || type == 6 || type == 8 || type == 9) {
            if (encoders >= way) {
                stop = 1;
            }
        }
        else {
            if (encoders >= way - linePreviewLooking * 2) {
                if (type == 1 && s2() < black && s3() < black) {
                    stop = 1;

                }
                else if (type == 2 && s3() < black) {
                    stop = 1;

                }
                else if (type == 3 && s2() < black) {
                    stop = 1;
                }
                else if (type == 4) {
                    ColorRGB color = getRGB(3);
                    if (color.r - color.g > 70) {
                        stop = 1;

                    }
                }
                else if (type == 5 && s2() > bluck) {
                    stop = 1;

                }
                else if (type == 7 && s3() < black) {
                    stop = 1;

                }
            }
        }

        switch (type) {
        case 2:
            error = (double)(grey - s2());
            break;
        case 3:
            error = (double)(s3() - grey);
            break;
        case 5:
            error = (double)(s3() - bley);
            break;
        case 6:
            error = (double)(s3() - bley);
            break;
        case 4:
            error = (double)(grey - s2());
            break;
        case 7:
            error = (double)(s2() - grey);
            break;
        case 8:
            error = (double)(s2() - grey);
            break;
        case 9:
            error = (double)(bley - s2());
            break;
        default:
            error = (double)(s3() - s2());
            break;
        } // подсчет ошибки
        double kP;
        double kD;
        if (type >= 2 && type <= 8) {
            kP = 0.3 * ((double)nowSpeed / (double)p.maxS);
            kD = 0.3 * ((double)nowSpeed / (double)p.maxS);
        }
        else {
            kP = p.p * ((double)nowSpeed / (double)p.maxS);
            kD = p.d * ((double)nowSpeed / (double)p.maxS);
        }

        SpeedMotor(E_Port_B, -1 * (nowSpeed - error * kP - (error - errors[count % lineArrayLen]) * kD));
        SpeedMotor(E_Port_C, nowSpeed + error * kP + (error - errors[count % lineArrayLen]) * kD);

        errors[count % lineArrayLen] = error; // для d составляющей
    }
    write(1, 1, Timer_Destroy(taa) / count);
    if (type == 4) s3(); // Какая-то типизациям - это переключение с ргб на обычный
}

void line(int speed, int dist, int type) {
    lineNEW(ZERO, dist, type);
}


#endif