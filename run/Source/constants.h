/*!
\file
\brief Файл содержит константы
*/


#ifndef constants_h
#define constants_h

#include "edge.h"
#include "speed.h"



const int cntplaces = 36;
const int maxv = 410;
const int ce = 21 * 2;
const int ver = 22;
const int black = 21;
const int bluck = 30;
const int bley = 40;
const double Pr = 0.3;
const int grey = 40;
const double dws = 180; // ���������� ����� ��������� � �������
const double dsl = 60; //����� �������� � �����
const double dtw = 340;
const int d90 = 270;
const int d180 = 540;
const int dovorot = 30;
const int d1w = 50;
const int w90 = 550;
const int ndir = 0;
const int speed = 23;
int speedD = 20;
int speedA = 100;

int get_deg_line_B = 0;
bool isend = 0;

double put_loops = 230;
double up_loops = 100;
double block_cubes = 334;
double after_take_cubes = 150;
double before_take_cubes = 460;
double before_take_blue_loops = 890;
double after_take_blue_loops = 500;
double before_take_green_loops = 555;
double after_take_green_loops = 200;
double up = 10;
double near_put_loops = 180;
double xren_loops = 490;

/*!
    \ingroup line
*/

///@{
const int lineArrayLen = 20;                            ///< Длинна массива задержек для линии
const int standart_line_preview_looking = 50;           ///< За это расстояние до конца линии робот начинает чекать датчиком конец
const int deltaSensors = -4;                             ///< Разность показаний датчиков
const Speed DOP1 = Speed(80, 20, 0.35, 0.7, 100, 300, 300, 100);   ///< Для двух
const Speed ONE1 = Speed(80, 20, 0.45, 1, 100, 300, 300, 100);     ///< Для одного
const Speed MIN = Speed(10, 10, 0.3, 0, 0, 0, 0, 0);
const Speed MIN1 = Speed(12, 12, 0.3, 0, 0, 0, 0, 0);
const Speed ONE = Speed(20, 20, 0.4, 0.2, 100, 50, 50, 30);        ///< Для одного
//Speed ZERO = Speed(80, 20, 0.5, 1, 50, 100, 100, 50);   ///< Настройка линии (функция lineNew) класс Speed
const Speed ZERO = Speed(20, 20, 0.2, 0.1, 0, 0, 0, 0); // p.s. для ИГОРЯ замедлил
//Speed ONEMOTOR = Speed(100, 20, 0, 0, 0, 100, 100, 0);    ///< Для moveB или moveС
const Speed ONEMOTOR = Speed(20, 20, 0.2, 0.1, 0, 0, 0, 0); // p.s. для ИГОРЯ замедлил
//Speed MOVEBC = Speed(100, 20, 0, 0, 100, 250, 250, 70);      ///< Для moveBC
const Speed MOVEBC = Speed(20, 20, 0.2, 0.1, 0, 0, 0, 0); // p.s. для ИГОРЯ замедлил
//Speed TURN = Speed(100, 15, 0, 0, 100, 300, 300, 50);        ///< Для turn
const Speed TURN = Speed(20, 20, 0.2, 0.1, 0, 0, 0, 0); // p.s. для ИГОРЯ замедлил
const Speed READ = Speed(30, 20, 0.5, 0.3, 100, 50, 50, 30);       ///< Для чтения
///}@

#define pb push_back

vector<int> grad;                                       ///< Длины основных элементов поля

void buildDegreesConstants() {                          /// Заполнение вектора grad
    grad.pb(850);
    grad.pb(390);
    grad.pb(470);
    grad.pb(820);
    grad.pb(760);
    grad.pb(590);
    grad.pb(590);
    grad.pb(270);
    grad.pb(220);
    grad.pb(980);
    grad.pb(1100);
    grad.pb(880);
    grad.pb(470);
    grad.pb(250);
    grad.pb(670);
    grad.pb(230);
    grad.pb(210);
    grad.pb(370);
    grad.pb(205);
    grad.pb(440);
    grad.pb(500);
}

vector<vector<Edge> > Edge::g = vector<vector<Edge> >(maxv);

vector<vector<Edge> > &g = Edge::g;

#endif