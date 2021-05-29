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

void t1w_old(int sp, int dt, int tp) {
    if (tp >= 0) {
        if (tp < 2) {
            stopC();
            SpeedMotor(E_Port_B, -1 * (sp));
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            dt -= 180;
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(dt));
            while (s3() > black);
            st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(d1w));
        } else {
            stopB();
            SpeedMotor(E_Port_C, sp);
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            dt -= 180;
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(dt));
            while (s2() > black);
            st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(d1w));
        }
    } else {
        if (tp == -1) {
            stopC();
            SpeedMotor(E_Port_B, -1 * (sp));
            double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < abs(dt));
        } else {
            stopB();
            SpeedMotor(E_Port_C, sp);
            double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
            while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < abs(dt));
        }
    }
    stopBC();
}
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


        buildplaces();

#endif

#endif