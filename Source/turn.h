/*!
\file
\brief Файл содержит модуль turn
*/

#ifndef turn_h
#define turn_h

#include "speed.h"
#include "motors.h"

/*!
    \defgroup turn Поворот
    \brief Модуль, содержащий функцию поворота на месте
    \todo Дописать разгон
*/

///@{

void turn(int sp, int dt, int tp) {
    if (tp >= 0) {
        if (tp == 4) {
            SpeedMotor(E_Port_B, -1 * (sp));
            SpeedMotor(E_Port_C, -1 * (sp));
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 100);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 50);
            while (s3() > bluck);
            st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
        }
        else if (tp == 5) {
            SpeedMotor(E_Port_B, sp);
            SpeedMotor(E_Port_C, sp);
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 100);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 50);
            while (s2() > bluck);
            st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
        }
        else if (tp < 2) {
            SpeedMotor(E_Port_B, -1 * (sp));
            SpeedMotor(E_Port_C, -1 * (sp));
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 100);
            while (s3() > black);
            st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
        } else {
            SpeedMotor(E_Port_B, sp);
            SpeedMotor(E_Port_C, sp);
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 100);
            while (s2() > black);
            st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
        }
    } else {
        Speed p = TURN;
        if (tp == -1) {
            int home = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium));

            double kUpDist = (((double)p.maxS - (double)p.minS) / (double)p.sEnc);
            double kDownDist = (((double)p.maxS - (double)p.minS) / (double)p.eEnc);

            int upDist, downDist;
            if (p.sEnc > 0) upDist = ((int)(p.sEnc)) + home;
            else upDist = -2147483648;
            if (p.eEnc > 0) downDist = ((int)((dt - p.zEnc - p.eEnc))) + home;
            else downDist = 2147483647;

            int way = dt + home;

            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt) {

                int encoders = abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium));

                int nowSpeed;
                if (encoders > way - linePreviewLooking) nowSpeed = p.minS;
                else if (encoders > downDist) nowSpeed = (double)p.maxS - ((double)encoders - (double)downDist) * kDownDist;
                else if (encoders < upDist) nowSpeed = ((double)encoders - (double)home) * kUpDist + p.minS;
                else nowSpeed = p.maxS;
                if (nowSpeed < p.minS) nowSpeed = p.minS;

                SpeedMotor(E_Port_B, -1 * nowSpeed);
                SpeedMotor(E_Port_C, -1 * nowSpeed);
            }
        } else {
            int home = abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium));

            double kUpDist = (((double)p.maxS - (double)p.minS) / (double)p.sEnc);
            double kDownDist = (((double)p.maxS - (double)p.minS) / (double)p.eEnc);

            int upDist, downDist;
            if (p.sEnc > 0) upDist = ((int)(p.sEnc)) + home;
            else upDist = -2147483648;
            if (p.eEnc > 0) downDist = ((int)((dt - p.zEnc - p.eEnc))) + home;
            else downDist = 2147483647;

            int way = dt + home;

            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < dt) {

                int encoders = abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium));

                int nowSpeed;
                if (encoders > way - linePreviewLooking) nowSpeed = p.minS;
                else if (encoders > downDist) nowSpeed = (double)p.maxS - ((double)encoders - (double)downDist) * kDownDist;
                else if (encoders < upDist) nowSpeed = ((double)encoders - (double)home) * kUpDist + p.minS;
                else nowSpeed = p.maxS;
                if (nowSpeed < p.minS) nowSpeed = p.minS;

                SpeedMotor(E_Port_B, nowSpeed);
                SpeedMotor(E_Port_C, nowSpeed);
            }
        }
    }
    stopBC();
}

///}@

#endif