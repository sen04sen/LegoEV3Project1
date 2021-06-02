#ifndef control_h
#define control_h

#include "motors.h"

volatile int how_a = 100;
volatile bool work_a = 1;

void* control(void* _) {
    goA(-100);
    wait(500);
    int enc = GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium) - enc) > 2) wait(100);
    stopA();
    wait(500);
    ResetMotor(E_Port_A);
    work_a = 0;
    while (true) {
        wait(100);
        if (GetBrickButtonPressed() == 32) 
            break;
        if (abs(how_a - GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium)) > 2) {
            int now = 0.3 * (how_a - GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium));
            if (now > 0) {
                if (now < 10) now = 10;
                else if (now > 100) now = 100;
            }
            else {
                if (now > -20) now = -20;
                else if (now < -100) now = -100;
            }
            goA(now);
        }
        else stopA();
    }
    wait(1000);
    while (!isBrickButtonPressed(E_BTN_ESC)) {
        StopMotorAll();
        wait(100);
    }
    exit(0);
}

void waitA() {
    wait(50);
    while (abs(how_a - GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium)) > 15 || work_a) wait(10);
}

#endif
