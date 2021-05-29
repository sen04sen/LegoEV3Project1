/*!
\file
\brief Файл содержит модуль line
*/

#ifndef line_h
#define line_h

#include "speed.h"
#include "constants.h"
#include "utils.h"
#include "sensors.h"

using namespace ev3_c_api;
using namespace std;

/*!
    \defgroup line Линия
    \brief Модуль, содержащий функцию линии
*/

///@{

/*!
    \brief Основная функция моей линии
    \param p Настройка скорости (класс Speed)
    \param dist Предполагаемое расстояние до конца линии
    \param type Тип линии (тут лучше шарит создатель)
    \warning Просто надо помнить, что эта линия не предполагает движения назaд
*/

void line(Speed p, int dist, int type, bool uping = true, bool downing = true, double end_preview = standart_line_preview_looking) {
    // Просто надо помнить, что эта линия не предполагает движения назaд
    // Какая-то типизация
    if (type == 4) {
        getRGB(3);
        EV3_Sleep(50);
    }
    if (type == 4) getRGB(3);

    int home = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) -
                GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2;

    Speed_compiled speed_control = Speed_compiled(p, dist, uping, downing);

    vector<int> errors = vector<int>(lineArrayLen, 0);

    bool stop = 0; // флаг завершения
    for (int count = 0; !stop; count++) {

        int encoders = (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) -
                        GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium)) / 2 - home;

        if (encoders > (double) dist * 0.6) {
            if (type == 0 || type == 6 || type == 8 || type == 10) {
                if (encoders >= dist) stop = 1;
            } else {
                if (type == 1 && s2() < black && s3() < black) {
                    stop = 1;
                } else if (type == 2 && s3() < black) {
                    stop = 1;
                } else if (type == 3 && s2() < black) {
                    stop = 1;
                }
                else if (type == 4) {
                    ColorRGB color = getRGB(3);
                    if (color.r - color.g > 70) stop = 1;
                } else if (type == 5 && s2() > bluck) {
                    stop = 1;
                } else if (type == 7 && s3() < black) {
                    stop = 1;
                } else if (type == 9 && s3() > bluck) stop = 1;
            }
        }

        int error = 0;
        switch (type) {
            case 10:
                error = (double)(grey - s3()) * 3 / 3;
                break;
            case 5:
                error = s3() - bley;
                break;
            case 6:
                error = (double) (bley - s2()) * 3 / 3;
                break;
            case 4:
                error = (double) (grey - s2()) * 4 / 3;
                break;
            case 7:
                error = s2() - grey;
                break;
            case 8:
                error = s2() - grey;
                break;
            case 9:
                error = bley - s2();
                break;
            default:
                error = s3() - s2() - deltaSensors;
                break;
        } // подсчет ошибки

        int nowSpeed = speed_control(encoders);

        double kP = p.get_p() * ((double) nowSpeed / (double) p.max_sp);
        double kD = p.get_d() * ((double) nowSpeed / (double) p.max_sp);

        SpeedMotor(E_Port_B, max((short)-100, short(-1 * (nowSpeed - error * kP - (error - errors[count % lineArrayLen]) * kD))));
        SpeedMotor(E_Port_C, min((short)100, short(nowSpeed + error * kP + (error - errors[count % lineArrayLen]) * kD)));

        errors[count % lineArrayLen] = error; // для d составляющей
    }
    if (type == 4) s3(); // Какая-то типизация
}

/*!
    \brief Обертка для функции линии
    \param p Настройка скорости (класс Speed)
    \param dist Предполагаемое расстояние до конца линии
    \param type Тип линии (тут лучше шарит создатель)
*/
void line(int _, int dist, int type, bool uping = true, bool downing = true, double end_preview = standart_line_preview_looking) {
    line(ZERO, dist, type, uping, downing, end_preview);
}

///}@

#endif