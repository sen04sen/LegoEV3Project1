#ifndef control_h
#define control_h

#include "motors.h"

volatile int how_a = 100;

void* control(void* _) {
    while (true) {
        if (abs(how_a - GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium)) > 15) {
            int now = 0.3 * (how_a - GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium));
            if (now > 0) {
                if (now < 10) now = 10;
                else if (now > 100) now = 100;
            }
            else {
                if (now > -10) now = -10;
                else if (now < -100) now = -100;
            }
            goA(now);
        }
        else stopA();
    }
}

void waitA() {
    wait(50);
    while (abs(how_a - GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium)) > 15) wait(10);
}

#endif
