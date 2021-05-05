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
#include "movement.cpp"


using namespace ev3_c_api;
using namespace std;

#define pb push_back

const int maxv = 90;
int ce = 21 * 2;
int ver = 22;

template<class T>
string str(T forConvert) {
    ostringstream ss;
    ss << forConvert;
    return ss.str();
}

class Exception {
    // Тут просто класс наших исключений
private:
    std::string m_error;

public:
    Exception(string error) : m_error(error) {}

    Exception(int line) : m_error(str(line)) {}

    const char *what() { return m_error.c_str(); }
};

/*
try{

}
catch(...){
    throw Exception(__LINE__)
}
*/

vector<vector<Edge> > g;

class Edge {
    int to, index;
    double time;
    bool active;
    void (*def)();
    // index объединяет ребра в группы для одновременной активации или дезактивации

public:
    static double closeTime;

    Edge(int newTo, void (*const newDef)(), double newTime = 1.0, bool newActive = true, int newIndex = 0) : to(newTo),
                                                                                                             time(newTime),
                                                                                                             active(newActive),
                                                                                                             index(newIndex) {}

    void operator()(bool reWriteTime = true) {
        if (reWriteTime) {
            T_TimerId id = Timer_Start();
            def();
            time = Timer_GetTime(id);
        } else def();
    }

    int getTo() { return to; }

    double getTime() { return active ? time : closeTime; }

    double getIndex() { return index; }

    void close() { active = false; }

    void open() { active = true; }

    static void closeFromIndex(int newIndex) {
        for (int i = 0; i < g.size(); ++i)
            for (int j = 0; j < g[i].size(); ++j)
                if (g[i][j].getIndex() == newIndex)
                    g[i][j].close();
    }

    static void openFromIndex(int newIndex) {
        for (int i = 0; i < g.size(); ++i)
            for (int j = 0; j < g[i].size(); ++j)
                if (g[i][j].getIndex() == newIndex)
                    g[i][j].open();
    }
};

void fillG() {
    vector<Edge> vec;

    vec.pb(Edge(1, []() {}));
    vec.pb(Edge(1, []() {}));
    vec.pb(Edge(1, []() {}));
    vec.pb(Edge(1, []() {}));
    g.pb(vec);
    vec.clear();
}


double stadegd;

double Pr = 0.3;
double dws = 130; // расстояние между датчиками и колёсами
double dsl = 50; //съезд датчиков с линии
int proe = 50;
int d90 = 250;
int d180 = 500;
int black = 25;
int bluck = 35;
int grey = 35;
int bley = 45;
int dovorot = 50;
int pov1wheel = 525;

int ndir = 0;
int speed = 15;
int speedD = 15;

void wait(int time) {
    // время сна в миллисекундах
    EV3_Sleep(time);
}

int s2() {
    return GetReflect(E_Port_2);
}

int s3() {
    return GetReflect(E_Port_3);
}


struct Color {
    int r, g, b;

    Color(int newR, int newG, int newB) : r(newR), g(newG), b(newB) {}
};


Color getRGB(int port) {
    const void *a;
    switch (port) {
        case 3:
            a = GetData_UART(E_Port_3, E_UART_Type_Color, 4);
            break;
        case 4:
            a = GetData_UART(E_Port_4, E_UART_Type_Color, 4);
            break;
        case 2:
            a = GetData_UART(E_Port_2, E_UART_Type_Color, 4);
            break;
        default:
            throw Exception("invalid port parameter (need 2-4), you gave " + str(port));
            break;
    }
    unsigned char *d = reinterpret_cast<unsigned char *>(const_cast<void *>(a));
    int r = d[0];
    int g = d[2];
    int b = d[4];
    Color color = Color(r, g, b);
    return color;
}

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
                    Color color = getRGB(3);
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

void pov(int sp, int dt, int tp) {
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

int gclr(int uy) {
    if (uy == 4) {
        return GetColor(E_Port_4);
    } else {
        return GetColor(E_Port_3);
    }
}

void write(int x, int y, int uy) {
    stringstream s;
    s << uy;
    string a;
    s >> a;
    Draw_Text(x, y, E_Font_Normal, 0, &(a[0]));
}


pair<int, int> msgo[maxv];

/*int go(int sp, int fromm, int dd1, int totoo, int dd2, bool lst = 1) {
    int from = fromm * 4 + dd1;
    int toto = totoo * 4 + dd2;
    for (int i = 0; i < maxv; i++) {
        msgo[i].first = 0;
        msgo[i].second = -1;
    }
    msgo[from] = make_pair(-1, 0);
    set<pair<int, int> > st;
    st.insert(make_pair(0, from));
    bool end = 0;
    while (!st.empty()) {
        int dd = st.begin()->first;
        int v = st.begin()->second;
        st.erase(st.begin());
        for (unsigned int i = 0; i < g[v].size(); i++) {
            int to = g[v][i].first;
            if (msgo[to].second == -1) {
                msgo[to].first = v;
                msgo[to].second = g[v][i].second;
                if (to == toto) {
                    end = 1;
                    break;
                }
                st.insert(make_pair(dd + ed[msgo[to].second].dt, to));
            }
        }
        if (end)
            break;
    }
    vector<int> way;
    int nv = toto;
    while (nv != from) {
        way.pb(msgo[nv].second);
        nv = msgo[nv].first;
    }
    int kak = 1;
    for (int i = way.size() - 1; i >= 0; i--) {
        int nw = way[i];
        write(kak, 1, nw);
        kak += 20;
        int del = 0;
        if (lst == 0) {
            if (ed[nw].tp >= 4) {
                moveBC(sp, proe, 0);
                del = dws - proe;
            }
            else if (ed[nw].tp >= -2)
                moveBC(sp, dws);
            else
                moveBC(sp, dws, 0);
        }
        if (ed[nw].tp == -3) {
            moveBC(sp, ed[nw].dt + del, 0);
            lst = 0;
            continue;
        }
        if (ed[nw].tp < 4) {
            pov(sp, ed[nw].dt, ed[nw].tp);
            lst = 1;
            continue;
        }
        line(sp, ed[nw].dt + del, ed[nw].tp - 4);
        lst = 0;
    }
    return way.size();
}*/

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
    moveBC(1, 2);
    moveBC(1, 2);
    moveD(speedD, 400);
    moveDTime(1, 500);
    stopD();
    moveD(-5, 8);
    wait(100);
    moveBC(1, 2);
    moveD(-speedD, 100);
    moveBC(1, 2);
    moveD(speedD, 120);
    goD(3);
}

void give4() {
    stopBC();
    stopD();
    wait(30);
    moveD(-speedD, 400);
    moveBC(1, 2);
    moveBC(1, 2);
    moveD(speedD, 400);
}

pair<int, int> gtf() {
    moveBC(1, 2);
    SpeedMotor(E_Port_C, -speed);
    double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
    while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < pov1wheel);
    stopC();
    moveBC(1, 2);
    GetColor(E_Port_4);
    EV3_Sleep(500);
    int fi = gclr(4);
    if (fi == 7)
        fi = 4;
    moveBC(1, 2);
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
    moveBC(1, 2);
    pov(speed, d90, 0);
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
    moveBC(1, 2);
    pov(speed, d90, -1);
    moveBC(1, 2);
    moveD(-speedD, 70);
    moveBC(1, 2);
    pov(speed, d90, 3);
    line(speed, 30, 2);
    moveBC(1, 2);
    line(speed, 220, 0);
    stopBC();
    pov(speed, d90, -1);
    moveD(10, 65);
    EV3_Sleep(1000);
    moveBC(1, 2);
    moveD(-speedD, 65);
    moveBC(1, 2);
    pov(speed, d90, 3);
    line(speed, 400, 3);
    if (d1.first == 3 || d1.second == 3) {
        moveBC(1, 2);
        pov(speed, d180, 1);
        line(speed, 780, 3);
        moveBC(speed, proe, 0);
        line(speed, 170, 2);
        moveBC(speed, proe, 0);
        line(speed, 590, 3);
        moveBC(1, 2);
        pov(speed, d90, 3);
        line(speed, 200, 4);
        moveBC(1, 2);
        moveD(10, 110);
        moveBC(1, 2);
        pov(speed, d90, 0);
        SpeedMotor_Time(E_Port_D, -30, 2000);
        wait(2000);
        stopD();
        line(speed, 270, 2);
        moveBC(1, 2);
        pov(speed, d90, 0);
        line(speed, 750, 0);
        moveBC(1, 2);
        wait(10000);
    } else {
        moveBC(speed, proe, 0);
        line(speed, 760, 2);
        moveBC(1, 2);
        pov(speed, d90, 0);
        line(speed, 200, 4);
        moveBC(1, 2);
        moveD(10, 110);
        moveBC(1, 2);
        pov(speed, d90, 0);
        SpeedMotor_Time(E_Port_D, -30, 2000);
        wait(2000);
        stopD();
        line(speed, 760, 2);
        moveBC(speed, proe, 0);
        line(speed, 780, 3);
        moveBC(speed, proe, 0);
        line(speed, 170, 2);
        moveBC(speed, proe, 0);
        line(speed, 590, 3);
        moveBC(speed, proe, 0);
        line(speed, 270, 2);
        moveBC(1, 2);
        pov(speed, d90, 0);
        line(speed, 750, 0);
        moveBC(1, 2);
        wait(10000);
    }
}

void pov_bat() {
    stopBC();
    moveC(speed, 80);
    moveB(speed, 80);
    moveBC(1, 2);
    moveD(speedD, 220);
    moveB(-speed, 30);
    moveD(speedD, 250);
    moveBC(1, 2);
    moveD(speedD, 500);
    pov(speed, d90 - 20, 3);
    line(speed, 330, 0);
    stopBC();
    pov(speed, d90, -1);
    moveBC(1, 2);
    moveD(speedD, 20);
    moveBC(1, 2);
    moveD(speedD, 220);
    moveB(-speed, 30);
    moveD(speedD, 250);
    moveBC(1, 2);
    moveD(speedD, 500);
    moveBC(1, 2);
    pov(speed, d90 - 30, 0);
    pov(speed, 60, -1);
    line(speed, 70, 7);
    moveBC(speed, proe, 0);
    line(speed, 550, 8);
    getRGB(2);
    moveBC(speed, 60, 0);
    goBC(speed);
    while (getRGB(2).b < 100);
    s2();
    moveBC(1, 2);
    pov(speed, d90, -2);
}

void get_4_blue() {
    moveBC(1, 2);
    pov(speed, d90, -2);
    moveBC(speed, 50, 0);
    line(speed, 460, 5);
    goBC(speed);
    while (s2() > black);
    stopBC();
    moveBC(1, 2);
    pov(speed, d90, -1);
    moveBC(1, 2);
    moveD(speedD, 395);
    moveBC(1, 2);
    moveD(speedD, 280);
    moveBC(1, 2);
    pov(speed, d90, -2);
    moveBC(1, 2);
    goBC(speed, 2);
    while (s3() > bluck);
    stopBC();
    line(speed, 150, 6);
    stopBC();
    pov(speed, d90, -1);
    moveBC(1, 2);
    moveD(speedD, 395);
    moveBC(1, 2);
    moveD(speedD, 280);
    moveBC(1, 2);
    pov(speed, d90, -1);
    moveBC(speed, 600, 0);
    goBC(speed);
    while (s2() > black);
    stopBC();
}

void *okonchanie(void *lpvoid) {
    while (!isBrickButtonPressed(E_BTN_ESC));
    exit(0);
}

void add(int from, int to, function<void()> def, double time = 1.0) {
    try {
        auto vec = g.at(from);
        try {
            vec.pb(Edge(1, def));
        }
        catch (exception &exception) {
            throw Exception("add: to index" + str(exception.what()));
        }
    }
    catch (exception &exception) {
        throw Exception("add: from index" + str(exception.what()));
    }
}

void addcrossroad(int v, int u, int r, int d, int l) {
    if (u == 1) {
        g[v + 2].pb(Edge(v, []() { pov(speed, d90, 3); }));
        g[v + 4].pb(Edge(v, []() { pov(speed, d180, 2); }));
        g[v + 6].pb(Edge(v, []() { pov(speed, d90, 0); }));
    } else {
        g[v + 2].pb(Edge(v, []() { pov(speed, d90, -2); }));
        g[v + 4].pb(Edge(v, []() { pov(speed, d180, -2); }));
        g[v + 6].pb(Edge(v, []() { pov(speed, d90, -1); }));
    }
    if (r == 1) {
        g[v].pb(Edge(v, []() { pov(speed, d90, 0); }));
        g[v + 4].pb(Edge(v, []() { pov(speed, d90, 3); }));
        g[v + 6].pb(Edge(v, []() { pov(speed, d180, 2); }));
    } else {
        g[v].pb(Edge(v, []() { pov(speed, d90, -1); }));
        g[v + 4].pb(Edge(v, []() { pov(speed, d90, -2); }));
        g[v + 6].pb(Edge(v, []() { pov(speed, d180, -2); }));
    }
    if (d == 1) {
        g[v].pb(Edge(v, []() { pov(speed, d90, 2); }));
        g[v + 2].pb(Edge(v, []() { pov(speed, d180, 0); }));
        g[v + 6].pb(Edge(v, []() { pov(speed, d90, 3); }));
    } else {
        g[v].pb(Edge(v, []() { pov(speed, d90, -2); }));
        g[v + 2].pb(Edge(v, []() { pov(speed, d180, -1); }));
        g[v + 6].pb(Edge(v, []() { pov(speed, d90, -2); }));
    }
    if (l == 1) {
        g[v].pb(Edge(v, []() { pov(speed, d90, 3); }));
        g[v + 2].pb(Edge(v, []() { pov(speed, d180, 2); }));
        g[v + 4].pb(Edge(v, []() { pov(speed, d90, 0); }));
    } else {
        g[v].pb(Edge(v, []() { pov(speed, d90, -2); }));
        g[v + 2].pb(Edge(v, []() { pov(speed, d180, -2); }));
        g[v + 4].pb(Edge(v, []() { pov(speed, d90, -1); }));
    }
    g[v + 1].pb(Edge(v + 4, []() { moveBC(speed, dws, 1); }));
    g[v + 1].pb(Edge(v + 9, []() { moveBC(speed, dsl, 0); }));
    g[v + 3].pb(Edge(v + 6, []() { moveBC(speed, dws, 1); }));
    g[v + 3].pb(Edge(v + 11, []() { moveBC(speed, dsl, 0); }));
    g[v + 5].pb(Edge(v, []() { moveBC(speed, dws, 1); }));
    g[v + 5].pb(Edge(v + 8, []() { moveBC(speed, dsl, 0); }));
    g[v + 7].pb(Edge(v + 2, []() { moveBC(speed, dws, 1); }));
    g[v + 7].pb(Edge(v + 9, []() { moveBC(speed, dsl, 0); }));
}

signed EV3_main() {
    addcrossroad(1, 1, 1, 0, 0);
    addcrossroad(13, 0, 1, 1, 1);
    addcrossroad(27, 1, 1, 0, 1);
    addcrossroad(39, 0, 1, 1, 1);
    addcrossroad(51, 1, 1, 0, 1);
    addcrossroad(63, 0, 1, 1, 1);
    add(3, 20, {line(speed, 270, 2);});
    goD(-4);
    wait(1000);
    stadegd = GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium);
    goD(0);
    CreateThread(okonchanie, 0);
    Clear_Display();
    pov_bat();
    return 0;
    moveBC(speed, 250, 0);
    stopB();
    moveC(speed, 220);
    moveD(speedD, 450);
    pov(speed, 120, -1);
    moveD(-speedD, 100);
    moveBC(speed, 10, 0);
    stopC();
    moveB(speed, 790);
    moveD(speedD, 100);
    goD(3);
    pov(speed, 50, -1);
    if (gclr(4) != 0) {
        gdeb = 4;
    }
    moveBC(speed, 375, 0);
    goBC(speed);
    while (s3() > black);
    moveBC(1, 2);
    pov(speed, 130, 3);
    line(speed, 200, 3);
    go(speed, 1, 2, 3, 2, 0);
    d1 = gtf();
    gtb();
    if (d1.first == 4 || d1.second == 4) {
        stopBC();
        pov(speed, d180, 2);
        line(speed, 200, 4);
        p1 = 1;
        give4();
        pov(speed, d180, 2);
        line(speed, 250, 1);
    }
    go(speed, 2, 0, 6, 0, 0);
    pov_bat();
    moveD(-speedD, 140);
    moveBC(1, 2);
    moveD(speedD, 110);
    goD(0);
    moveBC(1, 2);
    pov(speed, d90 + 40, -1);
    goBC(speed);
    while (s2() > black);
    moveBC(1, 2);
    pov(speed, d90, 3);
    line(speed, 200, 4);
    d2 = gtf();
    moveBC(1, 2);
    pov(speed, d90, -1);
    moveBC(1, 2);
    if (gclr(4) != 0) {
        gdeb = 2;
    }
    moveB(speed, pov1wheel);
    goBC(speed);
    while (s3() > black);
    moveBC(1, 2);
    if (d2.first == 4 || d2.second == 4) {
        pov(speed, d90, 0);
        line(speed, 300, 4);
        if (p1)
            give4();
        else
            give2();
        p2 = 1;
        pov(speed, d180, 2);
        line(speed, 600, 1);
    } else {
        pov(speed, d90, 3);
        line(speed, 200, 1);
    }
    go(speed, 12, 2, 15, 2, 0);
    d3 = gtf();
    gtb();
    if (d3.first == 4 || d3.second == 4) {
        pov(speed, d180, 2);
        line(speed, 200, 4);
        if (p1 || p2)
            give4();
        else
            give2();
        p3 = 1;
        pov(speed, d180, 2);
        line(speed, 250, 1);
    }
    moveBC(1, 2);
    pov(speed, d90, 0);
    line(speed, 100, 3);
    get_4_blue();
    return 0;
}
