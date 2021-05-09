#ifndef constants_h
#define constants_h

#include <map>
#include <vector>

#include "speed.h"
#include "file.h"

using namespace ev3_c_api;
using namespace std;

const int maxv = 500;
const int ce = 21 * 2;
const int ver = 22;
const int black = 25;
const int bluck = 35;
const int bley = 45;
const double Pr = 0.3;
const int grey = 35;
const double dws = 130; // ���������� ����� ��������� � �������
const double dsl = 50; //����� �������� � �����
const int d90 = 250;
const int d180 = 500;
const int dovorot = 50;
const int turn1wheel = 525;
const int ndir = 0;
const int speed = 23;
const int speedD = 40;

vector<int> degreesConstants;

#define pb push_back

void buildDegreesConstants() {
    degreesConstants.pb(850);
    degreesConstants.pb(390);
    degreesConstants.pb(470);
    degreesConstants.pb(820);
    degreesConstants.pb(760);
    degreesConstants.pb(580);
    degreesConstants.pb(580);
    degreesConstants.pb(270);
    degreesConstants.pb(220);
    degreesConstants.pb(990);
    degreesConstants.pb(1100);
    degreesConstants.pb(880);
    degreesConstants.pb(470);
    degreesConstants.pb(250);
    degreesConstants.pb(670);
    degreesConstants.pb(230);
    degreesConstants.pb(210);
    degreesConstants.pb(410);
}

vector<vector<Edge > > g(maxv);

const int lineArrayLen = 4;
const int linePreviewLooking = 50; // расстояние начала просмотра того, что пора тормозить

void buildSpeedConstants() {
    Speed::add(ZERO, 23, 20, 1.0, 1.0, 1, 1, 1);
}

extern File logs = File("logs.txt", true, true);

#endif