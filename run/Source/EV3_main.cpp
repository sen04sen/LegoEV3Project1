// docs https://sen04sen.github.io/LegoEV3Project1/html/index.html

//#define abs(x) ((x > 0) ? x : -x)

#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <queue>

#include <EV3_Motor.h>
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include "EV3_Thread.h"
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"

#define int short

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

int maxim = 0;

Field f = StandartInit();

/*int goDEIC(int sp, int from, int toto) {
    static pair<pair<double, int>, Edge> msgo[maxv];
    for (int i = 0; i < maxv; i++) {
        msgo[i].first.first = (double)1000000000;
        msgo[i].first.second = -1;
    }
    msgo[from].first.first = (double)0;
    set<pair<double, int> > st;
    st.insert(make_pair((double)0, from));
    bool end = 0;

    write(100, 100, 87);
    wait(4010);
    while (!st.empty()) {
        double dd = st.begin()->first;
        int v = st.begin()->second;
        st.erase(st.begin( ));

        for (int i = 0; i < g[v].size(); i++) {
            //write(50, 50, maxim);
            //maxim = max(int(st.size() * sizeof(pair<double, int>)), maxim);
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
    vector<pair<Edge, int> > way;
    int nv = toto;
    while (nv != from) {
        way.pb(make_pair(msgo[nv].second, nv ));
        nv = msgo[nv].first.second;
    }
    for (int i = way.size() - 1; i >= 0; i--) {
        Edge nw = way[i].first;
        nw(0);
    }
    return way.size();
}*/

int go(int sp, int from, int toto) {
    vector<int> father(maxv, -1);
    father[from] = 0;
    queue<int> q;
    q.push(from);
    while (!q.empty() && father[toto] == -1) {
        int v = q.front();
        q.pop();
        for (int i = 0; i < g[v].size(); ++i) {
            if (father[g[v][i].get_to()] == -1 && g[v][i].get_active()) {
                father[g[v][i].get_to()] = v;
                q.push(g[v][i].get_to());
            }
        }
    }
    if (father[toto] == -1) {
        Clear_Display();
        write(50, 50, "NO WAY");
        wait(5000);
        throw Exception("NO WAY");
    }
    vector<Edge*> way;
    int now = toto;
    while (now != from) {
        for (int i = 0; i < g[father[now]].size(); ++i)
            if (g[father[now]][i].get_to() == now)
                way.push_back(&g[father[now]][i]);
        now = father[now];
    }

    for (int i = way.size() - 1; i >= 0; i--) {

        (*way[i])(0);
    }
    return way.size();
}
/*int diist(int from, int toto) {
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
            //write(50, 50, maxim);
            //maxim = max(int(st.size() * sizeof(pair<double, int>)), maxim);
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
    vector<pair<Edge, int> > way;
    int nv = toto;
    if (msgo[nv].first.second == -1) {
        Clear_Display();
        write(1, 1, "NO WAY!!!");
        write(1, 40, from);
        write(40, 40, toto);
        wait(10000);
    }
    while (nv != from) {
        write(100, 100, 0);
        way.pb(make_pair(msgo[nv].second, nv));
        nv = msgo[nv].first.second;
    }
    return way.size();
}*/

int diist(int from, int toto) {
    vector<int> dist(maxv, maxv);
    dist[from] = 0;
    queue<int> q;
    q.push(from);
    while (!q.empty() && dist[toto] == maxv) {
        int v = q.front();
        q.pop();
        for (int i = 0; i < g[v].size(); ++i) {
            if (dist[g[v][i].get_to()] > dist[v] + 1 && g[v][i].get_active()) {
                dist[g[v][i].get_to()] = dist[v] + 1;
                q.push(g[v][i].get_to());
            }
        }
    }
    return dist[toto];
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
        moveBC(speed, -50);
        moveA(0);
        moveBC(speed, -120);
    }
}

void unloading_loops(int cnt) {
    stopBC();
    if (cnt == 4) {
        moveBC(speed, -250);
        moveD(20, near_put_loops);
        wait(300);
        moveD(speedD, put_loops);
        moveBC(speed, 80);
        moveBC(speed, -200);
        moveD(speedD, up_loops);
        moveA(0);
        moveBC(speed, 470);
        moveBC(speed, -100);
    }
    else {
        moveBC(speed, -250);
        moveD(20, near_put_loops);
        wait(300);
        moveD(speedD, xren_loops);
        moveBC(speed, -80);
        moveD(speedD, up_loops);
        moveA(0);
        moveBC(speed, -430);
        moveBC(speed, 100);
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
    gclr(4);
    moveBC(speed, -40);
    turn(speed, d90, -1);
    DoubleMarker a = read_home();
    stopBC();
    getRGB(4);
    return(a);
}


vector<int> min_way;
int mindist = 1000000001;
int vertoplaces[cntplaces];
int distplaces[cntplaces][cntplaces];

void buildplaces() {
    vertoplaces[0] = 1;
    vertoplaces[1] = 338;
    vertoplaces[2] = 339;
    vertoplaces[3] = 340;
    vertoplaces[4] = 206;
    vertoplaces[5] = 347;
    vertoplaces[6] = 348;
    vertoplaces[7] = 349;
    vertoplaces[8] = 350;
    vertoplaces[9] = 351;
    vertoplaces[10] = 238;
    vertoplaces[11] = 235;
    vertoplaces[12] = 232;
    vertoplaces[13] = 3000;
    vertoplaces[14] = 260;
    vertoplaces[15] = 352;
    vertoplaces[16] = 353;
    vertoplaces[17] = 354;
    vertoplaces[18] = 355;
    vertoplaces[19] = 356;
    vertoplaces[20] = 362;
    vertoplaces[21] = 151;
    vertoplaces[22] = 148;
    vertoplaces[23] = 343;
    vertoplaces[24] = 67;
    vertoplaces[25] = 64;
    vertoplaces[26] = 44;
    vertoplaces[27] = 357;
    vertoplaces[28] = 358;
    vertoplaces[29] = 359;
    vertoplaces[30] = 360;
    vertoplaces[31] = 361;
    vertoplaces[32] = 363;
    vertoplaces[33] = 364;
    vertoplaces[34] = 365;
    vertoplaces[35] = 366;
}

bool is_end(Field &f) {
    if (f.cnt1 == 4 && f.cnt2 == 4 && f.cnt3 == 4 && f.cntutils == 4) {
        bool m1 = 0, m2 = 0;
        bool bad = 0;
        for (int i = 0; i < 4; i++) {
            if (f.hom1[i] == f.house1.left)
                m1 = 1;
            if (f.hom1[i] == f.house1.right)
                m2 = 1;
            if (f.hom1[i] != f.B) {
                bad = 1;
                break;
            }
        }
        if (bad || !m1 || !m2)
            return 0;
        m1 = 0, m2 = 0;
        bad = 0;
        for (int i = 0; i < 4; i++) {
            if (f.hom2[i] == f.house2.left)
                m1 = 1;
            if (f.hom2[i] == f.house2.right)
                m2 = 1;
            if (f.hom2[i] != f.B) {
                bad = 1;
                break;
            }
        }
        if (bad || !m1 || !m2)
            return 0;
        m1 = 0, m2 = 0;
        bad = 0;
        for (int i = 0; i < 4; i++) {
            if (f.hom3[i] == f.house3.left)
                m1 = 1;
            if (f.hom3[i] == f.house3.right)
                m2 = 1;
            if (f.hom3[i] != f.B) {
                bad = 1;
                break;
            }
        }
        if (bad || !m1 || !m2)
            return 0;
        return 1;
    }
    else
        return 0;
}

int uyuyuy = 0;
/*
void perebor(Field &f, int ndist, vector<int> nway) {
    Clear_Display();
    write(1, 1, uyuyuy);
    wait(100);
    uyuyuy += 1;
    if (ndist > mindist)
        return;
    Clear_Display();
    write(1, 40, f.robot.now_position);
    write(40, 40, f.yellowA1);
    write(80, 40, f.yellowA3);
    write(120, 40, f.robot.how_front);
    wait(1000);
    Clear_Display();
    nway.pb(f.robot.now_position);
    if (is_end(f)) {
        write(1, 1, "WTF");
        wait(1000);
        int dt = diist(f.robot.now_position, vertoplaces[0]);
        if (mindist < dt + ndist) {
            nway.pb(vertoplaces[0]);
            min_way = nway;
            mindist = dt + ndist;
        }
        return;
    }
    write(1, 70, 1);
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
            int dt = diist(f.robot.now_position, vertoplaces[1]);
            f.robot.now_position = vertoplaces[1];
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
            int dt = diist(f.robot.now_position, vertoplaces[3]);
            f.robot.now_position = vertoplaces[3];
            perebor(f, ndist + dt, nway);
            f.robot.how_front -= 2;
            f.yellowA3 = YELLOW;
            f.robot.now_position = pref;
        }
    }
    write(1, 70, 2);
    if (f.robot.how_front == 0) {
        if (f.yellowB1 == 4) {
            int pref = f.robot.now_position;
            f.robot.how_front += 4;
            f.robot.cfront1 = YELLOW;
            f.robot.cfront2 = YELLOW;
            f.yellowB1 = NONE;
            int dt1 = diist(f.robot.now_position, vertoplaces[24]);
            int dt2 = diist(f.robot.now_position, vertoplaces[25]);
            if (dt1 > dt2) {
                f.robot.now_position = vertoplaces[25];
                perebor(f, ndist + dt2, nway);
            }
            else {
                f.robot.now_position = vertoplaces[24];
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
            int dt1 = diist(f.robot.now_position, vertoplaces[21]);
            int dt2 = diist(f.robot.now_position, vertoplaces[22]);
            if (dt1 > dt2) {
                f.robot.now_position = vertoplaces[22];
                perebor(f, ndist + dt2, nway);
            }
            else {
                f.robot.now_position = vertoplaces[21];
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
            int dt1 = diist(f.robot.now_position, vertoplaces[10]);
            int dt2 = diist(f.robot.now_position, vertoplaces[11]);
            if (dt1 > dt2) {
                f.robot.now_position = vertoplaces[10];
                perebor(f, ndist + dt2, nway);
            }
            else {
                f.robot.now_position = vertoplaces[11];
                perebor(f, ndist + dt1, nway);
            }
            f.blueB1 = BLUE;
            f.robot.how_front = 0;
            f.robot.now_position = pref;
        }
    }
    write(1, 70, 3);
    if (f.robot.how_front > 0) {
        if (f.cnt1 < 4) {
            if (f.robot.how_front == 2) {
                if (f.house1.left == f.robot.cfront1 || f.house1.right == f.robot.cfront1) {
                    int pref = f.robot.now_position;
                    int dt = diist(f.robot.now_position, vertoplaces[28]);
                    f.robot.now_position = vertoplaces[28];
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
                    int dt = diist(f.robot.now_position, vertoplaces[27]);
                    f.robot.now_position = vertoplaces[27];
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
                        int dt = diist(f.robot.now_position, vertoplaces[28]);
                        f.robot.now_position = vertoplaces[28];
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
                    int dt = diist(f.robot.now_position, vertoplaces[6]);
                    f.robot.now_position = vertoplaces[6];
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
                    int dt = diist(f.robot.now_position, vertoplaces[5]);
                    f.robot.now_position = vertoplaces[5];
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
                        int dt = diist(f.robot.now_position, vertoplaces[6]);
                        f.robot.now_position = vertoplaces[6];
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
                    int dt = diist(f.robot.now_position, vertoplaces[16]);
                    f.robot.now_position = vertoplaces[16];
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
                    int dt = diist(f.robot.now_position, vertoplaces[15]);
                    f.robot.now_position = vertoplaces[15];
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
                        int dt = diist(f.robot.now_position, vertoplaces[16]);
                        f.robot.now_position = vertoplaces[16];
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
    write(1, 70, 4);
    if (f.robot.how_back == 0) {
        if (f.blueA1 == 2) {
            int pref = f.robot.now_position;
            f.robot.how_back += 4;
            f.robot.cback1 = BLUE;
            f.robot.cback2 = BLUE;
            f.blueA1 = NONE;
            int dt = diist(f.robot.now_position, vertoplaces[13]);
            f.robot.now_position = vertoplaces[13];
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
            int dt = diist(f.robot.now_position, vertoplaces[23]);
            f.robot.now_position = vertoplaces[23];
            perebor(f, ndist + dt, nway);
            f.greenA1 = GREEN;
            f.robot.how_back = 0;
            f.robot.now_position = pref;
        }
    }
    write(1, 70, 5);
    if (f.robot.how_back > 0){
        if (f.cnt1 < 4) {
            if (f.robot.how_back == 2) {
                if (f.house1.left == f.robot.cback2 || f.house1.right == f.robot.cback2) {
                    int pref = f.robot.now_position;
                    int dt = diist(f.robot.now_position, vertoplaces[26]);
                    f.robot.now_position = vertoplaces[26];
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
                    int dt = diist(f.robot.now_position, vertoplaces[29]);
                    f.robot.now_position = vertoplaces[29];
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
                        int pref2 = f.robot.now_position;
                        int dt = diist(f.robot.now_position, vertoplaces[30]);
                        f.robot.now_position = vertoplaces[30];
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
                        f.robot.now_position = pref2;
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
    write(1, 70, 6);
    if (f.cntutils < 4) {
        if (f.robot.how_front == 2) {
            int pref = f.robot.now_position;
            int dt = diist(f.robot.now_position, vertoplaces[20]);
            f.robot.now_position = vertoplaces[20];
            f.robot.how_front -= 2;
            f.cntutils += 2;
            perebor(f, ndist + dt, nway);
            f.cntutils -= 2;
            f.robot.how_front += 2;
            f.robot.now_position = pref;
        }
        else if (f.cntutils == 0) {
            int pref = f.robot.now_position;
            int dt = diist(f.robot.now_position, vertoplaces[32]);
            f.robot.now_position = vertoplaces[32];
            f.robot.how_front -= 4;
            f.cntutils += 4;
            perebor(f, ndist + dt, nway);
            f.cntutils -= 4;
            f.robot.how_front += 4;
            f.robot.now_position = pref;
        }
        if (f.robot.how_back == 4) {
            int pref = f.robot.now_position;
            int dt = diist(f.robot.now_position, vertoplaces[33]);
            f.robot.now_position = vertoplaces[33];
            f.robot.how_back -= 2;
            f.cntutils += 2;
            perebor(f, ndist + dt, nway);
            f.cntutils -= 2;
            f.robot.how_back += 2;
            f.robot.now_position = pref;
        }
        if (f.robot.how_back == 2) {
            int pref = f.robot.now_position;
            int dt = diist(f.robot.now_position, vertoplaces[34]);
            f.robot.now_position = vertoplaces[34];
            f.robot.how_back -= 2;
            f.cntutils += 2;
            perebor(f, ndist + dt, nway);
            f.cntutils -= 2;
            f.robot.how_back += 2;
            f.robot.now_position = pref;
        }
    }
    write(1, 70, 7);
}
*/

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
    line(speed, grad[18] - dws - 15, 0);
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
    f.house1 = gtf();
}

void f20() {
    moveBC(speed, -360);
}

void f21() {
    line(speed, 50, 4);
}

void f22() {
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
    //moveA(speedA, before_take_green_loops);
    how_a = before_take_green_loops;
    moveBC(MIN, -50);
    line(MIN1, 115, 0);
    stopBC();
    //moveA(speedA, after_take_green_loops);
    how_a = after_take_green_loops;
}

void f27() {
    moveBC(speed, -80);
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
    line(speed, grad[2] - 2 * dws, 1);
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
    t1w(speed, w90, -2);
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

void f60a() {
    line(speed, 380 - dws, 0);
}

void f61a() {
    line(speed, 380 - dtw, 0);
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

void f66a() {
    line(speed, 300 - dws, 4);
}

void f67a() {
    line(speed, 300 - dtw, 4);
}

void f66b() {
    line(speed, 430 - dws, 4);
}

void f67b() {
    line(speed, 430 - dtw, 4);
}

void f66c() {
    line(speed, 800 - dws, 4);
}

void f67c() {
    line(speed, 800 - dtw, 4);
}

void f66d() {
    line(speed, 380 - dws, 0);
}

void f67d() {
    line(speed, 380 - dtw, 0);
}

void f66e() {
    line(speed, 410 - dws, 0);
}

void f67e() {
    line(speed, 410 - dtw, 0);
}

void f66f() {
    line(speed, 130, 0);
}

void f67f() {
    line(speed, 130, 0);
}


void f66g() {
    line(speed, 410 - dws, 0);
}

void f67g() {
    line(speed, 410 - dtw, 0);
}



void f68() {
    f.house2 = gtf();
}

void f69() {
    moveBC(speed, -40);
    turn(speed, d90, -1);
    wait(5000);
    ColorRGB x = getRGB(4);
    if (x.r + x.g + x.b > 10) {
        f.B = BLUE;
    }
    moveB(speed, w90, 1);
    moveBC(speed, -30);
    goBC(speed);
    while (s3() > black);
}

void f70() {
    line(speed, 100, 0);
}

void f71() {
    line(speed, 50, 0);
    stopBC();
    wait(5000);
}

void f72() {
    line(speed, 430, 0);
}

void f72a() {
    line(speed, grad[10] - 2 * dws, 0);
}


void f73() {
    moveBC(speed, 450, 0);
    while (s2() > black);
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
    f.house3 = gtf();
}

void f89() {
    moveBC(speed, -360);
}

void f90() {
    line(speed, grad[12] - 2 * dws, 1);
}

void f91() {
    line(speed, 50, 1);
}

void f91a() {
    line(speed, 50, 4);
}

void f92() {
    line(speed, grad[13] - dws, 3);
}

void f93() {
    moveBC(speed, dsl, 0);
    line(speed, 100 - dsl, 0);
}

void f94() {
    turn(speed, d90 - 70, 3);
    turn(speed, 70, -2);
}

void f95() {
    moveBC(speed, 50, 0);
    line(ONE, 460, 5);
    goBC(speed);
    while (s2() > black);
    stopBC();
    wait(5001);
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

void take4blue1() {
    moveBC(speed, -200);
    //moveA(speedA, before_take_blue_loops);
    how_a = before_take_blue_loops;
    line(MIN1, 100, 1);
    moveBC(MIN1, 120);
    //moveA(speedA, after_take_blue_loops);
    how_a = after_take_blue_loops;
    wait(500);
}

void take4blue2() {
    stopBC();
    moveA(speedA, before_take_blue_loops);
    moveBC(MIN, 70);
    moveA(speedA, after_take_blue_loops);
}

void bb() {
    moveBC(speed, -250);
}

void f99() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -400, 0);
    moveD(speedD, after_take_cubes);
}

void f100() {
    moveBC(speed, 180, 0);
    while (s2() > black);
}

void f101() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -590);
    moveD(speedD, after_take_cubes);
}

void f102() {
    moveBC(speed, 350, 0);
    while (s3() > black);
}

void f103() {
    line(speed, grad[6] - dws, 0);
    moveBC(speed, 50, 0);
}

void f104() {
    line(speed, grad[6] - dtw, 0);
    moveBC(speed, 50, 0);
}

void f103a() {
    line(speed, grad[5] - dws, 0);
    moveBC(speed, 50, 0);
}

void f104a() {
    line(speed, grad[5] - dtw, 0);
    moveBC(speed, 50, 0);
}

void f103b() {
    line(speed, grad[5] - dws, 2);
}

void f104b() {
    line(speed, grad[5] - dtw, 2);
}

void f103c() {
    line(speed, grad[6] - dws, 3);
}

void f104c() {
    line(speed, grad[6] - dtw, 3);
}

void f105() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -300);
    //moveBC(MIN, -100);
    moveD(speedD, after_take_cubes);
}

void f106() {
    moveBC(speed, 430, 0);
    while (s2() > black);
}

void f107() {
    moveBC(speed, 650 - dws - 70, 0);
    while (s2() > black);
}

void f108() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -600);
    moveD(speedD, after_take_cubes);
}

void f109() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -650);
    //moveBC(MIN, -100);
    moveD(speedD, after_take_cubes);
}

void f110() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -250);
    //moveBC(MIN, -100);
    moveD(speedD, after_take_cubes);
}

void f111() {
    moveBC(speed, 250);
}

void f112() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -250);
    //moveBC(MIN, -100);
    moveD(speedD, after_take_cubes);
}

void f113() {
    moveBC(speed, 250);
}

void f114() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -350);
    moveD(speedD, after_take_cubes);
}

void f115() {
    goBC(speed);
    while (s3() > black);
}

void f116() {
    moveD(speedD, before_take_cubes);
    moveBC(speed, -500);
    moveD(speedD, after_take_cubes);
}

void f117() {
    moveBC(speed, 250, 0);
    while (s3() > black);
}

void f118() {
    line(speed, 180 - dws, 3);
}

void f119() {
    line(speed, 180 - dtw, 3);
}

void f120() {
    line(speed, 180 - dws, 0);
}

void f121() {
    line(speed, 180 - dtw, 0);
}

void f122() {
    line(speed, 180 - dws, 0);
}

void f123() {
    line(speed, 180 - dtw, 0);
}

void f124() {
    line(speed, 180 - dws, 2);
}

void f125() {
    line(speed, 180 - dtw, 2);
}

void f126() {
    line(speed, 180, 6);
    moveBC(speed, dsl, 0);
    while (s3() > black);
}

void f127() {
    line(speed, 50, 3);
}

void f128() {
    line(speed, 350, 3);
}

void f129() {
    line(speed, 350, 3);
}



void unload_loops0() {

}

void unload_loops1() {

}

void unload_loops2() {

}

void unload_cubes0() {

}

void unload_cubes1() {

}

void hcb() {

}

void hlb() {

}

void utilsll() {

}

void utilslr() {

}

void backutils1() {

}

void utilscubes() {

}

void backutilscubes() {

}

void fend() {

}

void add(int from, int to, void (*def)(), short time = 1000, bool active = true, short index = 0) {
    g[from].pb(Edge(to, def, time, active, index));
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

    add(13, 1, fend);

    add(16, 29, f9);
    add(15, 29, f10);
    add(34, 23, f9);
    add(33, 23, f10);

    add(28, 113, f11);
    add(27, 113, f12);
    add(118, 35, f11);
    add(117, 35, f12);

    add(28, 53, f13);
    //add(27, 53, f14);
    add(58, 35, f32);
    //add(57, 35, f32);

    add(118, 59, f15);
    //add(117, 59, f16);
    add(52, 113, f33);
    add(51, 113, f34);

    add(31, 44, f17);
    //add(30, 44, f18);

    add(337, 26, f35);

    add(44, 336, f19);

    add(336, 298, f20);
    add(295, 44, f21);
    add(289, 26, f22);

    add(109, 86, f25);
    add(108, 86, f26);
    add(91, 116, f25);
    add(90, 116, f26);

    add(109, 98, f28);
    add(108, 98, f29);
    add(103, 116, f36);
    add(102, 116, f37);

    add(97, 86, f38);
    //add(96, 86, f39);
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
    //add(168, 182, f55); 
    add(187, 176, f56);
    add(186, 176, f57);

    add(169, 194, f58);
    add(168, 194, f59);
    add(199, 176, f60);
    add(198, 176, f61);

    add(199, 188, f60a);
    add(198, 188, f61a);

    add(169, 302, f62);
    add(168, 302, f63);
    add(307, 176, f64);
    add(306, 176, f65);

    add(169, 206, f66);
    add(168, 206, f67);

    add(301, 206, f66a);
    //add(300, 206, f67a);

    add(193, 206, f66b);
    add(192, 206, f67b);

    add(193, 301, f66f);
    add(307, 199, f67f);

    add(307, 188, f66g);
    add(306, 188, f67g);

    add(181, 206, f66c);
    add(180, 206, f67c);

    add(181, 194, f66d);
    add(180, 194, f67d);

    add(181, 302, f66e);
    add(180, 302, f67e);

    add(206, 341, f68, 1000, 0, 1);
    add(341, 203, f69);

    add(342, 307, f70);

    add(342, 200, f71);

    add(342, 188, f72);

    add(342, 176, f72a);

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
    //add(219, 245, f83);
    add(250, 227, f84);
    add(249, 227, f85);

    add(247, 260, f86);
    //add(246, 260, f87);

    add(260, 345, f88);

    add(345, 334, f89);

    add(346, 242, f90);

    add(325, 242, f91);
    add(331, 260, f91a);

    add(244, 269, f92);

    add(269, 268, f93);

    add(268, 265, f94);
    add(265, 278, f95);

    add(278, 277, f96);

    add(277, 280, f97);
    add(277, 286, f98);

    add(280, 347, take4blue1);
    add(281, 347, take4blue2);

    add(347, 281, bb);
    add(281, 282, f6d);
    add(282, 272, f126);
    add(272, 271, f1d);
    add(271, 274, f2l);
    add(274, 251, f127);

    add(358, 131, f128, 1000, 1, 3);
    add(352, 179, f129, 1000, 1, 2);

    add(55, 67, f99);
    add(67, 56, f100);

    add(100, 64, f101);
    add(64, 101, f102);

    add(88, 161, f103);
    add(87, 161, f104);

    add(166, 95, f103c);
    add(165, 95, f104c);

    add(94, 83, f103a);
    add(95, 83, f104a);

    add(76, 89, f103b);
    add(75, 89, f104b);

    add(280, 232, f105);

    add(235, 224, f106);
    add(238, 203, f107);
    add(223, 235, f108);
    add(202, 238, f109);

    add(79, 338, f110);
    add(338, 79, f111);
    add(163, 340, f112);
    add(340, 163, f113);

    add(139, 151, f114);
    add(151, 356, f115);
    add(184, 148, f116);
    add(148, 185, f117);

    add(206, 342, f5l);
    add(260, 346, f5l);
    add(44, 337, f5l);

    add(346, 365, unload_cubes0);
    add(337, 370, unload_cubes0);
    add(342, 360, unload_cubes0);

    add(346, 366, unload_cubes1);
    add(337, 371, unload_cubes1);
    add(342, 361, unload_cubes1);

    add(260, 367, unload_loops0);
    add(44, 372, unload_loops0);
    add(206, 362, unload_loops0);

    add(260, 368, unload_loops1);
    add(44, 373, unload_loops1);
    add(206, 363, unload_loops1);

    add(260, 369, unload_loops2);
    add(44, 374, unload_loops2);
    add(206, 364, unload_loops2);

    add(365, 346, hcb);
    add(370, 337, hcb);
    add(360, 342, hcb);

    add(366, 346, hcb);
    add(371, 337, hcb);
    add(361, 342, hcb);

    add(367, 346, hlb);
    add(372, 337, hlb);
    add(362, 342, hlb);

    add(368, 346, hlb);
    add(373, 337, hlb);
    add(363, 342, hlb);

    add(369, 346, hlb);
    add(374, 337, hlb);
    add(364, 342, hlb);

    addcrossroad(378, 0, 1, 0, 1);
    addcrossroad(390, 0, 1, 0, 1);

    add(382, 173, f118);
    //add(381, 173, f119);
    add(178, 389, f120);
    //add(177, 389, f121);

    add(172, 395, f122);
    //add(171, 395, f123);
    add(400, 179, f124);
    //add(400, 179, f125);

    /*add(374, 364, utilsll);
    add(386, 365, utilslr);
    add(364, 374, backutils1);
    add(365, 374, backutils1);
    add(169, 362, utilscubes);
    add(169, 363, utilscubes);
    add(362, 169, backutilscubes);
    add(363, 169, backutilscubes);*/
}

void vivod_h() {
    while (10000) {
        Clear_Display();
        write(10, 10, getHSV(4).h);
        wait(50);
    }
}

int used[maxv];

int clrclr = 1;

void dfs(int v) {
    used[v] = clrclr;
    Clear_Display();
    write(40, 40, v);
    write(60, 60, g[v].size());
    write(80, 80, used[v]);
    wait(100);
    for (int i = 0; i < g[v].size(); i++) {
        int to = g[v][i].get_to();
        if (used[to] == 0) {
            dfs(to);
        }
    }
}

void builddfs() {
    for (int i = 0; i < maxv; i++)
        used[i] = 0;
    dfs(1);
    write(1, 1, "start");
    wait(3000);
    Clear_Display();
    for (int i = 0; i < 391; i++) {
        write(1, 1, i);
        write(20, 20, used[i]);
        wait(1000);
    }
    write(1, 1, "end");
    wait(10001);
}
/*
    bool: 1
    char: 1
    short: 2
    int: 4
    long: 
    long long: 
    float: 
    double: 
    long double: 
*/

int EV3_main()
{
    Clear_Display();
    CreateThread(control, 0);
    CreateThread(okonchanie, 0);

    s3();
    s2();
    gclr(4);
    buildDegreesConstants();
    buildg();
    buildplaces();

    goD(-speedD);
    wait(300);
    goD(0);
    how_a = after_take_green_loops;

    go(0, 7, 235);
    go(0, 235, 206);
    stopBC();
    Edge::close(1);
    go(0, 206, 238);
    go(0, 238, 260);

    stopBC();
    wait(100000);
    Clear_Display();
    return 0;
    go(speed, 345, 109);
    go(speed, 109, 338);
    write(90, 90, maxim);
    wait(6000);
    return 0;

    /*f.B = YELLOW;
    go(speed, 7, 336);
    go(speed, 336, 136);
    ColorRGB x = getRGB(4);
    if (x.r + x.b + x.g > 10)
        f.B = GREEN;
    go(speed, 136, 341);
    go(speed, 341, 345);
    stopBC();
    go(speed, 345, 67);
    go(speed, 67, 206);
    turn(speed, d180, 0);
    moveBC(speed, -150);q
    moveD(speedD, before_take_cubes);
    Clear_Display();
    write(80, 1, f.B);
    wait(5000);*/
    f.B = YELLOW;
    f.house1.left = NONE;
    f.house1.right = YELLOW;
    f.house2.left = GREEN;
    f.house2.right = GREEN;
    f.house3.left = BLUE;
    f.house3.right = YELLOW;
    f.robot.now_position = 345;
    f.cnt1 = f.cnt2 = f.cnt3 = f.cntutils = f.robot.how_front = f.robot.how_back = 0;
    f.yellowA1 = f.yellowA2 = f.yellowA3 = f.yellowA4 = YELLOW;
    f.greenA1 = f.greenA2 = f.greenA3 = f.greenA4 = GREEN;
    f.blueA1 = f.blueA2 = f.blueA3 = f.blueA4 = BLUE;
    f.yellowB1 = f.yellowB2 = f.yellowB3 = f.yellowB4 = YELLOW;
    f.greenB1 = f.greenB2 = f.greenB3 = f.greenB4 = NONE;
    f.blueB1 = f.blueB2 = f.blueB3 = f.blueB4 = NONE;
    if (f.B == YELLOW)
        f.yellowB1 = f.yellowB2 = f.yellowB3 = f.yellowB4 = YELLOW;
    if (f.B == BLUE)
        f.blueB1 = f.blueB2 = f.blueB3 = f.blueB4 = BLUE;
    if (f.B == GREEN)
        f.greenB1 = f.greenB2 = f.greenB3 = f.greenB4 = GREEN;
    return 0;
    //vivod_h();
    goD(-speedD);
    wait(500);
    goD(0);
    goA(-50);
    wait(600);
    stopA();
    moveA(speedA, after_take_green_loops);
    wait(20000);
    return 0;
}