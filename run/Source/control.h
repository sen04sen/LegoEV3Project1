#ifndef control_h
#define control_h

#include "motors.h"
#import "math.h"

volatile int how_a = 100;

void *control(void* _) {
    while (true) {
        goA(max(-100, min(100, 0.03 * (GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium) - how_a))));
        write(10, 10, 0.03 * (GetMotor_RotationAngle(E_Port_A, E_MotorType_Medium) - how_a));
    }
}

#endif
