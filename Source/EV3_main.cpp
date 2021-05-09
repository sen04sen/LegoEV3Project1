#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <set>
#include <functional>

#include "EV3_Motor.h"
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include "EV3_Thread.h"
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"

#include "utils.h"
#include "motors.h"
#include "edge.h"
#include "sensors.h"
#include "line.h"
#include "constants.h"
#include "turn.h"
#include "field.h"
#include "file.h"


using namespace ev3_c_api;
using namespace std;

extern File logs;

#define pb push_back

int go(int sp, int from, int toto) {
    pair<pair<double, int>, Edge> msgo[maxv];
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

void give2() {
    stopBC();
    stopD();
    wait(30);
    moveD(-speedD, 400);
    moveBC(ZERO, 200);
    moveBC(ZERO, -110);
    moveD(speedD, 400);
    moveDTime(1, 500);
    stopD();
    moveD(-5, 8);
    wait(100);
    moveBC(ZERO, -150);
    moveD(-speedD, 100);
    moveBC(ZERO, 60);
    moveD(speedD, 120);
    goD(3);
}

void give4() {
    stopBC();
    stopD();
    wait(30);
    moveD(-speedD, 400);
    moveBC(ZERO, 200);
    moveBC(ZERO, -200);
    moveD(speedD, 400);
}

DoubleMarker gtf() {
    moveBC(ZERO, 35);
    SpeedMotor(E_Port_C, -speed);
    double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < turn1wheel);
    stopC();
    moveBC(ZERO, 365);
    GetColor(E_Port_4);
    EV3_Sleep(500);
    int fi = gclr(4);
    if (fi == 7)
        fi = 4;
    moveBC(ZERO, 120);
    EV3_Sleep(500);
    int se = gclr(4);
    if (se == 7)
        se = 4;
    stopBC();
    Clear_Display();
    write(1, 1, fi);
    write(51, 1, se);
    return DoubleMarker(fi, se);
}

Field field = StandartInit();
DoubleMarker& d1 = field.house1;
DoubleMarker& d2 = field.house2;
DoubleMarker& d3 = field.house3;
int& cnt1 = field.cnt1;
int& cnt2 = field.cnt2;
int& cnt3 = field.cnt3;

void end_4_green() {
    line(ZERO, 170, 2);
    moveBC(ZERO, -50);
    turn(speed, d90, -1);
    moveBC(ZERO, 140); // Была скорость 10
    moveD(-speedD, 70);
    moveBC(ZERO, -140);
    turn(speed, d90, 3);
    line(ZERO, 30, 2);
    moveBC(ZERO, dsl);
    line(ZERO, 220, 0);
    stopBC();
    turn(speed, d90, -1);
    moveD(10, 65);
    EV3_Sleep(1000);
    moveBC(ZERO, 140); // Была скорость 10
    moveD(-speedD, 65);
    moveBC(ZERO, -140);
    turn(speed, d90, 3);
    line(ZERO, 400, 3);
    if (d1.left == 3 || d1.right == 3) {
        moveBC(ZERO, dws);
        turn(speed, d180, 1);
        line(ZERO, 780, 3);
        moveBC(ZERO, dsl, 0);
        line(ZERO, 170, 2);
        moveBC(ZERO, dsl, 0);
        line(ZERO, 590, 3);
        moveBC(ZERO, dws);
        turn(speed, d90, 3);
        line(ZERO, 200, 4);
        moveBC(ZERO, 50);
        moveD(10, 110);
        moveBC(ZERO, -400);
        turn(speed, d90, 0);
        SpeedMotor_Time(E_Port_D, -30, 2000);
        wait(2000);
        stopD();
        line(ZERO, 270, 2);
        moveBC(ZERO, dws);
    } else {
        moveBC(ZERO, dsl, 0);
        line(ZERO, 760, 2);
        moveBC(ZERO, dws);
        turn(speed, d90, 0);
        line(ZERO, 200, 4);
        moveBC(ZERO, 50);
        moveD(10, 110);
        moveBC(ZERO, -400);
        turn(speed, d90, 0);
        SpeedMotor_Time(E_Port_D, -30, 2000);
        wait(2000);
        stopD();
        line(ZERO, 760, 2);
        moveBC(ZERO, dsl, 0);
        line(ZERO, 780, 3);
        moveBC(ZERO, dsl, 0);
        line(ZERO, 170, 2);
        moveBC(ZERO, dsl, 0);
        line(ZERO, 590, 3);
        moveBC(ZERO, dsl, 0);
        line(ZERO, 270, 2);
        moveBC(ZERO, dws);
        turn(speed, d90, 0);
        line(ZERO, 750, 0);
        moveBC(ZERO, 300);
        wait(10000);
    }
}

void turn_bat() {
    stopBC();
    moveD(speedD, 0);
    moveC(speed, 70, 1);
    moveB(speed, 70, 1);
    moveBC(ZERO, 130); // Была скорость 10
    moveD(15, 230);
    moveB(-speed, 30, 1);
    moveD(speedD, 250);
    moveBC(ZERO, -80);
    moveD(speedD, 520);
    turn(speed, d90 - 20, 3);
    line(ZERO, 320, 0);
    stopBC();
    turn(speed, d90, -1);
    moveBC(ZERO, -60);
    moveD(speedD, 20);
    moveBC(ZERO, 180);
    moveD(15, 230);
    moveB(-speed, 30, 1);
    moveD(speedD, 250);
    moveBC(ZERO, -90);
    moveD(speedD, 520);
    moveBC(ZERO, -60);
    turn(speed, d90 - 30, 0);
    turn(speed, 60, -1);
    line(ZERO, 50, 7);
    moveBC(ZERO, dsl, 0);
    line(ZERO, 535, 8);
    getRGB(2);
    moveBC(ZERO, 75, 0);
    goBC(speed);
    while (getRGB(2).b < 100);
    s2();
    moveBC(ZERO, 70);
    turn(speed, d90, -2);
}

void get_4_blue() {
    moveBC(ZERO, 40);
    turn(speed, d90, -2);
    moveBC(ZERO, 50, 0);
    line(ZERO, 460, 5);
    goBC(speed);
    while (s2() > black);
    stopBC();
    moveBC(ZERO, -30);
    turn(speed, d90, -1);
    moveBC(ZERO, -50);
    moveD(speedD, 395);
    moveBC(ZERO, 90);
    moveD(speedD, 280);
    moveBC(ZERO, -30);
    turn(speed, d90, -2);
    moveBC(ZERO, 130);
    goBC(speed, 2);
    while (s3() > bluck);
    stopBC();
    line(ZERO, 150, 6);
    stopBC();
    turn(speed, d90, -1);
    moveBC(ZERO, -50);
    moveD(speedD, 395);
    moveBC(ZERO, 90);
    moveD(speedD, 280);
    moveBC(ZERO, -60);
    turn(speed, d90, -1);
    moveBC(ZERO, 600, 0);
    goBC(speed);
    while (s2() > black);
    stopBC();
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
    moveBC(ZERO, dsl, 0);
}

void f8() {
    moveBC(ZERO, dws);
}

void f9() {
    line(ZERO, degreesConstants[1] - dws, 2);
}

void f10() {
    line(ZERO, degreesConstants[1] - dsl, 2);
}

void f11() {
    line(ZERO, degreesConstants[3] - dws, 3);
}

void f12() {
    line(ZERO, degreesConstants[3] - dsl, 3);
}

void f13() {
    line(ZERO, degreesConstants[2] - dws, 4);
}

void f14() {
    moveBC(ZERO, 35);
    SpeedMotor(E_Port_C, -speed);
    double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < turn1wheel);
    stopC();
    moveBC(ZERO, 365);
    GetColor(E_Port_4);
    EV3_Sleep(500);
    int fi = gclr(4);
    if (fi == 7)
        fi = 4;
    moveBC(ZERO, 120);
    EV3_Sleep(500);
    int se = gclr(4);
    if (se == 7)
        se = 4;
    stopBC();
    d1 = DoubleMarker(fi, se);
    Clear_Display();
    write(1, 1, fi);
    write(51, 1, se);
}

void f15() {
    moveBC(ZERO, -330);
    turn(speed, d90, 0);
    line(ZERO, 50, 1);
}

void f16() {
    line(ZERO, degreesConstants[4] - dws, 1);
}

void f17() {
    line(ZERO, degreesConstants[7] - dws, 2);
}

void f18() {
    line(ZERO, degreesConstants[7] - dsl, 2);
}

void f19() {
    line(ZERO, degreesConstants[9] - dws, 3);
}

void f20() {
    line(ZERO, degreesConstants[9] - dsl, 3);
}

void f21() {
    line(ZERO, degreesConstants[11] - dws, 2);
}

void f22() {
    line(ZERO, degreesConstants[11] - dsl, 2);
}

void f23() {
    line(ZERO, degreesConstants[13] - dws, 3);
}

void f24() {
    line(ZERO, degreesConstants[13] - dsl, 3);
}

void f25() {
    moveBC(ZERO, 340, 0);
    stopB();
    moveC(speed, 80, 1);
    moveB(speed, 890, 1);
    moveD(speedD, 520);
    turn(speed, 60, -1);
    if (gclr(4) != 0) {
        field.setYellowB();
    }
    write(1, 1, field.gdeb());
    moveBC(ZERO, 390, 0);
    goBC(speed);
    while (s2() > black);
    moveBC(ZERO, 100);
    turn(speed, 160, 3);
    line(ZERO, 200, 3);
}

void f26() {
    stopBC();
    stopD();
    wait(30);
    moveD(speedD, 420);
    moveBC(ZERO, 200);
    moveBC(ZERO, -200);
    moveD(speedD, 520);
}

void f27() {
    turn(speed, 180, 2);
    line(ZERO, degreesConstants[2] - dws, 1);
}

void add(int from, int to, void (*def)(), double time = 1.0) {
    g[from].pb(Edge(to, def, time));
}

void addcrossroad(int v, int u, int r, int d, int l) {
    if (u == 1) {
        g[v + 1].pb(Edge(v, f1));
        g[v + 2].pb(Edge(v, f2));
        g[v + 3].pb(Edge(v, f3));
    }
    else {
        g[v + 1].pb(Edge(v, f4));
        g[v + 2].pb(Edge(v, f5));
        g[v + 3].pb(Edge(v, f6));
    }
    if (r == 1) {
        g[v].pb(Edge(v + 1, f3));
        g[v + 2].pb(Edge(v + 1, f1));
        g[v + 3].pb(Edge(v + 1, f2));
    }
    else {
        g[v].pb(Edge(v + 1, f6));
        g[v + 2].pb(Edge(v + 1, f4));
        g[v + 3].pb(Edge(v + 1, f5));
    }
    if (d == 1) {
        g[v].pb(Edge(v + 2, f2));
        g[v + 1].pb(Edge(v + 2, f3));
        g[v + 3].pb(Edge(v + 2, f1));
    }
    else {
        g[v].pb(Edge(v + 2, f6));
        g[v + 1].pb(Edge(v + 2, f5));
        g[v + 3].pb(Edge(v + 2, f4));
    }
    if (l == 1) {
        g[v].pb(Edge(v + 3, f1));
        g[v + 1].pb(Edge(v + 3, f2));
        g[v + 2].pb(Edge(v + 3, f3));
    }
    else {
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

void buildG() {
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

    add(23, 111, f26);
    add(111, 23, f27);

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
    buildDegreesConstants();
    buildG();
    buildSpeedConstants();
    goD(-speed);
    wait(700);
    goD(0);
    go(speed, 0, 26);
    if (d1.left == 4 || d1.right == 4) {
        go(speed, 26, 111);
        go(speed, 11, 99);
    } else
        go(speed, 26, 99);
    turn_bat();
    return 0;
    /*house1 = gtf();
    gtb();
    if (house1.first == 4 || house1.second == 4) {
        stopBC();
        turn(speed, d180, 2);
        line(ZERO, 200, 4);
        p1 = 1;
        give4();
        turn(speed, d180, 2);
        line(ZERO, 250, 1);
    }
    ////go(speed, 2, 0, 6, 0, 0);
    turn_bat();
    moveD(-speedD, 140);
    moveBC(ZERO, 320);
    moveD(speedD, 110);
    goD(0);
    moveBC(ZERO, -320);
    turn(speed, d90 + 40, -1);
    goBC(speed);
    while (s2() > black);
    moveBC(ZERO, dws + 10);
    turn(speed, d90, 3);
    line(ZERO, 200, 4);
    d2 = gtf();
    moveBC(ZERO, -20);
    turn(speed, d90, -1);
    moveBC(ZERO, 10);
    if (gclr(4) != 0) {
        field.setBlueB();
    }
    moveB(speed, turn1wheel, 1);
    goBC(speed);
    while (s3() > black);
    moveBC(ZERO, dws);
    if (d2.first == 4 || d2.second == 4) {
        turn(speed, d90, 0);
        line(ZERO, 300, 4);
        if (p1)
            give4();
        else
            give2();
        p2 = 1;
        turn(speed, d180, 2);
        line(ZERO, 600, 1);
    } else {
        turn(speed, d90, 3);
        line(ZERO, 200, 1);
    }
    ////go(speed, 12, 2, 15, 2, 0);
    d3 = gtf();
    gtb();
    if (d3.first == 4 || d3.second == 4) {
        turn(speed, d180, 2);
        line(ZERO, 200, 4);
        if (p1 || p2)
            give4();
        else
            give2();
        p3 = 1;
        turn(speed, d180, 2);
        line(ZERO, 250, 1);
    }
    moveBC(ZERO, dws);
    turn(speed, d90, 0);
    line(ZERO, 100, 3);
    get_4_blue();*/
    return 0;
}