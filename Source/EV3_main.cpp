// docs https://sen04sen.github.io/LegoEV3Project1/html/index.html

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
#include "turn.h"
#include "line.h"
#include "constants.h"
#include "field.h"
#include "EV3_Bluetooth.h"


using namespace ev3_c_api;
using namespace std;

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

Field field = StandartInit();
DoubleMarker &d1 = field.house1;
DoubleMarker &d2 = field.house2;
DoubleMarker &d3 = field.house3;
bool p1 = 0, p2 = 0, p3 = 0;
double st;
int gdeb = 3;
int nv = 0;
int c_cubes = 0;
int c_loops = 0;

void unloading_cubes(int cnt) {
    stopBC();
    if (cnt == 4) {
        moveA(1);
        moveBC(speed, 150);
        moveBC(speed, -170);
        moveA(0);
        moveBC(speed, 80);
        moveBC(speed, -100);
    } else {
        moveA(1);
        moveBC(speed, 160);
        moveBC(-speed, 50);
        moveA(0);
        moveBC(-speed, 120);
    }
}

void unloading_loops(int cnt) {
    stopBC();
    if (cnt == 4) {
        moveBC(-speed, 250);
        moveD(20, near_put_loops);
        wait(300);
        moveD(speedD, put_loops);
        moveBC(speed, 80);
        moveBC(-speed, 200);
        moveD(speedD, up_loops);
        moveA(0);
        moveBC(speed, 470);
        moveBC(-speed, 100);
    } else {
        moveBC(-speed, 250);
        moveD(20, near_put_loops);
        wait(300);
        moveD(speedD, xren_loops);
        moveBC(-speed, 80);
        moveD(speedD, up_loops);
        moveA(0);
        moveBC(speed, 430);
        moveBC(-speed, 100);
    }
}

void get_4_blue() {
    int lala = speedD;
    moveA(2);
    speedD = 15;
    stopBC();
    wait(50);
    moveBC(speed, -30, 1);
    turn(speed, d90, 0);
    moveBC(speed, -170, 1);
    moveD(speedD, before_take_blue_loops);
    moveBC(speed, 90, 1);
    moveD(speedD, after_take_blue_loops);
    c_loops += 2;
    moveBC(speed, 70, 1);
    turn(speed, d90 - 20, -2);
    moveBC(speed, 130);
    goBC(7, 2);
    while (s3() > bley);
    stopBC();
    lineNEW(ONE, 150, 6);
    stopBC();
    turn(speed, d90, 0);
    moveBC(speed, -200, 1);
    moveD(speedD, before_take_blue_loops);
    moveBC(speed, 110, 1);
    moveD(speedD, after_take_blue_loops);
    moveBC(speed, 30);
    c_loops += 2;
    turn(speed, d90 - 20, -1);
    moveBC(speed, 210);
    goBC(7, 1);
    while (s2() > bley);
    stopBC();
    lineNEW(ONE, 370, 9);
    moveBC(speed, 35);
    moveB(speed, turn1wheel);
    speedD = lala;
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
    moveBC(speed, -60, 1);
    moveD(speedD, 520);
    turn(speed, d90 - 20, 3);
    line(speed, 320, 0);
    stopBC();
    turn(speed, d90, 0);
    moveBC(speed, -60, 1);
    moveD(speedD, 20);
    moveBC(speed, 180, 1);
    moveD(15, 230);
    moveB(-speed, 30, 1);
    moveD(speedD, 250);
    moveBC(speed, -90, 1);
    moveD(speedD, 520);
    moveBC(speed, -60, 1);
    turn(speed, d90 - 30, 0);
    turn(speed, 60, -1);
    lineNEW(ONE, 50, 7);
    moveBC(speed, dsl, 0);
    lineNEW(ONE, 510, 8);
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
    turn(speed, d90, 3);
}


DoubleMarker gtf() {
    moveBC(15, -140);
    gclr(4);
    turn(15, d90, 0);
    moveBCNEW(MIN1, 185, 1);
    wait(500);
    int fi = gclr(4);
    if (fi == 7)
        fi = 4;
    if (fi == 1)
        fi = 2;
    moveBCNEW(MIN1, 120, 1);
    wait(500);
    int se = gclr(4);
    if (se == 7)
        se = 4;
    if (se == 1)
        se = 2;
    stopBC();
    Clear_Display();
    write(1, 1, fi);
    write(51, 1, se);
    return DoubleMarker(Color(fi), Color(se));
}

void f228() {
    line(speed, grad[10] - dws, 4);
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
    turn(speed, d90, 3);
}

void f5() {
    turn(speed, d180, 3);
}

void f6() {
    turn(speed, d90, 0);
}

void f7() {
    moveBC(speed, dsl, 0);
}

void f8() {
    moveBC(speed, dws, 1);
}

void f9() {
    lineNEW(MIN1, grad[1] - dws, 2);
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
    lineNEW(MIN1, grad[2] - dws, 4);
}

void f14() {
    d1 = gtf();
    //d1 = DoubleMarker(Color(4), Color(4)); //�������-------------------------------------------------------------------------------------------
}

void f15() {
    moveBC(speed, -320);
}

void f15a() {
    turn(speed, d90, 3);
    moveD(speedD, up);
    lineNEW(ONE, 50, 4);
}

void f15b() {
    turn(speed, d90, 0);
    line(speed, 50, 1);
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
    moveC(speed, 50);
    moveB(speed, 40);
    moveBC(speed, 410, 0);
    stopC();
    moveB(speed, 850, 1);
    c_cubes += 2;
    turn(speed, 65, -1);
    moveA(0);
    getRGB(4);
    wait(100);
    /*if (getRGB(4).r > 15 || getRGB(4).g > 15) {
        gdeb = 4;
    }*/
    
    write(1, 1, gdeb);
    moveBC(speed, 425, 0);
    while (s2() > black);
    stopB();
    moveC(speed, 370);
    line(speed, 300, 3);
}

void f26() {
    turn(speed, d180, 2);
    line(speed, grad[2] - 2 * dws, 1);
}

void f28() {
    moveA(1);
    moveBCNEW(MIN, 340, 1);
    c_cubes += 2;
    moveA(0);
}

void f29() {
    moveBC(speed, -340, 1);
}

void f30() {
    turn(speed, d90 + 30, -1);
    goBC(speed);
    while (s2() > black);
    moveBC(speed, dws, 1);
}

void f31() {
    turn(speed, d90 + 40, 3);
    lineNEW(ONE, 200, 4);
}

void f32() {
    d2 = gtf();
    //d2 = DoubleMarker(Color(3), Color(3)); //�������-------------------------------------------------------------------
}

void f33() {
    moveBC(speed, -20, 1);
    turn(speed, d90, 0);
    getRGB(4);
    wait(100);
    /*if (getRGB(4).r > 15 || getRGB(4).g > 15) {
        gdeb = 2;
    }*/
    moveB(speed, turn1wheel, 1);
    moveBC(-speed, 30);
    goBC(speed);
    while (s3() > black);
    moveBC(speed, dws, 1);
}

void f34() {
    turn(speed, d90, 0);
    lineNEW(ONE, 300, 4);
}

void f35() {
    turn(speed, d180, 2);
    line(speed, grad[10] - dws * 2, 1);
}

void f36() {
    turn(speed, d90, 3);
    line(speed, 200, 1);
}

void f37() {
   d3 = gtf();
   //d3 = DoubleMarker(Color(2), Color(0)); //�������----------------------------------------------------------------------------------------
}

void f38() {
    moveBC(speed, 40);
    turn(speed, d90, 3);
}

void f39() {
    moveBC(speed, 50, 0);
    lineNEW(ONE, 460, 5);
    goBC(speed);
    while (s2() > black);
}

void f40() {
    wait(30);
    moveBC(15, -50);
    moveA(1);
    line(10, 120, 0);
    moveBC(speed, 420);
    moveA(0);
}

void f41() {
    turn(speed, d90, 3);
    moveBC(speed, 460, 0);
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
    moveBC(speed, dws - 50);
}

void f45() {
    turn(speed, d90 - 50, 0);
    turn(speed, 80, -1);
}

void f46() {
    lineNEW(ONE, grad[6] - dws + 20, 8);
    getRGB(2);
    moveBC(speed, 55, 0);
    while (getRGB(2).b < 100);
    moveBC(speed, 80, 1);
    s2();
}

void f47() {
    line(speed, 290, 0);
    stopBC();
    turn(speed, d90, 3);
}

void f48() {
    moveA(1);
    moveBCNEW(MIN, 540);
    moveA(0);
}

void f49() {
    turn(speed, d90, 3);
    moveBC(speed, 140, 0);
    while (s2() > black);
    moveBC(speed, dws, 1);
}

void f50() {
    turn(speed, d90, 0);
    line(speed, 290, 3);
}

void f51() {
    turn(speed, d90, 3);
    line(speed, 390, 3);
}

void f52() {
    stopBC();
    moveA(0);
    moveBC(-10, 70);
    moveD(speedD, before_take_cubes);
    moveA(1);
    moveBC(-10, 50);
    moveA(0);
    moveBC(-10, 40);
    moveD(speedD, after_take_cubes);
    moveA(1);
    moveBC(speed, 440);
    moveD(speedD, before_take_cubes);
}

void f53() {
    stopBC();
    moveA(0);
    moveBC(-10, 70);
    moveD(speedD, before_take_cubes);
    moveA(1);
    moveBC(-10, 50);
    moveA(0);
    moveBC(-10, 40);
    moveD(speedD, after_take_cubes);
    moveA(1);
    moveBC(speed, 440 - dws);
    moveD(speedD, before_take_cubes);
}

void f54() {
    moveBC(-speed, 280 - dws);
    moveD(speedD, up);
}

void f55() {
    moveBC(-speed, 110);
    turn(speed, d90, 0);
}

void f56() {
    moveA(1);
    moveBCNEW(MIN, 520);
    moveA(0);
}

void f57() {
    moveBC(-speed, 580);
}

void f58() {
    turn(speed, d90, 3);
    line(speed, 110, 3);
}

void f59() {
    turn(speed, d90, 0);
    line(speed, 490, 3);
}

void f60() {
    stopBC();
    moveBC(-speed, 50);
    turn(speed, d90, 0);
}

void f61() {
    stopBC();
    turn(speed, d90, 3);
}

void f62() {
    moveBC(-speed, 150);
    moveD(10, before_take_green_loops);
    moveBC(10, 80);
    moveD(10, after_take_green_loops);
}

void f63() {
    moveBC(speed, 70);
}

void f64() {
    turn(speed, d90, 3);
    line(speed, 50, 2);
}

void f65() {
    turn(speed, d90, 0);
}

void f66() {
    stopBC();
    moveBC(-speed, 50);
    turn(speed, d90, 3);
}

void f67() {
    line(speed, 210, 0);
    stopBC();
    turn(speed, d90, 0);
}

void f68() {
    moveBC(-speed, 150);
    moveD(10, before_take_green_loops);
    moveBC(10, 80);
    moveD(10, after_take_green_loops);
}

void f69() {
    moveBC(speed, 70);
}

void f70() {
    turn(speed, d90, 0);
    line(speed, 50, 3);
}

void f71() {
    turn(speed, d90, 3);
    line(speed, 700, 3);
}

void f72(){
    line(speed, grad[2] - dtw, 4);
}

void f73() {
    line(speed, grad[3] - dtw, 3);
}

void f74() {
    line(speed, grad[10] - dtw, 4);
}

void f75() {
    line(speed, grad[9] - dtw, 3);
}

void f76() {
    line(speed, grad[11] - dtw, 2);
}

void f77() {
    line(speed, grad[12] - dtw, 4);
}

void f78() {
    line(speed, grad[11] - dtw, 2);
}

void twb() {
    stopBC();
    moveBC(-speed, 50);
    moveB(speed, turn1wheel - 180, 0);
    while (s3() > black);
    moveB(speed, 100);
}

void twc() {
    stopBC();
    moveBC(-speed, 50);
    moveC(speed, turn1wheel - 180, 0);
    while (s2() > black);
    moveC(speed, 100);
}

void f79() {
    turn(speed, 30, -1);
    goBC(speed);
    while (s2() > black);
    moveBC(speed, dws, 1);
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
        g[v].pb(Edge(v + 2, f5));
        g[v + 1].pb(Edge(v + 2, f6));
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
    g[v + 8].pb(Edge(v + 6, f7, 0.9));
    g[v + 10].pb(Edge(v, f8));
    g[v + 10].pb(Edge(v + 4, f7, 0.9));
    g[v + 11].pb(Edge(v + 1, f8));
    g[v + 11].pb(Edge(v + 5, f7, 0.9));
    g[v + 9].pb(Edge(v + 3, f8));
    g[v + 9].pb(Edge(v + 7, f7, 0.9));
}

void put_loops_4(int clr) {
    if (d3.left == clr && d3.right == clr) {
        go(speed, nv, 121);
        unloading_loops(4);
        c_loops -= 4;
        nv = 121;
    } else if (d3.left == clr || d3.right == clr) {
        go(speed, nv, 121);
        unloading_loops(2);
        c_loops -= 2;
        nv = 121;
    }
    if (d2.left == clr && d2.right == clr) {
        go(speed, nv, 116);
        unloading_loops(4);
        c_loops -= 4;
        nv = 116;
    } else if (d2.left == clr || d2.right == clr) {
        go(speed, nv, 116);
        if (c_loops == 2)
            unloading_loops(4);
        else
            unloading_loops(2);
        c_loops -= 2;
        nv = 116;
    }
    if (d1.left == clr && d1.right == clr) {
        go(speed, nv, 25);
        unloading_loops(4);
        c_loops -= 4;
        nv = 25;
    } else if (d1.left == clr || d1.right == clr) {
        go(speed, nv, 25);
        if (c_loops == 2)
            unloading_loops(4);
        else
            unloading_loops(2);
        c_loops -= 2;
        nv = 25;
    }
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

    add(51, 116, f228);

    add(15, 25, f13);

    add(25, 26, f14);

    add(26, 120, f15);
    add(120, 25, f15a);
    add(120, 23, f15b);

    add(150, 23, f26);

    add(27, 109, f16);

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

    add(151, 59, f35);
    add(119, 59, f36);

    add(65, 121, f13);

    add(121, 122, f37);
    add(122, 123, f15);
    add(123, 121, f15a);
    add(123, 73, f15b);
    add(152, 73, f26a);

    add(86, 75, f38);
    add(75, 97, f39);
    add(97, 87, f8);
    add(87, 90, f1);
    add(90, 124, f40);
    add(124, 125, f41);
    add(125, 60, f42);
    add(125, 74, f43);

    add(109, 99, f44);
    add(99, 100, f45);
    add(99, 102, f45x);
    add(108, 102, f45y);
    add(110, 100, f45z);
    add(100, 112, f46);

    add(51, 126, f47);
    add(126, 127, f48);
    add(127, 128, f49);
    add(128, 48, f50);
    add(128, 62, f51);

    add(59, 131, f52);
    add(53, 131, f53);
    add(131, 53, f54);

    add(22, 129, f55);
    add(129, 130, f56);
    add(130, 129, f57);

    add(129, 22, f58);
    add(129, 38, f59);

    add(50, 132, f60);
    add(36, 132, f61);
    add(132, 133, f62);
    add(133, 132, f63);
    add(132, 50, f64);
    add(132, 36, f65);
    add(48, 134, f66);
    add(44, 134, f67);
    add(134, 135, f68);
    add(135, 134, f69);
    add(134, 48, f70);
    add(134, 62, f71);

    /*add(23, 136, twb);
    add(137, 25, f72);
    add(136, 38, f73);

    add(60, 138, twb);
    add(59, 139, twc);
    add(62, 138, twc);
    add(59, 140, twb);
    add(138, 116, f74);
    add(140, 48, f75);
    add(139, 74, f76);

    add(73, 142, twc);
    add(141, 121, f77);
    add(142, 60, f78);*/

    add(112, 115, f79);
    add(0, 9, f80);
    add(130, 146, f81);
    add(146, 109, f82);
    add(146, 35, f83);
    add(27, 144, f84);
    add(101, 144, f85);
    add(144, 145, f86);
    add(145, 147, f87);
    add(147, 22, f88);
    add(147, 38, f89);
    add(41, 137, f90);
    add(137, 41, f91);
    add(40, 139, f92);
    add(139, 140, f93);
    add(140, 141, f94);
    add(141, 116, f95);
    add(141, 59, f96);
    add(54, 139, f97);
    add(54, 48, f98);
    add(40, 62, f99);   
    add(52, 142, f100);
    add(142, 143, f101);
    add(143, 138, f102);
    add(143, 147, f103);
    add(147, 116, f104);
    add(147, 59, f105);
    add(51, 148, f106);
    add(148, 149, f107);
    add(149, 138, f108);
    add(149, 125, f109);
    add(25, 150, f110);
    add(116, 151, f111);
    add(121, 152, f112);
    add(151, 153, f113);
    add(153, 154, f114);
    add(154, 113, f115);
    add(154, 108, f116);
    add(102, 155, f117);
    add(155, 156, f118);
    add(156, 155, f119);
    add(155, 110, f120);
}

void vivod_h() {
    while (10000) {
        Clear_Display();
        write(10, 10, getHSV(4).    h);
        wait(50);
    }
}

void t2c() {
    goD(10);
    wait(1000);
    stopD();
    while (1) {
        isend = 0;
        moveBC(speed, dsl, 0);
        line(speed, 0, 10);
        if (isend == 0)
            break;
    }
    stopBC();
    turn(speed, d180, 0);
    line(speed, 0, 10);
    moveBC(speed, dws, 0);
    turn(speed, d90, 0);
    int c = 0;
    vector<int> degs;
    while (1) {
        isend = 0;
        moveBC(speed, dsl, 0);
        line(speed, 0, 10);
        degs.push_back(get_deg_line_B);
        c++;
        if (isend == 0)
            break;
    }
    stopBC();
    int v = 0;
    if (degs[c - 1] > 1500)
        v = 2;
    else if (c == 1)
        v = 0;
    else
        v = 6;
    write(10, 10, v);
    if (v == 0) {
        turn(speed, d180, 0);
        line(speed, 0, 10);
        moveBC(speed, dws);
        turn(speed, d90, 0);
        line(speed, 0, 10);
        moveBC(speed, dsl, 0);
        line(speed, 0, 10);
        stopBC();
    }
    else if (v == 2) {
        turn(speed, d180, 0);
        line(speed, 0, 10);
        moveBC(speed, dws);
        turn(speed, d90, 0);
        line(speed, 0, 10);
        moveBC(speed, dsl, 0);
        line(speed, 0, 10);
        moveBC(speed, dws);
        turn(speed, d90, 0);
        line(speed, 0, 10);
        moveBC(speed, dws);
        turn(speed, d90, 3);
        line(speed, 0, 10);
        stopBC();
    }
    moveD(speedD, 0);
    turn(speed, d180, 0);
    line(speed, 0, 10);
    moveBC(speed, dsl, 0);
    line(speed, 0, 10);
    moveBC(speed, dsl, 0);
    line(speed, 0, 10);
    turn(speed, d180, 0);
    moveBC(-speed, 110);
    moveD(speedD, 337);
    line(speed, 0, 10);
    moveBC(speed, dws);
    turn(speed, d90, 0);
    line(speed, 0, 10);
    moveBC(-speed, 120);
    turn(speed, d180, 0);
    moveD(speedD, 0);
    moveBC(-speed, 230);
    moveD(speedD, 337);
    line(speed, 0, 10);
    moveBC(speed, dsl, 0);
    line(speed, 0, 10);
    moveBC(speed, dws);
    turn(speed, d90, 3);
    line(speed, 0, 10);
    moveBC(speed, dsl, 0);
    line(speed, 0, 10);
    moveBC(-speed, 120);
    turn(speed, d180, 0);
    moveD(speedD, 0);
    moveBC(-speed, 230);
    moveD(speedD, 337);
    line(speed, 0, 10);
    moveBC(speed, dws);
    turn(speed, d90, 0);
    line(speed, 0, 10);
    moveBC(-speed, 120);
    turn(speed, d180, 0);
    moveD(speedD, 0);
    moveBC(-speed, 230);
    moveD(speedD, 337);
    line(speed, 0, 10);
    moveBC(speed, dsl, -1);
    line(speed, 0, 10);
    moveBC(speed, dws);
    turn(speed, d90, 3);
    line(speed, 0, 10);
    moveBC(-speed, 120);
    turn(speed, d180, 0);
    moveD(speedD, 0);
    moveBC(-speed, 230);
    moveD(speedD, 337);
    line(speed, 0, 10);
    moveBC(speed, dsl, 0);
    line(speed, 0, 10);
    moveBC(speed, dsl, 0);
    line(speed, 0, 10);
    moveBC(-speed, 120);
    turn(speed, d180, 0);
    moveD(speedD, 0);
    moveBC(-speed, 230);
    moveD(speedD, 337);
    line(speed, 0, 10);
    moveBC(speed, dws);
    turn(speed, d90, 3);
    line(speed, 0, 10);
    moveBC(speed, dws);
    turn(speed, d90, 3);
    line(speed, 0, 10);
    moveBC(speed, dws);
    turn(speed, d90, 3);
    line(speed, 0, 10);
    turn(speed, d180, 3);
    moveD(speedD, 0);
    moveBC(-speed, 450);
    wait(10000);
}

signed EV3_main() {
    Clear_Display();
    CreateThread(okonchanie, 0);
    //s1();
    s2();
    s3();
    s4();
    wait(200);
    t2c();
    return 0;
    buildDegreesConstants();
    buildg();
    //vivod_h();
    s2();
    s3();
    gclr(4);
    goD(speedD);
    wait(950);
    goD(0);
    goA(20);
    wait(500);
    stopA();
    moveA(1);
    go(speed, 1, 26);
    wait(10000);
    go(speed, 0, 26);
    nv = 26;
    if (d1.left == 4 || d1.right == 4) {
        go(speed, 26, 25);
        stopBC();
        unloading_cubes(4);
        c_cubes -= 2;
        go(speed, 25, 114);
    } else {
        go(speed, 26, 114);
    }
    go(speed, 114, 117);
    go(speed, 117, 119);
    if (d1.left == 4 && d1.right == 4) {
        c_cubes -= 2;
        go(speed, 119, 131);
        go(speed, 131, 122);
    } else if (d2.left == 4 && d2.right == 4) {
        go(speed, 119, 116);
        unloading_cubes(4);
        c_cubes -= 4;
        go(speed, 116, 122);
    } else if (d2.left == 4 || d2.right == 4) {
        go(speed, 119, 116);
        if (c_cubes == 2)
            unloading_cubes(4);
        else {
            unloading_cubes(2);
        }
        c_cubes -= 2;
        go(speed, 116, 122);
    } else {
        go(speed, 119, 122);
    }
    go(speed, 122, 123);
    if (d3.left == 4 || d3.right == 4) {
        go(speed, 123, 121);
        if (c_cubes == 2) {
            unloading_cubes(4);
        } else {
            unloading_cubes(2);
        }
        c_cubes -= 2;
        nv = 121;
    } else {
        nv = 123;
    }
    if (c_cubes == 2) {
        go(speed, nv, 131);
        c_cubes = 0;
        nv = 131;
    }
    gdeb = 3; //�����������������������������������-----------------------------------------------------------------------------------------
    int gg = 127;
    if (gdeb == 4)
        gg = 130;
    else if (gdeb == 2)
        gg = 124;
    go(speed, nv, gg);
    nv = gg;
    if (d1.left == 4 && d1.right == 4) {
        go(speed, nv, 25);
        unloading_cubes(2);
        nv = 25;
        if (d2.left == 0 || d2.right == 0) {
            go(speed, nv, 116);
            unloading_cubes(4);
            nv = 116;
        } else {
            go(speed, nv, 121);
            unloading_cubes(4);
            nv = 121;
        }
    } else {
        if (d1.left == 0 || d1.right == 0) {
            go(speed, nv, 25);
            unloading_cubes(2);
            nv = 25;
        } else if (d2.left == 0 || d2.right == 0) {
            go(speed, nv, 116);
            unloading_cubes(2);
            nv = 116;
        } else {
            go(speed, nv, 121);
            unloading_cubes(2);
            nv = 121;
        }
        moveA(0);
        turn(speed, d90, 3);
        moveA(1);
        moveBC(speed, 210);
        moveBC(-speed, 210);
        turn(speed, d90, 0);
    }
    int cc = 0;
    if (d1.left == 2)
        cc++;
    if (d1.right == 2)
        cc++;
    if (d2.left == 2)
        cc++;
    if (d2.right == 2)
        cc++;
    if (d3.left == 2)
        cc++;
    if (d3.right == 2)
        cc++;
    if (cc == 2) {
        go(speed, nv, 97);
        get_4_blue();
        nv = 66;
        put_loops_4(2);
        if (nv == 25) {
            go(speed, nv, 133);
            go(speed, 133, 135);
            nv = 135;
        } else {
            go(speed, nv, 135);
            go(speed, 135, 133);
            nv = 133;
        }
        c_loops += 4;
        put_loops_4(3);
    } else {
        if (nv == 25) {
            go(speed, nv, 133);
            go(speed, 133, 135);
            nv = 135;
        } else {
            go(speed, nv, 135);
            go(speed, 135, 133);
            nv = 133;
        }
        c_loops += 4;
        put_loops_4(3);
        go(speed, nv, 97);
        get_4_blue();
        nv = 66;
        put_loops_4(2);
    }
    return 0;
}