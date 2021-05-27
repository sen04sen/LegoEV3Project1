/*!
\file
\brief Файл содержит модуль turn
*/

#ifndef turn_h
#define turn_h

/*!
    \defgroup turn Поворот
    \brief Модуль, содержащий функцию поворота на месте
    \todo Дописать разгон
*/

///@{

void turn(int sp, int dt, int tp) {
    stopBC();
    if (tp >= 0) {
        if (tp < 2 || tp == 4) {
            SpeedMotor(E_Port_B, -1 * (sp));
            SpeedMotor(E_Port_C, -1 * (sp));
        }
        else {
            SpeedMotor(E_Port_B, sp);
            SpeedMotor(E_Port_C, sp);
        }

        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 140);
        if (tp == 4) {
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 50);
            while (s3() > bluck);
        }
        else if (tp == 5) {
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 50);
            while (s2() > bluck);
        }
        else if (tp < 2) {
            while (s3() > black);
        }
        else {
            while (s2() > black);
        }
        st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
    }
    else {
        if (tp == -1) {
            SpeedMotor(E_Port_B, -1 * (sp));
            SpeedMotor(E_Port_C, -1 * (sp));
        }
        else {
            SpeedMotor(E_Port_B, sp);
            SpeedMotor(E_Port_C, sp);
        }
        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(dt));
    }
    stopBC();
}

void t1w(int sp, int dt, int tp) {
    if (tp >= 0) {
        if (tp < 2) {
            stopC();
            SpeedMotor(E_Port_B, -1 * (sp));
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(dt - 140));
            while (s3() > black);
            st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(d1w));
        }
        else {
            stopB();
            SpeedMotor(E_Port_C, sp);
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(dt - 140));
            while (s2() > black);
            st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(d1w));
        }
    }
    else {
        if (tp == -1) {
            stopC();
            SpeedMotor(E_Port_B, -1 * (sp));
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(dt));
        }
        else {
            stopB();
            SpeedMotor(E_Port_C, sp);
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(dt));
        }
    }
    stopBC();
}

///}@

#endif