/*!
\file
\brief Файл содержит неиспользуемые функции (не рекомендуется подключать)
*/

#ifndef noUse_h
#define noUse_h


using namespace ev3_c_api;
using namespace std;



void give2() {
    stopBC();
    stopD();
    wait(30);
    moveD(-speedD, 400);
    moveBC(speed, 200);
    moveBC(speed, -110);
    moveD(speedD, 400);
    moveDTime(1, 500);
    stopD();
    moveD(-5, 8);
    wait(100);
    moveBC(speed, -150);
    moveD(-speedD, 100);
    moveBC(speed, 60);
    moveD(speedD, 120);
    goD(3);
}

/*void end_4_green() {
    line(speed, 170, 2);
    moveBC(speed, -50, 1);
    turn(speed, d90, -1);
    moveBC(10, 140, 1);
    moveD(-speedD, 70);
    moveBC(speed, -140, 1);
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
    moveBC(speed, -140, 1);
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
        moveBC(speed, -400, 1);
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
        moveBC(speed, -400, 1);
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
}*/


void get_4_blue() {
    stopBC();
    moveBC(speed, -30, 1);
    turn(speed, d90, -1);
    moveBC(speed, -100, 1);
    moveD(speedD, put_loops);
    moveBC(speed, 140, 1);
    moveD(speedD, 280);
    moveBC(speed, -30, 1);
    turn(speed, d90, -2);
    moveBC(speed, 130, 1);
    goBC(speed, 2);
    while (s3() > bluck);
    stopBC();
    line(speed, 150, 6);
    stopBC();
    turn(speed, d90, -1);
    moveBC(speed, -50, 1);
    moveD(speedD, 395);
    moveBC(speed, 90, 1);
    moveD(speedD, 280);
    moveBC(speed, -60, 1);
    turn(speed, d90, -1);
    moveBC(speed, 600, 0);
    goBC(speed);
    while (s2() > black);
    stopBC();
}

#ifndef noUse_h
#define noUse_h

#include "constants.h"

using namespace ev3_c_api;
using namespace std;

void line(int sp, int dist, int tp) {
    bool stop = 0;
    if (tp == 4) {
        getRGB(3);
        EV3_Sleep(50);
    }
    double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    if (tp == 4)
        getRGB(3);
    while (!stop) {
        if (tp == 0 || tp == 6 || tp == 8) {
            if (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st > dist) {
                stop = 1;
            }
        } else {
            if (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st > dist - 50) {
                if (tp == 1 && s2() < black && s3() < black)
                    stop = 1;
                else if (tp == 2 && s3() < black)
                    stop = 1;
                else if (tp == 3 && s2() < black)
                    stop = 1;
                else if (tp == 4) {
                    //make_pair(make_pair(r, g), b);
                    ColorRGB color = getRGB(3);
                    if (color.r - color.g > 70)
                        stop = 1;
                } else if (tp == 5 && s2() > bluck)
                    stop = 1;
                else if (tp == 7 && s3() < black)
                    stop = 1;
            }
        }
        double del;
        if (tp == 5 || tp == 6) {
            del = (double) (s3() - bley) * Pr * 3 / 3;
        } else if (tp == 4) {
            del = (double) (grey - s2()) * Pr * 2 / 3;
        } else if (tp == 7 || tp == 8) {
            del = (double) (s2() - grey) * Pr * 3 / 3;
        } else
            del = (double) (s3() - s2()) * Pr;
        SpeedMotor(E_Port_B, -1 * (sp - del));
        SpeedMotor(E_Port_C, sp + del);
    }
    if (tp == 4)
        s3();
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

#endif

#endif