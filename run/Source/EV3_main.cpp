// docs https://sen04sen.github.io/LegoEV3Project1/html/index.html

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <set>
#include <functional>

#include <EV3_Motor.h>
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include "EV3_Thread.h"
#include "EV3_Bluetooth.h"
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"

#include "utils.h"
#include "motors.h"
#include "edge.h"
#include "sensors.h"
#include "turn.h"
#include "line.h"
#include "constants.h"
#include "field.h"
#include "control.h"


using namespace ev3_c_api;
using namespace std;

#define pb push_back

int go(int sp, int from, int toto) {
    pair<pair<double, int>, Edge> msgo[maxv];
    for (int i = 0; i < maxv; i++) {
        msgo[i].first.first = (double)1000000000;
        msgo[i].first.second = -1;
    }
    msgo[from].first.first = (double)0;
    set<pair<double, int> > st;
    st.insert(make_pair((double)0, from));
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

int diist(int from, int toto) {
    pair<pair<double, int>, Edge> msgo[maxv];
    for (int i = 0; i < maxv; i++) {
        msgo[i].first.first = (double)1000000000;
        msgo[i].first.second = -1;
    }
    msgo[from].first.first = (double)0;
    set<pair<double, int> > st;
    st.insert(make_pair((double)0, from));
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
    return way.size();
}

Field field = StandartInit();
DoubleMarker& d1 = field.house1;
DoubleMarker& d2 = field.house2;
DoubleMarker& d3 = field.house3;
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
    }
    else {
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
    }
    else {
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
    line(ONE, 150, 6);
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
    line(ONE, 370, 9);
    moveBC(speed, 35);
    moveB(speed, w90);
    speedD = lala;
}


DoubleMarker gtf() {
    moveBC(15, -140);
    gclr(4);
    turn(15, d90, 0);
    moveBC(speed, 185);
    wait(500);
    int fi = gclr(4);
    if (fi == 7)
        fi = 4;
    if (fi == 1)
        fi = 2;
    moveBC(speed, 120);
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


vector<int> min_way;
int mindist = 1000000000;
int vertoplaces[cntplaces];
int distplaces[cntplaces][cntplaces];

void buildplaces() {
    vertoplaces[0] = 7;
    vertoplaces[1] = 338;
    vertoplaces[2] = 339;
    vertoplaces[3] = 340;
    vertoplaces[4] = 206;
    vertoplaces[5] = 347;
    vertoplaces[6] = 348;
    vertoplaces[7] = 349;
    vertoplaces[8] = 350;
    vertoplaces[9] = 342;
    vertoplaces[10] = 238;
    vertoplaces[11] = 235;
    vertoplaces[12] = 232;
    vertoplaces[13] = 347;
    vertoplaces[14] = 260;
    vertoplaces[15] = 351;
    vertoplaces[16] = 352;
    vertoplaces[17] = 353;
    vertoplaces[18] = 354;
    vertoplaces[19] = 346;
    vertoplaces[20] = 344;
    vertoplaces[21] = 151;
    vertoplaces[22] = 148;
    vertoplaces[23] = 343;
    vertoplaces[24] = 67;
    vertoplaces[25] = 64;
    vertoplaces[26] = 44;
    vertoplaces[27] = 355;
    vertoplaces[28] = 356;
    vertoplaces[29] = 357;
    vertoplaces[30] = 358;
    vertoplaces[31] = 337;
}


void perebor(Field f, int ndist, vector<int> nway) {
    if (ndist > mindist)
        return;
    nway.pb(f.robot.now_position);
    if (f.robot.how_front < 4) {
        if (f.yellowA1 == 4) {
            int pref = f.robot.now_position;
            if (f.robot.how_front == 2) {
                f.robot.cfront2 = YELLOW;
            }
            else {
                f.robot.cfront1 = YELLOW;
            }
            f.robot.how_front += 2;
            f.yellowA1 = NONE;
            int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[1]);
            f.robot.now_position = 1;
            perebor(f, ndist + dt, nway);
            f.robot.how_front -= 2;
            f.yellowA1 = YELLOW;
            f.robot.now_position = pref;
        }
        if (f.yellowA3 == 4) {
            int pref = f.robot.now_position;
            if (f.robot.how_front == 2) {
                f.robot.cfront2 = YELLOW;
            }
            else {
                f.robot.cfront1 = YELLOW;
            }
            f.robot.how_front += 2;
            f.yellowA3 = NONE;
            int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[3]);
            f.robot.now_position = 3;
            perebor(f, ndist + dt, nway);
            f.robot.how_front -= 2;
            f.yellowA3 = YELLOW;
            f.robot.now_position = pref;
        }
    }
    if (f.robot.how_front == 0) {
        if (f.yellowB1 == 4) {
            int pref = f.robot.now_position;
            f.robot.how_front += 4;
            f.robot.cfront1 = YELLOW;
            f.robot.cfront2 = YELLOW;
            f.yellowB1 = NONE;
            int dt1 = diist(vertoplaces[f.robot.now_position], vertoplaces[24]);
            int dt2 = diist(vertoplaces[f.robot.now_position], vertoplaces[25]);
            if (dt1 > dt2) {
                f.robot.now_position = 25;
                perebor(f, ndist + dt2, nway);
            }
            else {
                f.robot.now_position = 24;
                perebor(f, ndist + dt1, nway);
            }
            f.yellowB1 = YELLOW;
            f.robot.how_front = 0;
            f.robot.now_position = pref;
        }
        if (f.greenB1 == 3) {
            int pref = f.robot.now_position;
            f.robot.how_front += 4;
            f.robot.cfront1 = GREEN;
            f.robot.cfront2 = GREEN;
            f.greenB1 = NONE;
            int dt1 = diist(vertoplaces[f.robot.now_position], vertoplaces[21]);
            int dt2 = diist(vertoplaces[f.robot.now_position], vertoplaces[22]);
            if (dt1 > dt2) {
                f.robot.now_position = 22;
                perebor(f, ndist + dt2, nway);
            }
            else {
                f.robot.now_position = 21;
                perebor(f, ndist + dt1, nway);
            }
            f.greenB1 = GREEN;
            f.robot.how_front = 0;
            f.robot.now_position = pref;
        }
        if (f.blueB1 == 2) {
            int pref = f.robot.now_position;
            f.robot.how_front += 4;
            f.robot.cfront1 = BLUE;
            f.robot.cfront2 = BLUE;
            f.blueB1 = NONE;
            int dt1 = diist(vertoplaces[f.robot.now_position], vertoplaces[10]);
            int dt2 = diist(vertoplaces[f.robot.now_position], vertoplaces[11]);
            if (dt1 > dt2) {
                f.robot.now_position = 10;
                perebor(f, ndist + dt2, nway);
            }
            else {
                f.robot.now_position = 11;
                perebor(f, ndist + dt1, nway);
            }
            f.blueB1 = BLUE;
            f.robot.how_front = 0;
            f.robot.now_position = pref;
        }
    }
    if (f.robot.how_front > 0) {
        if (f.cnt1 < 4) {
            if (f.robot.how_front == 2) {
                if (f.house1.left == f.robot.cfront1 || f.house1.right == f.robot.cfront1) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[28]);
                    f.robot.now_position = 28;
                    f.robot.how_front -= 2;
                    f.cnt1 += 2;
                    f.hom1.pb(f.robot.cfront1);
                    f.hom1.pb(f.robot.cfront1);
                    perebor(f, ndist + dt, nway);
                    f.cnt1 -= 2;
                    f.hom1.pop_back();
                    f.hom1.pop_back();
                    f.robot.how_front += 2;
                    f.robot.now_position = pref;
                }
            }
            else {
                if (f.house1.left == f.robot.cfront2 || f.house1.right == f.robot.cfront2) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[27]);
                    f.robot.now_position = 27;
                    f.robot.how_front -= 2;
                    f.cnt1 += 2;
                    f.hom1.pb(f.robot.cfront2);
                    f.hom1.pb(f.robot.cfront2);
                    perebor(f, ndist + dt, nway);
                    f.cnt1 -= 2;
                    f.hom1.pop_back();
                    f.hom1.pop_back();
                    f.robot.how_front += 2;
                    f.robot.now_position = pref;
                    if ((f.house1.left == f.robot.cfront1 || f.house1.right == f.robot.cfront1) && f.cnt1 == 0) {
                        int pref = f.robot.now_position;
                        int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[28]);
                        f.robot.now_position = 28;
                        f.robot.how_front -= 4;
                        f.cnt1 += 4;
                        f.hom1.pb(f.robot.cfront1);
                        f.hom1.pb(f.robot.cfront1);
                        f.hom1.pb(f.robot.cfront2);
                        f.hom1.pb(f.robot.cfront2);
                        perebor(f, ndist + dt, nway);
                        f.cnt1 -= 4;
                        f.hom1.pop_back();
                        f.hom1.pop_back();
                        f.hom1.pop_back();
                        f.hom1.pop_back();
                        f.robot.how_front += 4;
                        f.robot.now_position = pref;
                    }
                }
            }
        }
        if (f.cnt2 < 4) {
            if (f.robot.how_front == 2) {
                if (f.house2.left == f.robot.cfront1 || f.house2.right == f.robot.cfront1) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[6]);
                    f.robot.now_position = 6;
                    f.robot.how_front -= 2;
                    f.cnt2 += 2;
                    f.hom2.pb(f.robot.cfront1);
                    f.hom2.pb(f.robot.cfront1);
                    perebor(f, ndist + dt, nway);
                    f.cnt2 -= 2;
                    f.hom2.pop_back();
                    f.hom2.pop_back();
                    f.robot.how_front += 2;
                    f.robot.now_position = pref;
                }
            }
            else {
                if (f.house2.left == f.robot.cfront2 || f.house2.right == f.robot.cfront2) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[5]);
                    f.robot.now_position = 5;
                    f.robot.how_front -= 2;
                    f.cnt2 += 2;
                    f.hom2.pb(f.robot.cfront2);
                    f.hom2.pb(f.robot.cfront2);
                    perebor(f, ndist + dt, nway);
                    f.cnt2 -= 2;
                    f.hom2.pop_back();
                    f.hom2.pop_back();
                    f.robot.how_front += 2;
                    f.robot.now_position = pref;
                    if ((f.house2.left == f.robot.cfront1 || f.house2.right == f.robot.cfront1) && f.cnt2 == 0) {
                        int pref = f.robot.now_position;
                        int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[6]);
                        f.robot.now_position = 6;
                        f.robot.how_front -= 4;
                        f.cnt2 += 4;
                        f.hom2.pb(f.robot.cfront1);
                        f.hom2.pb(f.robot.cfront1);
                        f.hom2.pb(f.robot.cfront2);
                        f.hom2.pb(f.robot.cfront2);
                        perebor(f, ndist + dt, nway);
                        f.cnt2 -= 4;
                        f.hom2.pop_back();
                        f.hom2.pop_back();
                        f.hom2.pop_back();
                        f.hom2.pop_back();
                        f.robot.how_front += 4;
                        f.robot.now_position = pref;
                    }
                }
            }
        }
        if (f.cnt3 < 4) {
            if (f.robot.how_front == 2) {
                if (f.house3.left == f.robot.cfront1 || f.house3.right == f.robot.cfront1) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[16]);
                    f.robot.now_position = 16;
                    f.robot.how_front -= 2;
                    f.cnt3 += 2;
                    f.hom3.pb(f.robot.cfront1);
                    f.hom3.pb(f.robot.cfront1);
                    perebor(f, ndist + dt, nway);
                    f.cnt3 -= 2;
                    f.hom3.pop_back();
                    f.hom3.pop_back();
                    f.robot.how_front += 2;
                    f.robot.now_position = pref;
                }
            }
            else {
                if (f.house3.left == f.robot.cfront2 || f.house3.right == f.robot.cfront2) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[15]);
                    f.robot.now_position = 15;
                    f.robot.how_front -= 2;
                    f.cnt3 += 2;
                    f.hom3.pb(f.robot.cfront2);
                    f.hom3.pb(f.robot.cfront2);
                    perebor(f, ndist + dt, nway);
                    f.cnt3 -= 2;
                    f.hom3.pop_back();
                    f.hom3.pop_back();
                    f.robot.how_front += 2;
                    f.robot.now_position = pref;
                    if ((f.house3.left == f.robot.cfront1 || f.house3.right == f.robot.cfront1) && f.cnt3 == 0) {
                        int pref = f.robot.now_position;
                        int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[16]);
                        f.robot.now_position = 16;
                        f.robot.how_front -= 4;
                        f.cnt3 += 4;
                        f.hom3.pb(f.robot.cfront1);
                        f.hom3.pb(f.robot.cfront1);
                        f.hom3.pb(f.robot.cfront2);
                        f.hom3.pb(f.robot.cfront2);
                        perebor(f, ndist + dt, nway);
                        f.cnt3 -= 4;
                        f.hom3.pop_back();
                        f.hom3.pop_back();
                        f.hom3.pop_back();
                        f.hom3.pop_back();
                        f.robot.how_front += 4;
                        f.robot.now_position = pref;
                    }
                }
            }
        }
    }
    if (f.robot.how_back == 0) {
        if (f.blueA1 == 2) {
            int pref = f.robot.now_position;
            f.robot.how_back += 4;
            f.robot.cfront1 = BLUE;
            f.robot.cfront2 = BLUE;
            f.blueA1 = NONE;
            int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[13]);
            f.robot.now_position = 13;
            perebor(f, ndist + dt, nway);
            f.blueA1 = BLUE;
            f.robot.how_back = 0;
            f.robot.now_position = pref;
        }
        if (f.greenA1 == 3) {
            int pref = f.robot.now_position;
            f.robot.how_back += 4;
            f.robot.cfront1 = GREEN;
            f.robot.cfront2 = GREEN;
            f.greenA1 = NONE;
            int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[23]);
            f.robot.now_position = 23;
            perebor(f, ndist + dt, nway);
            f.greenA1 = GREEN;
            f.robot.how_back = 0;
            f.robot.now_position = pref;
        }
    }
    if (f.robot.how_back > 0) {
        if (f.cnt1 < 4) {
            if (f.robot.how_back == 2) {
                if (f.house1.left == f.robot.cback2 || f.house1.right == f.robot.cback2) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[26]);
                    f.robot.now_position = 26;
                    f.robot.how_back -= 2;
                    f.cnt1 += 2;
                    f.hom1.pb(f.robot.cback2);
                    f.hom1.pb(f.robot.cback2);
                    perebor(f, ndist + dt, nway);
                    f.cnt1 -= 2;
                    f.hom1.pop_back();
                    f.hom1.pop_back();
                    f.robot.how_back += 2;
                    f.robot.now_position = pref;
                }
            }
            else {
                if (f.house1.left == f.robot.cback1 || f.house1.right == f.robot.cback1) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[29]);
                    f.robot.now_position = 29;
                    f.robot.how_back -= 2;
                    f.cnt1 += 2;
                    f.hom1.pb(f.robot.cback1);
                    f.hom1.pb(f.robot.cback1);
                    perebor(f, ndist + dt, nway);
                    f.cnt1 -= 2;
                    f.hom1.pop_back();
                    f.hom1.pop_back();
                    f.robot.how_back += 2;
                    f.robot.now_position = pref;
                    if ((f.house1.left == f.robot.cback2 || f.house1.right == f.robot.cback2) && f.cnt1 == 0) {
                        int pref = f.robot.now_position;
                        int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[30]);
                        f.robot.now_position = 30;
                        f.robot.how_back -= 4;
                        f.cnt1 += 4;
                        f.hom1.pb(f.robot.cback1);
                        f.hom1.pb(f.robot.cback1);
                        f.hom1.pb(f.robot.cback2);
                        f.hom1.pb(f.robot.cback2);
                        perebor(f, ndist + dt, nway);
                        f.cnt1 -= 4;
                        f.hom1.pop_back();
                        f.hom1.pop_back();
                        f.hom1.pop_back();
                        f.hom1.pop_back();
                        f.robot.how_back += 4;
                        f.robot.now_position = pref;
                    }
                }
            }
        }
        if (f.cnt2 < 4) {
            if (f.robot.how_back == 2) {
                if (f.house2.left == f.robot.cback2 || f.house2.right == f.robot.cback2) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[4]);
                    f.robot.now_position = 4;
                    f.robot.how_back -= 2;
                    f.cnt2 += 2;
                    f.hom2.pb(f.robot.cback2);
                    f.hom2.pb(f.robot.cback2);
                    perebor(f, ndist + dt, nway);
                    f.cnt2 -= 2;
                    f.hom2.pop_back();
                    f.hom2.pop_back();
                    f.robot.how_back += 2;
                    f.robot.now_position = pref;
                }
            }
            else {
                if (f.house2.left == f.robot.cback1 || f.house2.right == f.robot.cback1) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[7]);
                    f.robot.now_position = 7;
                    f.robot.how_back -= 2;
                    f.cnt2 += 2;
                    f.hom2.pb(f.robot.cback1);
                    f.hom2.pb(f.robot.cback1);
                    perebor(f, ndist + dt, nway);
                    f.cnt2 -= 2;
                    f.hom2.pop_back();
                    f.hom2.pop_back();
                    f.robot.how_back += 2;
                    f.robot.now_position = pref;
                    if ((f.house2.left == f.robot.cback2 || f.house2.right == f.robot.cback2) && f.cnt2 == 0) {
                        int pref = f.robot.now_position;
                        int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[8]);
                        f.robot.now_position = 8;
                        f.robot.how_back -= 4;
                        f.cnt2 += 4;
                        f.hom2.pb(f.robot.cback1);
                        f.hom2.pb(f.robot.cback1);
                        f.hom2.pb(f.robot.cback2);
                        f.hom2.pb(f.robot.cback2);
                        perebor(f, ndist + dt, nway);
                        f.cnt2 -= 4;
                        f.hom2.pop_back();
                        f.hom2.pop_back();
                        f.hom2.pop_back();
                        f.hom2.pop_back();
                        f.robot.how_back += 4;
                        f.robot.now_position = pref;
                    }
                }
            }
        }
        if (f.cnt3 < 4) {
            if (f.robot.how_back == 2) {
                if (f.house3.left == f.robot.cback2 || f.house3.right == f.robot.cback2) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[16]);
                    f.robot.now_position = 14;
                    f.robot.how_back -= 2;
                    f.cnt3 += 2;
                    f.hom3.pb(f.robot.cback2);
                    f.hom3.pb(f.robot.cback2);
                    perebor(f, ndist + dt, nway);
                    f.cnt3 -= 2;
                    f.hom3.pop_back();
                    f.hom3.pop_back();
                    f.robot.how_back += 2;
                    f.robot.now_position = pref;
                }
            }
            else {
                if (f.house3.left == f.robot.cback1 || f.house3.right == f.robot.cback1) {
                    int pref = f.robot.now_position;
                    int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[17]);
                    f.robot.now_position = 17;
                    f.robot.how_back -= 2;
                    f.cnt3 += 2;
                    f.hom3.pb(f.robot.cback1);
                    f.hom3.pb(f.robot.cback1);
                    perebor(f, ndist + dt, nway);
                    f.cnt3 -= 2;
                    f.hom3.pop_back();
                    f.hom3.pop_back();
                    f.robot.how_back += 2;
                    f.robot.now_position = pref;
                    if ((f.house3.left == f.robot.cback2 || f.house3.right == f.robot.cback2) && f.cnt3 == 0) {
                        int pref = f.robot.now_position;
                        int dt = diist(vertoplaces[f.robot.now_position], vertoplaces[18]);
                        f.robot.now_position = 18;
                        f.robot.how_back -= 4;
                        f.cnt3 += 4;
                        f.hom3.pb(f.robot.cback1);
                        f.hom3.pb(f.robot.cback1);
                        f.hom3.pb(f.robot.cback2);
                        f.hom3.pb(f.robot.cback2);
                        perebor(f, ndist + dt, nway);
                        f.cnt3 -= 4;
                        f.hom3.pop_back();
                        f.hom3.pop_back();
                        f.hom3.pop_back();
                        f.hom3.pop_back();
                        f.robot.how_back += 4;
                        f.robot.now_position = pref;
                    }
                }
            }
        }
    }
    if (f.robot.how_front == 2) {
        if (f.cntutils < 4) {

        }
    }
}

void f1d() {
    moveBC(speed, dws, 1);
}

void f1l() {
    moveBC(speed, dsl, 0);
    line(speed, dws - dsl, 0);
}

void f2d() {
    turn(speed, d90, -1);
}

void f2l() {
    turn(speed, d90, 0);
}

void f3d() {
    turn(speed, d180, -1);
}

void f3l() {
    turn(speed, d180, 0);
}

void f4d() {
    turn(speed, d90, -2);
}

void f4l() {
    turn(speed, d90, 3);
}

void f5d() {
    turn(speed, d180, -2);
}

void f5l() {
    turn(speed, d180, 3);
}

void f6d() {
    moveBC(speed, 20, 0);
    t1w(speed, w90, -1);
}

void f6l() {
    moveBC(speed, 20, 0);
    t1w(speed, w90, 0);
}

void f7d() {
    moveBC(speed, 20, 0);
    t1w(speed, w90, -2);
}

void f7l() {
    moveBC(speed, 20, 0);
    t1w(speed, w90, 3);
}

void f8() {
    moveBC(speed, 150, 0);
    line(speed, grad[0] - 50, 3);
}

void f9() {
    line(speed, grad[1] - dws, 2);
}

void f10() {
    line(speed, grad[1] - dtw, 2);
    stopBC();
}

void f11() {
    line(speed, grad[3] - dws, 3);
}

void f12() {
    line(speed, grad[3] - dtw, 3);
}

void f13() {
    line(speed, grad[18] - dws, 0);
}

void f14() {
    line(speed, grad[18] - dtw + dws, 0);
}

void f15() {
    line(speed, grad[3] - grad[18] - dws, 0);
}

void f16() {
    line(speed, grad[3] - grad[18] - dtw, 0);
}

void f17() {
    line(speed, grad[2] - dws, 4);
}

void f18() {
    line(speed, grad[2] - dtw, 4);
}

void f19() {
    d1 = gtf();
}

void f20() {
    moveBC(speed, -320);
}

void f21() {
    turn(speed, d90, 3);
    line(speed, 50, 4);
}

void f22() {
    turn(speed, d90, 0);
    line(speed, 50, 1);
}

void f23() {
    line(speed, grad[7] - dws, 2);
}

void f24() {
    line(speed, grad[7] - dtw, 2);
}

void f25() {
    line(speed, grad[4] - dws, 1);
}

void f26() {
    line(speed, grad[4] - dtw, 1);
}

void take4green() {
    //moveA(speedA, 100);
    moveBC(-speed, 130);
    line(speed, grad[8] - dws + 130, 0);
    stopBC();
    //moveA(speedA, 200);
}

void f27() {
    moveBC(-speed, grad[8] - dws);
}

void f28() {
    line(speed, grad[19] - dws, 0);
}

void f29() {
    line(speed, grad[19] - dtw, 0);
}

void f30() {
    line(speed, grad[19] - dws, 1);
}

void f31() {
    line(speed, grad[19] - dtw, 1);
}

void f32() {
    line(speed, grad[18] - dws, 3);
}

void f33() {
    line(speed, grad[3] - grad[18] - dws, 3);
}

void f34() {
    line(speed, grad[3] - grad[18] - dtw, 3);
}

void f35() {
    line(speed, 200, 1);
}

void f36() {
    line(speed, grad[19] - dws, 1);
}

void f37() {
    line(speed, grad[19] - dtw, 1);
}

void f38() {
    line(speed, grad[4] - grad[19] - dws, 1);
}

void f39() {
    line(speed, grad[4] - grad[19] - dtw, 1);
}

void f40() {
    line(speed, grad[4] - grad[19] - dws, 0);
}

void f41() {
    line(speed, grad[4] - grad[19] - dtw, 0);
}

void f42() {
    line(speed, grad[7] - dws, 2);
}

void f43() {
    turn(speed, w90, -2);
    line(speed, dws - 60, 0);
}

void f44() {
    line(speed, grad[9] - dws, 3);
}

void f45() {
    line(speed, grad[9] - dtw, 3);
}

void f46() {
    stopBC();
    moveB(speed, 200);
    moveC(speed, 200);
    line(speed, grad[20] - dws - 200, 8);
}

void f47() {
    stopBC();
    moveB(speed, 200);
    moveC(speed, 200);
    line(speed, grad[20] - dtw - 200, 8);
}

void f48() {
    line(speed, 350 - dws, 10);
    stopBC();
    turn(speed, 70, -1);
    moveBC(speed, 90);
    turn(speed, 70, 3);
    line(speed, 40, 3);
}

void f49() {
    line(speed, 350 - dtw, 10);
    stopBC();
    turn(speed, 70, -1);
    moveBC(speed, 90);
    turn(speed, 70, 3);
    line(speed, 40, 3);
}

void f52() {
    stopBC();
    moveC(speed, 200);
    moveB(speed, 200);
    line(speed, grad[9] - grad[20] - dws - 200, 10);
}

void f53() {
    stopBC();
    moveC(speed, 200);
    moveB(speed, 200);
    line(speed, grad[9] - grad[20] - dtw - 200, 8);
}

void f50() {
    line(speed, 350 - dws, 8);
    stopBC();
    turn(speed, 70, -2);
    moveBC(speed, 90);
    turn(speed, 70, 0);
    line(speed, 40, 3);
}

void f51() {
    line(speed, 350 - dtw, 8);
    stopBC();
    turn(speed, 70, -2);
    moveBC(speed, 90);
    turn(speed, 70, 0);
    line(speed, 40, 3);
}

void f54() {
    line(speed, 260 - dws, 0);
}

void f55() {
    line(speed, 260 - dtw, 0);
}

void f56() {
    line(speed, 260 - dws, 1);
}

void f57() {
    line(speed, 260 - dtw, 1);
}

void f58() {
    line(speed, 650 - dws, 0);
}

void f59() {
    line(speed, 650 - dtw, 0);
}

void f60() {
    line(speed, 650 - dws, 1);
}

void f61() {
    line(speed, 650 - dtw, 1);
}

void f62() {
    line(speed, 750 - dws, 0);
}

void f63() {
    line(speed, 750 - dtw, 0);
}

void f64() {
    line(speed, 750 - dws, 1);
}

void f65() {
    line(speed, 750 - dtw, 1);
}

void f66() {
    line(speed, grad[10] - dws, 4);
}

void f67() {
    line(speed, grad[10] - dtw, 4);
}

void f68() {
    d2 = gtf();
}

void f69() {
    moveBC(speed, -20, 1);
    turn(speed, d90, 0);
    getRGB(4);
    wait(100);
    if (getRGB(4).r > 15 || getRGB(4).g > 15) {
        gdeb = 2;
    }
    moveB(speed, w90, 1);
    moveBC(-speed, 30);
    goBC(speed);
    while (s3() > black);
    moveBC(speed, dws, 1);
}

void f70() {

}

void f71() {

}

void f72() {

}

void f73() {
    moveBC(speed, 650, 0);
}

void f74() {
    line(speed, grad[11] - dws, 2);
}

void f75() {
    line(speed, grad[11] - dtw, 2);
}

void f76() {
    line(speed, grad[11] - dws, 2);
}

void f77() {
    line(speed, grad[11] - dtw, 2);
}

void f78() {
    line(speed, 650 - dws, 0);
}

void f79() {
    line(speed, 650 - dtw, 0);
}

void f80() {
    line(speed, 650 - dws, 2);
}

void f81() {
    line(speed, 650 - dtw, 2);
}

void f82() {
    line(speed, 230 - dws, 2);
}

void f83() {
    line(speed, 230 - dtw, 2);
}

void f84() {
    line(speed, 230 - dws, 0);
}

void f85() {
    line(speed, 230 - dtw, 0);
}

void f86() {
    line(speed, grad[12] - dws, 4);
}

void f87() {
    line(speed, grad[12] - dtw, 4);
}

void f88() {
    d3 = gtf();
}

void f89() {
    moveBC(speed, -320);
}

void f90() {
    line(speed, grad[11] - dws, 2);
}

void f91() {
    turn(speed, d90, 0);
    line(speed, 50, 1);
}

void f92() {
    line(speed, grad[13] - dws, 3);
}

void f93() {
    line(speed, 365 - dsl, 0);
}

void f94() {
    turn(speed, d90 - 40, 3);
    turn(speed, 40, -2);
}

void f95() {
    moveBC(speed, 50, 0);
    line(speed, 460, 5);
    goBC(speed);
    while (s2() > black);
}

void f96() {
    moveBC(speed, dws);
}

void f97() {
    turn(speed, d90, -1);
}

void f98() {
    turn(speed, d90, 3);
}

void get4blue1() {

}

void get4blue2() {

}

void f99() {
    moveBC(-speed, 430);
}

void f100() {
    moveBC(speed, 200, 0);
    while (s2() > black);
}

void f101() {
    moveBC(-speed, 590);
}

void f102() {
    moveBC(speed, 340, 0);
    while (s2() > black);
}

/*void f13() {
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
    line(ONE, 50, 4);
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

 /*   write(1, 1, gdeb);
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
    line(ONE, 200, 4);
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
 /*   moveB(speed, turn1wheel, 1);
    moveBC(-speed, 30);
    goBC(speed);
    while (s3() > black);
    moveBC(speed, dws, 1);
}

void f34() {
    turn(speed, d90, 0);
    line(ONE, 300, 4);
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
    line(ONE, grad[6] - dws + 20, 8);
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
    moveBC(MIN, 540);
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
    moveBC
    (MIN, 520);
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

void f72() {
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
}*/

void add(int from, int to, void (*def)(), double time = 1.0) {
    g[from].pb(Edge(to, def, time));
}

void addcrossroad(int v, int u, int r, int d, int l) {
    if (u == 1) {
        g[v + 2].pb(Edge(v + 1, f1l));
        g[v + 11].pb(Edge(v, f6l));
        g[v + 5].pb(Edge(v, f7l));
        g[v + 4].pb(Edge(v + 1, f4l));
        g[v + 10].pb(Edge(v + 1, f2l));
        g[v + 7].pb(Edge(v + 1, f3l));
    }
    else {
        g[v + 2].pb(Edge(v + 1, f1d));
        g[v + 11].pb(Edge(v, f6d));
        g[v + 5].pb(Edge(v, f7d));
        g[v + 4].pb(Edge(v + 1, f4d));
        g[v + 10].pb(Edge(v + 1, f2d));
        g[v + 7].pb(Edge(v + 1, f3d));
    }
    if (r == 1) {
        g[v + 5].pb(Edge(v + 4, f1l));
        g[v + 2].pb(Edge(v + 3, f6l));
        g[v + 8].pb(Edge(v + 3, f7l));
        g[v + 7].pb(Edge(v + 4, f4l));
        g[v + 10].pb(Edge(v + 4, f3l));
        g[v + 1].pb(Edge(v + 4, f2l));
    }
    else {
        g[v + 5].pb(Edge(v + 4, f1d));
        g[v + 2].pb(Edge(v + 3, f6d));
        g[v + 8].pb(Edge(v + 3, f7d));
        g[v + 7].pb(Edge(v + 4, f4d));
        g[v + 10].pb(Edge(v + 4, f3d));
        g[v + 1].pb(Edge(v + 4, f2d));
    }
    if (d == 1) {
        g[v + 8].pb(Edge(v + 7, f1l));
        g[v + 5].pb(Edge(v + 6, f6l));
        g[v + 11].pb(Edge(v + 6, f7l));
        g[v + 4].pb(Edge(v + 7, f2l));
        g[v + 1].pb(Edge(v + 7, f3l));
        g[v + 10].pb(Edge(v + 7, f4l));
    }
    else {
        g[v + 8].pb(Edge(v + 7, f1d));
        g[v + 5].pb(Edge(v + 6, f6d));
        g[v + 11].pb(Edge(v + 6, f7d));
        g[v + 4].pb(Edge(v + 7, f2d));
        g[v + 1].pb(Edge(v + 7, f3d));
        g[v + 10].pb(Edge(v + 7, f4d));
    }
    if (l == 1) {
        g[v + 11].pb(Edge(v + 10, f1l));
        g[v + 8].pb(Edge(v + 9, f6l));
        g[v + 2].pb(Edge(v + 9, f7l));
        g[v + 7].pb(Edge(v + 10, f2l));
        g[v + 4].pb(Edge(v + 10, f3l));
        g[v + 1].pb(Edge(v + 10, f4l));
    }
    else {
        g[v + 11].pb(Edge(v + 10, f1d));
        g[v + 8].pb(Edge(v + 9, f6d));
        g[v + 2].pb(Edge(v + 9, f7d));
        g[v + 7].pb(Edge(v + 10, f2d));
        g[v + 4].pb(Edge(v + 10, f3d));
        g[v + 1].pb(Edge(v + 10, f4d));
    }
}

/*void put_loops_4(int clr) {
    if (d3.left == clr && d3.right == clr) {
        go(speed, nv, 121);
        unloading_loops(4);
        c_loops -= 4;
        nv = 121;
    }
    else if (d3.left == clr || d3.right == clr) {
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
    }
    else if (d2.left == clr || d2.right == clr) {
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
    }
    else if (d1.left == clr || d1.right == clr) {
        go(speed, nv, 25);
        if (c_loops == 2)
            unloading_loops(4);
        else
            unloading_loops(2);
        c_loops -= 2;
        nv = 25;
    }
}*/

void buildg() {
    addcrossroad(0, 0, 0, 0, 0);
    addcrossroad(12, 1, 1, 0, 0);
    addcrossroad(24, 0, 1, 1, 1);
    addcrossroad(36, 1, 0, 0, 0);
    addcrossroad(48, 0, 1, 0, 1);
    addcrossroad(60, 0, 0, 0, 0);
    addcrossroad(72, 0, 1, 0, 0);
    addcrossroad(84, 0, 1, 1, 1);
    addcrossroad(96, 1, 0, 1, 0);
    addcrossroad(108, 1, 1, 0, 1);
    addcrossroad(120, 0, 1, 1, 1);
    addcrossroad(132, 0, 0, 0, 0);
    addcrossroad(144, 0, 0, 0, 0);
    addcrossroad(156, 0, 0, 0, 1);
    addcrossroad(168, 1, 1, 0, 1);
    addcrossroad(180, 1, 0, 1, 0);
    addcrossroad(192, 1, 0, 1, 0);
    addcrossroad(204, 0, 0, 1, 0);
    addcrossroad(216, 0, 1, 0, 1);
    addcrossroad(228, 0, 0, 0, 0);
    addcrossroad(240, 0, 1, 1, 1);
    addcrossroad(252, 1, 0, 0, 0);
    //addcrossroad(264, 1, 1, 0, 1);
    addcrossroad(288, 1, 0, 1, 0);
    addcrossroad(300, 1, 0, 1, 0);
    addcrossroad(312, 1, 0, 1, 0);
    addcrossroad(324, 1, 0, 1, 0);
    addcrossroad(348, 0, 1, 0, 1);

    add(7, 20, f8);

    add(16, 29, f9);
    add(15, 29, f10);
    add(34, 23, f9);
    add(33, 23, f10);

    add(28, 113, f11);
    add(27, 113, f12);
    add(118, 59, f11);
    add(117, 59, f12);

    add(28, 53, f13);
    add(27, 52, f14);
    add(58, 35, f32);

    add(118, 59, f15);
    add(117, 59, f16);
    add(52, 113, f33);
    add(51, 113, f34);

    add(31, 44, f17);
    add(30, 44, f18);

    add(337, 26, f35);

    add(44, 336, f19);

    add(336, 298, f20);
    add(295, 44, f21);
    add(289, 25, f22);

    add(112, 125, f23);
    add(111, 125, f24);
    add(130, 119, f23);
    add(129, 119, f24);

    add(109, 86, f25);
    add(108, 86, f26);
    add(91, 116, f25);
    add(90, 116, f26);

    add(109, 98, f28);
    add(108, 98, f29);
    add(103, 116, f36);
    add(102, 116, f37);

    add(97, 86, f38);
    add(96, 86, f39);
    add(91, 104, f40);
    add(90, 104, f41);

    add(112, 125, f42);
    add(116, 124, f43);
    add(130, 119, f42);
    //add(129, 119, f43);

    add(127, 343, take4green);
    add(343, 127, f27);

    add(124, 173, f44);
    add(123, 173, f45);
    add(178, 131, f44);
    add(177, 131, f45);

    add(124, 137, f46);
    add(123, 137, f47);
    add(142, 131, f48);
    add(141, 131, f49);

    add(136, 173, f50);
    add(135, 173, f51);
    add(178, 143, f52);
    add(177, 143, f53);

    add(169, 182, f54);
    add(168, 182, f55); 
    add(187, 176, f56);
    add(186, 176, f57);

    add(169, 194, f58);
    add(168, 194, f59); 
    add(199, 176, f60);
    add(198, 176, f61);

    add(169, 302, f62);
    add(168, 302, f63);
    add(307, 176, f64);
    add(306, 176, f65);

    add(169, 206, f66);
    add(168, 206, f67);

    add(206, 341, f68);

    add(341, 203, f69);

    add(342, 308, f70);

    add(342, 200, f71);

    add(342, 176, f72);

    add(160, 305, f73);

    add(172, 245, f74);
    add(171, 245, f75);
    add(250, 179, f76);
    add(249, 179, f77);

    add(172, 221, f78);
    add(171, 221, f79);
    add(226, 179, f80);
    add(225, 179, f81);

    add(220, 245, f82);
    add(219, 245, f83);
    add(250, 227, f84);
    add(249, 227, f85);

    add(247, 260, f86);
    add(246, 260, f87);

    add(260, 345, f88);
    
    add(345, 334, f89);

    add(346, 242, f90);

    add(325, 242, f91);

    add(244, 269, f92);

    add(269, 268, f93);

    add(268, 265, f94);
    add(265, 278, f95);

    add(278, 277, f96);

    add(277, 280, f97);
    add(277, 286, f98);

    add(280, 347, get4blue1);
    add(281, 347, get4blue2);

    add(55, 67, f99);
    add(67, 56, f100);

    add(100, 64, f101);
    add(64, 101, f102);

    /*add(280, 232, f101);

    add(235, 224, f102);
    add(238, 203, f103);
    add(223, 235, f104);
    add(202, 238, f105);

    add(79, 338, f106);
    add(338, 79, f107);
    add(163, 340, f108);
    add(340, 163, f109);

    add(139, 151, f110);
    add(151, 356, f111);
    add(184, 148, f112);
    add(148, 185, f113);*/



   /* add(15, 25, f13);

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

    /*add(112, 115, f79);
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
    add(155, 110, f120);*/
}

void vivod_h() {
    while (10000) {
        Clear_Display();
        write(10, 10, getHSV(4).h);
        wait(50);
    }
}


const int how = 100000;

int EV3_main()
{
    CreateThread(okonchanie, nullptr);
    CreateThread(control, nullptr);
    Clear_Display();
    while (true) {
        how_a = 50;
        wait(5000);
        how_a = 100;
        wait(3000);
    }
    return 0;
    line(MIN, 300, 4);
    moveBC(0, 125);
    stopBC();
    moveC(0, -590);
    stopBC();
    DoubleMarker a = read_home();
    moveBC(0, -530);
    stopBC();
    write(10, 10, int(a.left));
    write(40, 40, int(a.right));
    wait(5000);
    return 0;
    //s1();
    s2();
    s3();
    gclr(4);
    buildDegreesConstants();
    buildg();
    //vivod_h();
    /*goD(speedD);
    wait(950);
    goD(0);
    goA(20);
    wait(500);
    stopA();
    moveA(1);*/
    go(speed, 7, 67);
    go(speed, 67, 206);
    stopBC();
    return 0;
    go(speed, 0, 26);
    nv = 26;
    if (d1.left == 4 || d1.right == 4) {
        go(speed, 26, 25);
        stopBC();
        unloading_cubes(4);
        c_cubes -= 2;
        go(speed, 25, 114);
    }
    else {
        go(speed, 26, 114);
    }
    go(speed, 114, 117);
    go(speed, 117, 119);
    if (d1.left == 4 && d1.right == 4) {
        c_cubes -= 2;
        go(speed, 119, 131);
        go(speed, 131, 122);
    }
    else if (d2.left == 4 && d2.right == 4) {
        go(speed, 119, 116);
        unloading_cubes(4);
        c_cubes -= 4;
        go(speed, 116, 122);
    }
    else if (d2.left == 4 || d2.right == 4) {
        go(speed, 119, 116);
        if (c_cubes == 2)
            unloading_cubes(4);
        else {
            unloading_cubes(2);
        }
        c_cubes -= 2;
        go(speed, 116, 122);
    }
    else {
        go(speed, 119, 122);
    }
    go(speed, 122, 123);
    if (d3.left == 4 || d3.right == 4) {
        go(speed, 123, 121);
        if (c_cubes == 2) {
            unloading_cubes(4);
        }
        else {
            unloading_cubes(2);
        }
        c_cubes -= 2;
        nv = 121;
    }
    else {
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
        }
        else {
            go(speed, nv, 121);
            unloading_cubes(4);
            nv = 121;
        }
    }
    else {
        if (d1.left == 0 || d1.right == 0) {
            go(speed, nv, 25);
            unloading_cubes(2);
            nv = 25;
        }
        else if (d2.left == 0 || d2.right == 0) {
            go(speed, nv, 116);
            unloading_cubes(2);
            nv = 116;
        }
        else {
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
/*    if (cc == 2) {
        go(speed, nv, 97);
        get_4_blue();
        nv = 66;
        put_loops_4(2);
        if (nv == 25) {
            go(speed, nv, 133);
            go(speed, 133, 135);
            nv = 135;
        }
        else {
            go(speed, nv, 135);
            go(speed, 135, 133);
            nv = 133;
        }
        c_loops += 4;
        put_loops_4(3);
    }
    else {
        if (nv == 25) {
            go(speed, nv, 133);
            go(speed, 133, 135);
            nv = 135;
        }
        else {
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
    }*/
    return 0;
}