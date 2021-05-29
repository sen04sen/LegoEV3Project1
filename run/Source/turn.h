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
        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        int home = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) +
            GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2;

        Speed_compiled compiled = Speed_compiled(TURN, abs(dt));

        int encoders = 0;
        while (encoders < abs(dt - 60)) {
            encoders = abs((GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) +
                GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2 - home);

            int nowSpeed = compiled(encoders);

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
        } else if (tp == 5) {
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 50);
            while (s2() > bluck);
        } else if (tp < 2) {
            while (s3() > black);
        } else {
            while (s2() > black);
        }
        st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
    } else {
        int home = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) +
            GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2;

        Speed_compiled compiled = Speed_compiled(TURN, abs(dt));

        int encoders = 0;
        //write(10, 10, 1);
        //wait(5000);
        while (encoders < abs(dt)) {
            encoders = abs((GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) +
                GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2 - home);

            int nowSpeed = compiled(encoders);

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

void t1w_old(int sp, int dt, int tp) {
    if (tp >= 0) {
        if (tp < 2) {
            stopC();
            SpeedMotor(E_Port_B, -1 * (sp));
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            dt -= 180;
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(dt));
            while (s3() > black);
            st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(d1w));
        } else {
            stopB();
            SpeedMotor(E_Port_C, sp);
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            dt -= 180;
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(dt));
            while (s2() > black);
            st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(d1w));
        }
    } else {
        if (tp == -1) {
            stopC();
            SpeedMotor(E_Port_B, -1 * (sp));
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(dt));
        } else {
            stopB();
            SpeedMotor(E_Port_C, sp);
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(dt));
        }
    }
    stopBC();
}

void t1w(int sp, int dt, int tp) {
    if (tp >= 0) {
        if (tp < 2) {
            stopC();
            //SpeedMotor(E_Port_B, -1 * (sp));
            dt -= 180;
            Speed_compiled compiled = Speed_compiled(ONEMOTOR, abs(dt));
            int encoders = 0;
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
            //SpeedMotor(E_Port_C, sp);
            dt -= 180;
            Speed_compiled compiled = Speed_compiled(ONEMOTOR, abs(dt));
            int encoders = 0;
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
            int encoders = 0;
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (encoders < abs(dt)) {
                encoders = abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st);
                SpeedMotor(E_Port_B, -compiled(encoders));
            }
        } else {
            stopB();
            //SpeedMotor(E_Port_C, sp);
            Speed_compiled compiled = Speed_compiled(ONEMOTOR, abs(dt));
            int encoders = 0;
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