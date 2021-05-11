#ifndef turn_h
#define turn_h

void turn(int sp, int dt, int tp) {
    if (tp >= 0) {
        if (tp < 2 || tp == 4) {
            SpeedMotor(E_Port_B, -1 * (sp));
            SpeedMotor(E_Port_C, -1 * (sp));
        } else {
            SpeedMotor(E_Port_B, sp);
            SpeedMotor(E_Port_C, sp);
        }

        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 80);
        if (tp == 4)
            while (s3() > bluck);
        else if (tp == 5)
            while (s2() > bluck);
        else if (tp < 2) {
            while (s3() > black);
        } else {
            while (s2() > black);
        }
        st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
        stopBC();
    } else {
        if (tp == -1) {
            SpeedMotor(E_Port_B, -1 * (sp));
            SpeedMotor(E_Port_C, -1 * (sp));
        } else {
            SpeedMotor(E_Port_B, sp);
            SpeedMotor(E_Port_C, sp);
        }
        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt);
        stopBC();
    }
}

#endif