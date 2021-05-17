#ifndef constants_h
#define constants_h

#include "speed.h"

const int maxv = 500;
const int ce = 21 * 2;
const int ver = 22;
const int black = 13;
const int bluck = 25;
const int bley = 40;
const double Pr = 0.3;
const int grey = 40;
const double dws = 130; // ���������� ����� ��������� � �������
const double dsl = 50; //����� �������� � �����
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
double before_take_blue_loops = 230;
double after_take_blue_loops = 100;
double before_take_green_loops = 200;
double after_take_green_loops = 60;
double up = 10;

const int lineArrayLen = 100;
const int linePreviewLooking = 50;
Speed ZERO = Speed(23, 20, 0.3, 0.4, 1, 1, 1);

vector<int> grad;

#define pb push_back

void buildDegreesConstants() {
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

vector<vector<Edge> > g(maxv);


#endif