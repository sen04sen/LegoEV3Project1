#include <EV3_Motor.h>
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "EV3_Thread.h"
#include <sstream>
#include <set>
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"
#include "utils.h"
#include "motors.h"
#include "edge.h"
#include "sensors.h"
#include "line.h"

using namespace ev3_c_api;
using namespace std;

#define pb push_back


const int maxv = 500;
int ce = 21 * 2;
int ver = 22;

vector<vector<Edge> > g(maxv);


double Pr = 0.3;
double dws = 130; // расстояние между датчиками и колёсами
double dsl = 50; //съезд датчиков с линии
int d90 = 250;
int d180 = 500;
int black = 25;
int bluck = 35;
int grey = 35;
int bley = 45;
int dovorot = 50;
int turn1wheel = 525;

int ndir = 0;
int speed = 23;
int speedD = 40;


void turn(int sp, int dt, int tp) {
    if (tp >= 0) {
        if (tp < 2 || tp == 4) {
            SpeedMotor(E_Port_B, -1 * (sp));
            SpeedMotor(E_Port_C, -1 * (sp));
        } else {
            SpeedMotor(E_Port_B, sp);
            SpeedMotor(E_Port_C, sp);
        }

        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 80);
        if (tp == 4)
            while (s3() > bluck);
        else if (tp == 5)
            while (s2() > bluck);
        else if (tp < 2) {
            while (s3() > black);
        } else {
            while (s2() > black);
        }
        st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
        stopBC();
    } else {
        if (tp == -1) {
            SpeedMotor(E_Port_B, -1 * (sp));
            SpeedMotor(E_Port_C, -1 * (sp));
        } else {
            SpeedMotor(E_Port_B, sp);
            SpeedMotor(E_Port_C, sp);
        }
        double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
        while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt);
        stopBC();
    }
}


pair<pair<double, int>, Edge> msgo[maxv];

int go(int sp, int from, int toto) {
    for (int i = 0; i < maxv; i++) {
        msgo[i].first.first = (double) 1000000000;
        msgo[i].first.second = -1;
    }
    msgo[from].first.first = (double) 0;
    set<pair<double, int> > st;
    st.insert(make_pair((double) 0, from));
    bool end = 0;
    while (!st.empty()) {
        double dd = st.begin()->first;
        int v = st.begin()->second;
        st.erase(st.begin());
        for (int i = 0; i < g[v].size(); i++) {
            int to = g[v][i].getTo();
            if (dd + g[v][i].getTime() < msgo[to].first.first) {
                st.erase(make_pair(msgo[to].first.first, to));
                msgo[to].first.first = dd + g[v][i].getTime();
                msgo[to].first.second = v;
                msgo[to].second = g[v][i];
                if (to == toto) {
                    end = 1;
                    break;
                }
                st.insert(make_pair(msgo[to].first.first, to));
            }
        }
        if (end)
            break;
    }
    vector<Edge> way;
    int nv = toto;
    while (nv != from) {
        way.pb(msgo[nv].second);
        nv = msgo[nv].first.second;
    }
    for (int i = way.size() - 1; i >= 0; i--) {
        Edge nw = way[i];
        nw();
    }
    return way.size();
}

void vivod_4() {
    for (int i = 0; i < 100; i++) {
        const void *a = GetData_UART(E_Port_3, E_UART_Type_Color, 4);
        unsigned char *d = reinterpret_cast<unsigned char *>(const_cast<void *>(a));
        int r = d[0];
        int g = d[2];
        int b = d[4];
        Clear_Display();
        write(1, 1, r);
        write(41, 1, g);
        write(81, 1, b);
        EV3_Sleep(200);
    }
}

void vivod_clr() {
    for (int i = 0; i < 100; i++) {
        Clear_Display();
        write(1, 1, GetColor(E_Port_3));
        EV3_Sleep(200);
    }
}

void give2() {
    stopBC();
    stopD();
    wait(30);
    moveD(-speedD, 400);
    moveBC(speed, 200);
    moveBC(-speed, 110);
    moveD(speedD, 400);
    moveDTime(1, 500);
    stopD();
    moveD(-5, 8);
    wait(100);
    moveBC(-speed, 150);
    moveD(-speedD, 100);
    moveBC(speed, 60);
    moveD(speedD, 120);
    goD(3);
}

void give4() {
    stopBC();
    stopD();
    wait(30);
    moveD(-speedD, 400);
    moveBC(speed, 200);
    moveBC(-speed, 200);
    moveD(speedD, 400);
}

pair<int, int> gtf() {
    moveBC(speed, 35);
    SpeedMotor(E_Port_C, -speed);
    double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < turn1wheel);
    stopC();
    moveBC(speed, 365);
    GetColor(E_Port_4);
    EV3_Sleep(500);
    int fi = gclr(4);
    if (fi == 7)
        fi = 4;
    moveBC(speed, 120);
    EV3_Sleep(500);
    int se = gclr(4);
    if (se == 7)
        se = 4;
    stopBC();
    Clear_Display();
    write(1, 1, fi);
    write(51, 1, se);
    return make_pair(fi, se);
}

void gtb() {
    moveBC(-speed, 330);
    turn(speed, d90, 0);
    line(speed, 50, 1);
}

pair<int, int> d1;
pair<int, int> d2;
pair<int, int> d3;
bool p1 = 0, p2 = 0, p3 = 0;
double st;
int gdeb = 3;

void end_4_green() {
    line(speed, 170, 2);
    moveBC(-speed, 50, 1);
    turn(speed, d90, -1);
    moveBC(10, 140, 1);
    moveD(-speedD, 70);
    moveBC(-speed, 140, 1);
    turn(speed, d90, 3);
    line(speed, 30, 2);
    moveBC(speed, dsl, 1);
    line(speed, 220, 0);
    stopBC();
    turn(speed, d90, -1);
    moveD(10, 65);
    EV3_Sleep(1000);
    moveBC(10, 140, 1);
    moveD(-speedD, 65);
    moveBC(-speed, 140, 1);
    turn(speed, d90, 3);
    line(speed, 400, 3);
    if (d1.first == 3 || d1.second == 3) {
        moveBC(speed, dws, 1);
        turn(speed, d180, 1);
        line(speed, 780, 3);
        moveBC(speed, dsl, 0);
        line(speed, 170, 2);
        moveBC(speed, dsl, 0);
        line(speed, 590, 3);
        moveBC(speed, dws, 1);
        turn(speed, d90, 3);
        line(speed, 200, 4);
        moveBC(speed, 50, 1);
        moveD(10, 110);
        moveBC(-speed, 400, 1);
        turn(speed, d90, 0);
        SpeedMotor_Time(E_Port_D, -30, 2000);
        wait(2000);
        stopD();
        line(speed, 270, 2);
        moveBC(speed, dws, 1);
    } else {
        moveBC(speed, dsl, 0);
        line(speed, 760, 2);
        moveBC(speed, dws, 1);
        turn(speed, d90, 0);
        line(speed, 200, 4);
        moveBC(speed, 50, 1);
        moveD(10, 110);
        moveBC(-speed, 400, 1);
        turn(speed, d90, 0);
        SpeedMotor_Time(E_Port_D, -30, 2000);
        wait(2000);
        stopD();
        line(speed, 760, 2);
        moveBC(speed, dsl, 0);
        line(speed, 780, 3);
        moveBC(speed, dsl, 0);
        line(speed, 170, 2);
        moveBC(speed, dsl, 0);
        line(speed, 590, 3);
        moveBC(speed, dsl, 0);
        line(speed, 270, 2);
        moveBC(speed, dws, 1);
        turn(speed, d90, 0);
        line(speed, 750, 0);
        moveBC(speed, 300, 1);
        wait(10000);
    }
}

void turn_bat() {
    stopBC();
    moveD(speedD, 0);
    moveC(speed, 70, 1);
    moveB(speed, 70, 1);
    moveBC(speed, 130, 1);
    moveD(speedD, 220);
    moveB(-speed, 30, 1);
    moveD(speedD, 250);
    moveBC(-speed, 90, 1);
    moveD(speedD, 500);
    pov(speed, d90 - 20, 3);
    line(speed, 310, 0);
    stopBC();
    turn(speed, d90, -1);
    moveBC(-speed, 60, 1);
    moveD(speedD, 20);
    moveBC(speed, 170, 1);
    moveD(speedD, 220);
    moveB(-speed, 30, 1);
    moveD(speedD, 250);
    moveBC(-speed, 90, 1);
    moveD(speedD, 500);
    moveBC(-speed, 60, 1);
    pov(speed, d90 - 30, 0);
    pov(speed, 60, -1);
    line(speed, 50, 7);
    moveBC(speed, dsl, 0);
    line(speed, 550, 8);
    getRGB(2);
    moveBC(speed, 60, 0);
    goBC(speed);
    while (getRGB(2).b < 100);
    s2();
    moveBC(speed, 70, 1);
    turn(speed, d90, -2);
}

void get_4_blue() {
    moveBC(speed, 40, 1);
    turn(speed, d90, -2);
    moveBC(speed, 50, 0);
    line(speed, 460, 5);
    goBC(speed);
    while (s2() > black);
    stopBC();
    moveBC(-speed, 30, 1);
    turn(speed, d90, -1);
    moveBC(-speed, 50, 1);
    moveD(speedD, 395);
    moveBC(speed, 90, 1);
    moveD(speedD, 280);
    moveBC(-speed, 30, 1);
    turn(speed, d90, -2);
    moveBC(speed, 130, 1);
    goBC(speed, 2);
    while (s3() > bluck);
    stopBC();
    line(speed, 150, 6);
    stopBC();
    turn(speed, d90, -1);
    moveBC(-speed, 50, 1);
    moveD(speedD, 395);
    moveBC(speed, 90, 1);
    moveD(speedD, 280);
    moveBC(-speed, 60, 1);
    turn(speed, d90, -1);
    moveBC(speed, 600, 0);
    goBC(speed);
    while (s2() > black);
    stopBC();
}

vector<int> grad;



void buildgrad() {
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

void f1() {
    turn(speed, d90, 3);
}

void f2() {
    turn(speed, d180, 2);
}

void f3() {
    turn(speed, d90, 0);
}

void f4() {
    turn(speed, d90, -2);
}

void f5() {
    turn(speed, d180, -2);
}

void f6() {
    turn(speed, d90, -1);
}

void f7() {
    moveBC(speed, dsl, 0);
}

void f8() {
    moveBC(speed, dws, 1);
}

void f9() {
    line(speed, grad[1] - dws, 2);
}

void f10() {
    line(speed, grad[1] - dsl, 2);
}

void f11() {
    line(speed, grad[3] - dws, 3);
}

void f12() {
    line(speed, grad[3] - dsl, 3);
}

void f13() {
    line(speed, grad[2] - dws, 4);
}

void f14() {
    gtf();
}

void f15() {
    gtb();
}

void f16() {
    line(speed, grad[4] - dws, 1);
}

void f17() {
    line(speed, grad[7] - dws, 2);
}

void f18() {
    line(speed, grad[7] - dsl, 2);
}

void f19() {
    line(speed, grad[9] - dws, 3);
}

void f20() {
    line(speed, grad[9] - dsl, 3);
}

void f21() {
    line(speed, grad[11] - dws, 2);
}

void f22() {
    line(speed, grad[11] - dsl, 2);
}

void f23() {
    line(speed, grad[13] - dws, 3);
}

void f24() {
    line(speed, grad[13] - dsl, 3);
}

void f25() {
    moveBC(speed, 340, 0);
    stopB();
    moveC(speed, 80, 1);
    moveB(speed, 890, 1);
    moveD(speedD, 520);
    pov(speed, 60, -1);
    wait(2000);
    if (gclr(4) != 0) {
        gdeb = 4;
    }
    write(1, 1, gdeb);
    wait(2000);
    moveBC(speed, 380, 0);
    goBC(speed);
    while (s2() > black);
    moveBC(speed, 100, 1);
    pov(speed, 160, 3);
    line(speed, 200, 3);
}



void add(int from, int to, void (*def)(), double time = 1.0) {
    g[from].pb(Edge(to, def, time));
}

void addcrossroad(int v, int u, int r, int d, int l) {
    if (u == 1) {
        g[v + 1].pb(Edge(v, f1));
        g[v + 2].pb(Edge(v, f2));
        g[v + 3].pb(Edge(v, f3));
    } else {
        g[v + 1].pb(Edge(v, f4));
        g[v + 2].pb(Edge(v, f5));
        g[v + 3].pb(Edge(v, f6));
    }
    if (r == 1) {
        g[v].pb(Edge(v + 1, f3));
        g[v + 2].pb(Edge(v + 1, f1));
        g[v + 3].pb(Edge(v + 1, f2));
    } else {
        g[v].pb(Edge(v + 1, f6));
        g[v + 2].pb(Edge(v + 1, f4));
        g[v + 3].pb(Edge(v + 1, f5));
    }
    if (d == 1) {
        g[v].pb(Edge(v + 2, f2));
        g[v + 1].pb(Edge(v + 2, f3));
        g[v + 3].pb(Edge(v + 2, f1));
    } else {
        g[v].pb(Edge(v + 2, f6));
        g[v + 1].pb(Edge(v + 2, f5));
        g[v + 3].pb(Edge(v + 2, f4));
    }
    if (l == 1) {
        g[v].pb(Edge(v + 3, f1));
        g[v + 1].pb(Edge(v + 3, f2));
        g[v + 2].pb(Edge(v + 3, f3));
    } else {
        g[v].pb(Edge(v + 3, f4));
        g[v + 1].pb(Edge(v + 3, f5));
        g[v + 2].pb(Edge(v + 3, f6));
    }
    g[v + 8].pb(Edge(v + 2, f8));
    g[v + 8].pb(Edge(v + 6, f7));
    g[v + 10].pb(Edge(v, f8));
    g[v + 10].pb(Edge(v + 4, f7));
    g[v + 11].pb(Edge(v + 1, f8));
    g[v + 11].pb(Edge(v + 5, f7));
    g[v + 9].pb(Edge(v + 3, f8));
    g[v + 9].pb(Edge(v + 7, f7));
}


void buildg() {
    addcrossroad(1, 1, 1, 0, 0);
    addcrossroad(13, 0, 1, 1, 1);
    addcrossroad(27, 1, 1, 0, 1);
    addcrossroad(39, 0, 1, 1, 1);
    addcrossroad(51, 1, 1, 0, 1);
    addcrossroad(63, 0, 1, 1, 1);

    add(2, 24, f9);
    add(16, 10, f9);
    add(20, 10, f10);

    add(14, 38, f11);
    add(18, 38, f12);
    add(30, 22, f11);
    add(34, 22, f12);

    add(15, 25, f13);

    add(25, 26, f14);

    add(26, 23, f15);

    add(27, 99, f16);

    add(28, 50, f17);
    add(32, 50, f18);
    add(42, 36, f17);
    add(46, 36, f18);

    add(40, 62, f19);
    add(44, 62, f20);
    add(54, 48, f19);
    add(58, 48, f20);

    add(52, 74, f21);
    add(56, 74, f22);
    add(66, 60, f21);
    add(70, 60, f22);

    add(64, 86, f23);
    add(68, 86, f24);

    add(0, 9, f25);
}


signed EV3_main() {
    Clear_Display();
    CreateThread(okonchanie, 0);
    buildgrad();
    buildg();
    goD(-speed);
    wait(1000);
    goD(0);
    go(speed, 0, 26);
    go(speed, 26, 99);
    pov_bat();
    return 0;
    d1 = gtf();
    gtb();
    if (d1.first == 4 || d1.second == 4) {
        stopBC();
        turn(speed, d180, 2);
        line(speed, 200, 4);
        p1 = 1;
        give4();
        turn(speed, d180, 2);
        line(speed, 250, 1);
    }
    ////go(speed, 2, 0, 6, 0, 0);
    turn_bat();
    moveD(-speedD, 140);
    moveBC(speed, 320, 1);
    moveD(speedD, 110);
    goD(0);
    moveBC(-speed, 320, 1);
    turn(speed, d90 + 40, -1);
    goBC(speed);
    while (s2() > black);
    moveBC(speed, dws + 10, 1);
    turn(speed, d90, 3);
    line(speed, 200, 4);
    d2 = gtf();
    moveBC(-speed, 20, 1);
    turn(speed, d90, -1);
    moveBC(speed, 10, 1);
    if (gclr(4) != 0) {
        gdeb = 2;
    }
    moveB(speed, turn1wheel, 1);
    goBC(speed);
    while (s3() > black);
    moveBC(speed, dws, 1);
    if (d2.first == 4 || d2.second == 4) {
        turn(speed, d90, 0);
        line(speed, 300, 4);
        if (p1)
            give4();
        else
            give2();
        p2 = 1;
        turn(speed, d180, 2);
        line(speed, 600, 1);
    } else {
        turn(speed, d90, 3);
        line(speed, 200, 1);
    }
    ////go(speed, 12, 2, 15, 2, 0);
    d3 = gtf();
    gtb();
    if (d3.first == 4 || d3.second == 4) {
        turn(speed, d180, 2);
        line(speed, 200, 4);
        if (p1 || p2)
            give4();
        else
            give2();
        p3 = 1;
        turn(speed, d180, 2);
        line(speed, 250, 1);
    }
    moveBC(speed, dws, 1);
    turn(speed, d90, 0);
    line(speed, 100, 3);
    get_4_blue();
    return 0;
}