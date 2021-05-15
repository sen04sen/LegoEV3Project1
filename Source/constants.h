/*!
\file
\brief Файл содержит константы
*/


#ifndef constants_h
#define constants_h

#include "speed.h"

const int maxv = 500;         ///< \todo Описать
const int ce = 21 * 2;        ///< \todo Описать
const int ver = 22;           ///< \todo Описать
const int black = 25;         ///< \todo Описать
const int bluck = 25;         ///< \todo Описать
const int bley = 40;          ///< \todo Описать
const double Pr = 0.3;        ///< \todo Описать
const int grey = 35;          ///< \todo Описать
const double dws = 130;       ///< \todo Описать
const double dsl = 50;        ///< \todo Описать
const int d90 = 270;          ///< \todo Описать
const int d180 = 540;         ///< \todo Описать
const int dovorot = 45;       ///< \todo Описать
const int turn1wheel = 550;   ///< \todo Описать
const int ndir = 0;           ///< \todo Описать
const int speed = 23;         ///< \todo Описать
const int speedD = 40;        ///< \todo Описать

const int lineArrayLen = 20;                      ///< Длинна массива задержек для линии
const int linePreviewLooking = 50;                ///< За это расстояние до конца линии робот начинает чекать датчиком конец
Speed ZERO = Speed(23, 20, 0.3, 0.3, 1, 1, 1);    ///< Настройка линии (функция lineNew) класс Speed

#define pb push_back

vector<int> grad; ///< Длины основных элементов поля

void buildDegreesConstants() { /// Заполнение вектора grad
    grad.pb(850);
    grad.pb(390);
    grad.pb(470);
    grad.pb(820);
    grad.pb(760);
    grad.pb(580);
    grad.pb(580);
    grad.pb(270);
    grad.pb(220);
    grad.pb(990);
    grad.pb(1100);
    grad.pb(880);
    grad.pb(470);
    grad.pb(250);
    grad.pb(670);
    grad.pb(230);
    grad.pb(210);
    grad.pb(410);
}

vector<vector<Edge> > g(maxv); ///< Вектор для деикстры

#endif