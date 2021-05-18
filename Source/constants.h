/*!
\file
\brief Файл содержит константы
*/


#ifndef constants_h
#define constants_h

#include "edge.h"
#include "speed.h"

const int maxv = 500;
const int ce = 21 * 2;
const int ver = 22;
const int black = 18;
const int bluck = 30;
const int bley = 40;
const double Pr = 0.3;
const int grey = 40;
const double dws = 130; // ���������� ����� ��������� � �������
const double dsl = 50; //����� �������� � �����
const double dtw = 360;
const int d90 = 275;
const int d180 = 550;
const int dovorot = 45;
const int turn1wheel = 550;
const int ndir = 0;
const int speed = 23;
int speedD = 40;

double put_loops = 230;
double up_loops = 100;
double block_cubes = 334;
double after_take_cubes = 150;
double before_take_cubes = 460;
double before_take_blue_loops = 240;
double after_take_blue_loops = 100;
double before_take_green_loops = 204;
double after_take_green_loops = 60;
double up = 10;
double near_put_loops = 180;
double xren_loops = 490;

/*!
    \ingroup line
*/

///@{
const int lineArrayLen = 20;                            ///< Длинна массива задержек для линии
const int linePreviewLooking = 100;                     ///< За это расстояние до конца линии робот начинает чекать датчиком конец
const int deltaSensors = 6;                             ///< Разность показаний датчиков
Speed DOP1 = Speed(80, 20, 0.35, 0.7, 300, 300, 100);   ///< Для двух
Speed ONE1 = Speed(80, 20, 0.45, 1, 300, 300, 100);     ///< Для одного
Speed MIN = Speed(23, 20, 0.3, 0.3, 1, 1, 1);
Speed MIN1 = Speed(12, 12, 0.3, 0, 0, 0, 0);
Speed ONE = Speed(30, 20, 0.5, 0.3, 50, 50, 30);        ///< Для одного
Speed ZERO = Speed(90, 20, 0.45, 1, 300, 300, 100);     ///< Настройка линии (функция lineNew) класс Speed
Speed ONEMOTOR = Speed(100, 20, 0, 0, 100, 150, 50);    ///< Для moveB или moveС
Speed TURN = Speed(100, 15, 0, 0, 300, 300, 50);        ///< Для turn
///}@

vector<vector<Edge> > g(maxv);                          ///< Вектор для деикстры

#define pb push_back

vector<int> grad;                                       ///< Длины основных элементов поля

void buildDegreesConstants() {                          /// Заполнение вектора grad
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

#endif