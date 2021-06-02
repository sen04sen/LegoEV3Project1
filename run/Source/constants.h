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
const int black = 20;
const int white = 35;
const int bluck = 30;
const int bley = 40;
const double Pr = 0.3;
const int grey = 40;
const double dws = 170; // ���������� ����� ��������� � �������
const double dsl = 60; //����� �������� � �����
const double dtw = 340;
const int d90 = 270;
const int d180 = 540;
const int dovorot = 30;
const int d1w = 50;
const int w90 = 550;
const int ndir = 0;
const int speed = 30;
int speedD = 20;
int speedA = 100;

int get_deg_line_B = 0;
bool isend = 0;

double put_loops = 230;
double up_loops = 100;
double block_cubes = 334;
double after_take_cubes = 150;
double before_take_cubes = 460;
double before_take_blue_loops = 860;
double give2loops = 810;
double after_take_blue_loops = 200;
double before_take_green_loops = 520;
double after_take_green_loops = 200;
double up = 10;
double near_put_loops = 180;
double xren_loops = 490;

/*!
    \ingroup line43, 
*/

///@{
const int32_t lineArrayLen = 5;                            ///< Длинна массива задержек для линии
const double standart_line_preview_looking = 0.6;           ///< За это расстояние до конца линии робот начинает чекать датчиком конец
const int32_t deltaSensors = -10;                             ///< Разность показаний датчиков
const double end_line = 1.1;           ///< За это расстояние до конца линии робот начинает чекать датчиком конец
const Speed MIN = Speed(12, 12, 0.15, 0.0, 50, 50, 50, 50);
const Speed ONE = Speed(12, 12, 1, 0.0, 50, 50, 50, 50);       ///< Для одного
const Speed ZERO = Speed(45, 20, 0.22, 0.24, 100, 50, 50, 50); // p.s. для ИГОРЯ замедлил
const Speed ONEMOTOR = Speed(20, 20, 0.3, 0, 0, 0, 0, 0); // p.s. для ИГОРЯ замедлил
const Speed MOVEBC = Speed(20, 20, 0.2, 0, 0, 0, 0, 0); // p.s. для ИГОРЯ замедлил
const Speed TURN = Speed(20, 20, 0.2, 0.2, 0, 0, 0, 0); // p.s. для ИГОРЯ замедлил
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
    grad.pb(540);
    grad.pb(540);
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

int Edge::convert_ms = 50;

#endif