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
#include <functional>
#include "utils.h"
#include "motors.h"
#include "edge.h"
#include "sensors.h"
#include "line.h"
#include "constants.h"
#include "turn.h"
#include "field.h"

using namespace ev3_c_api;
using namespace std;

#define pb push_back

double block_cubes = 330;
double before_take_cubes = 200;
double after_take_cubes = 460;
double before_take_loops = 340;
double after_take_loops = 440;
double up = 10;


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
    moveBC(10, 130, 1);
    moveD(15, 230);
    moveB(-speed, 30, 1);
    moveD(speedD, 250);
    moveBC(-speed, 60, 1);
    moveD(speedD, 520);
    turn(speed, d90 - 20, 3);
    line(speed, 320, 0);
    stopBC();
    turn(speed, d90, -1);
    moveBC(-speed, 60, 1);
    moveD(speedD, 20);
    moveBC(speed, 180, 1);
    moveD(15, 230);
    moveB(-speed, 30, 1);
    moveD(speedD, 250);
    moveBC(-speed, 90, 1);
    moveD(speedD, 520);
    moveBC(-speed, 60, 1);
    turn(speed, d90 - 30, 0);
    turn(speed, 60, -1);
    line(speed, 50, 7);
    moveBC(speed, dsl, 0);
    line(speed, 510, 8);
    stopBC();
    wait(5000);
    getRGB(2);
    moveBC(speed, 70, 1); 
    stopBC();
    wait(5000);
    return;
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


pair<int, int> gtf() {
    moveBC(-speed, 120);
    turn(speed, d90, -1);
    moveD(speedD, up);
    moveBC(speed, 215);
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
    d1 = make_pair(fi, se);
    Clear_Display();
    write(1, 1, fi);
    write(51, 1, se);
    moveD(speedD, block_cubes);
    return make_pair(fi, se);
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
    line(speed, degreesConstants[1] - dws, 2);
}

void f10() {
    line(speed, degreesConstants[1] - dsl, 2);
}

void f11() {
    line(speed, degreesConstants[3] - dws, 3);
}

void f12() {
    line(speed, degreesConstants[3] - dsl, 3);
}

void f13() {
    line(speed, degreesConstants[2] - dws, 4);
}

void f14() {
    d1 = gtf();
}

void f15() {
    moveBC(-speed, 60);
    moveD(speedD, block_cubes);
    moveBC(-speed, 270);
}

void f15a() {
    turn(speed, d90, 3);
    line(speed, 50, 4);
}

void f15b() {
    turn(speed, d90, 0);
    line(speed, 50, 1);
}

void f16() {
    line(speed, degreesConstants[4] - dws, 1);
}

void f17() {
    line(speed, degreesConstants[7] - dws, 2);
}

void f18() {
    line(speed, degreesConstants[7] - dsl, 2);
}

void f19() {
    line(speed, degreesConstants[9] - dws, 3);
}

void f20() {
    line(speed, degreesConstants[9] - dsl, 3);
}

void f21() {
    line(speed, degreesConstants[11] - dws, 2);
}

void f22() {
    line(speed, degreesConstants[11] - dsl, 2);
}

void f23() {
    line(speed, degreesConstants[13] - dws, 3);
}

void f24() {
    line(speed, degreesConstants[13] - dsl, 3);
}

void f25() {
    moveBC(speed, 380, 0);
    stopB();
    moveC(speed, 120, 1);
    moveB(speed, 930, 1);
    moveD(speedD, block_cubes);
    turn(speed, 60, -1);
    if (gclr(4) != 0) {
        gdeb = 4;
    }
    write(1, 1, gdeb);
    moveD(speedD, up);
    moveBC(speed, 390, 0);
    goBC(speed);
    while (s2() > black);
    moveBC(speed, 100, 1);
    turn(speed, 160, 3);
    moveD(speedD, block_cubes);
    line(speed, 200, 3);
}

void f26() {
    turn(speed, d180, 2);
    line(speed, degreesConstants[2] - 2 * dws, 1);
}

void f28() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, 320, 1);
    moveD(speedD, block_cubes);
    goD(0);
}

void f29() {
    moveBC(-speed, 320, 1);
}

void f30() {
    turn(speed, d90 + 30, -1);
    goBC(speed);
    while (s2() > black);
    moveBC(speed, dws + 10, 1);
}

void f31() {
    turn(speed, d90 + 40, 3);
    line(speed, 200, 4);
}

void f32() {
    d2 = gtf();
}

void f33() {
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
}

void f34() {
    turn(speed, d90, 0);
    line(speed, 300, 4);
}

void f35() {
    turn(speed, d180, 2);
    line(speed, degreesConstants[10] - dws * 2, 1);
}

void f36() {
    turn(speed, d90, 3);
    line(speed, 200, 1);
}

void f37() {
    d3 = gtf();
}

void f38() {
    moveBC(speed, 40);
    turn(speed, d90, -2);
}

void f39() {
    moveBC(speed, 50, 0);
    line(speed, 460, 5);
    goBC(speed);
    while (s2() > black);
}

void f40() {
    wait(30);
    moveBC(-15, 60);
    moveD(speedD, before_take_cubes);
    line(speed, 130, 0);
    moveBC(speed, 420);
    moveD(speedD, block_cubes);
}

void f41() {
    turn(speed, d90, -2);
    moveBC(speed, 440, 0);
    while (s2() > black);
    moveBC(speed, dws);
}

void f42() {
    turn(speed, d90, 0);
    line(speed, 500, 2);
}

void f43() {
    turn(speed, d90, 3);
    line(speed, 200, 2);
}

void f44() {
    moveBC(speed, dws - 20);
}

void f45() {
    turn(speed, d90, 0);
    turn(speed, 30, -1);
}

void f46() {
    line(speed, degreesConstants[6]- dws, 8);
    getRGB(2);
    moveBC(speed, 55, 0);
    while (getRGB(2).b < 100);
    moveBC(speed, 70, 1);
    s2();
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

    add(26, 120, f15);
    add(120, 25, f15a);
    add(120, 23, f15b);

    add(25, 23, f26);

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

    add(112, 113, f4);
    add(113, 114, f28);
    add(114, 113, f29);
    add(113, 115, f30);
    add(115, 116, f31);

    add(116, 117, f32);
    add(117, 119, f33);

    add(119, 116, f34);

    add(116, 59, f35);
    add(119, 59, f36);

    add(65, 121, f13);

    add(121, 122, f37);
    add(122, 123, f15);
    add(123, 121, f15a);
    add(123, 73, f15b);
    add(121, 73, f26);

    add(86, 75, f38);
    add(75, 97, f39);
    add(97, 87, f8);
    add(87, 90, f1);
    add(90, 124, f40);
    add(124, 125, f41);
    add(125, 60, f42);
    add(125, 74, f43);

    add(108, 99, f44);
    add(99, 100, f45);
    add(100, 112, f46);
}

signed EV3_main() {
    Clear_Display();
    CreateThread(okonchanie, 0);
    buildDegreesConstants();
    buildg();
    goD(speedD);
    wait(700);
    goD(0);
    go(speed, 0, 26);
    if (d1.first == 4 || d1.second == 4) {
        go(speed, 26, 25);
        stopBC();
        moveD(speedD, before_take_cubes);
        moveBC(speed, 270);
        moveBC(-speed, 270);
        moveD(speedD, block_cubes);
        go(speed, 25, 114);
    }
    else
        go(speed, 26, 114);
    go(speed, 114, 117);
    go(speed, 117, 119);
    if (d2.first == 4 || d2.second == 4) {
        go(speed, 119, 116);
        stopBC();
        moveD(speedD, before_take_cubes);
        moveBC(speed, 270);
        moveBC(-speed, 270);
        moveD(speedD, block_cubes);
        go(speed, 116, 122);
    }
    else {
        go(speed, 119, 122);
    }
    go(speed, 122, 123);
    if (d3.first == 4 || d3.second == 4) {
        go(speed, 123, 121);
        stopBC();
        moveD(speedD, before_take_cubes);
        moveBC(speed, 270);
        moveBC(-speed, 270);
        moveD(speedD, block_cubes);
        go(speed, 121, 124);
    }
    else {
        go(speed, 123, 124);
    }
    go(speed, 124, 25);
    stopBC();
    return 0;
}