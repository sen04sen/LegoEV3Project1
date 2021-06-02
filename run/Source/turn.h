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



void turnNEW(int sp, int dst, char dir, int tp) { // 1 - влево, 2 - вправо
    int dr = 1;
    if (dir == 'r')
        dr = -1; // типо костыль для поворота в нужную сторону
    int home = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    int ndist;
    goB(-1 * sp * dr);
    goC(sp * dr);
    if (tp == 0) {
        ndist = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - home);
        while (ndist < dst)
            ndist = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - home);
    }
    else if (tp == 1) {
        ndist = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - home);
        while (ndist < 180)
            ndist = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - home);
        if (dir == 'l') {
            while (s3() < black);
            while (s2() > black);
        }
        else {
            while (s2() < black);
            while (s3() > black);
        }
        int now = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - home);
        ndist = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - home);
        while (ndist - now < dovorot)
            ndist = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - home);
    }
    stopBC();
}

char l = 'l';
char r = 'r';

void turn(int sp, int dt, int tp) {
    if (tp == -2)
        turnNEW(speed, dt, l, 0);
    else if (tp == -1)
        turnNEW(speed, dt, r, 0);
    else if (tp == 0)
        turnNEW(speed, dt, r, 1);
    else if (tp == 3)
        turnNEW(speed, dt, l, 1);
    else {
        stopBC();
        if (tp >= 0) {
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            int32_t home = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) +
                GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2;

            Speed_compiled compiled = Speed_compiled(TURN, abs(dt));

            int32_t encoders = 0;
            while (encoders < abs(dt - 100)) {
                encoders = abs((GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) +
                    GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2 - home);

                int32_t nowSpeed = compiled(encoders);

                if (tp < 2 || tp == 4) {
                    SpeedMotor(E_Port_B, -1 * (nowSpeed));
                    SpeedMotor(E_Port_C, -1 * (nowSpeed));
                }
                else {
                    SpeedMotor(E_Port_B, nowSpeed);
                    SpeedMotor(E_Port_C, nowSpeed);
                }
            }

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
            int32_t home = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) +
                GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2;

            Speed_compiled compiled = Speed_compiled(TURN, abs(dt));

            int32_t encoders = 0;
            while (encoders < abs(dt)) {
                encoders = abs((GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) +
                    GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2 - home);

                int32_t nowSpeed = compiled(encoders);

                if (tp == -1) {
                    SpeedMotor(E_Port_B, -1 * (nowSpeed));
                    SpeedMotor(E_Port_C, -1 * (nowSpeed));
                }
                else {
                    SpeedMotor(E_Port_B, nowSpeed);
                    SpeedMotor(E_Port_C, nowSpeed);
                }
            }
        }
        stopBC();
    }
}

void t1w(int32_t sp, int32_t dt, int32_t tp) {
    if (tp >= 0) {
        if (tp < 2) {
            stopC();
            dt -= 150;
            Speed_compiled compiled = Speed_compiled(ONEMOTOR, abs(dt));
            int32_t encoders = 0;
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (encoders < abs(dt)) {
                encoders = abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st);
                SpeedMotor(E_Port_B, -compiled(encoders));
            }
            while (s3() > black);
            st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(d1w));
        } else {
            stopB();
            dt -= 150;
            Speed_compiled compiled = Speed_compiled(ONEMOTOR, abs(dt));
            int32_t encoders = 0;
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (encoders < abs(dt)) {
                encoders = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st);
                SpeedMotor(E_Port_C, compiled(encoders));
            }
            while (s2() > black);
            st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(d1w));
        }
    } else {
        if (tp == -1) {
            stopC();
            //SpeedMotor(E_Port_B, -1 * (sp));
            Speed_compiled compiled = Speed_compiled(ONEMOTOR, abs(dt));
            int32_t encoders = 0;
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (encoders < abs(dt)) {
                encoders = abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st);
                SpeedMotor(E_Port_B, -compiled(encoders));
            }
        } else {
            stopB();
            //SpeedMotor(E_Port_C, sp);
            Speed_compiled compiled = Speed_compiled(ONEMOTOR, abs(dt));
            int32_t encoders = 0;
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (encoders < abs(dt)) {
                encoders = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st);
                SpeedMotor(E_Port_C, compiled(encoders));
            }
        }
    }
    stopBC();
}

///}@

#endif