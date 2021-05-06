#ifndef speed_h
#define speed_h

#include <map>

using namespace ev3_c_api;
using namespace std;

enum SpeedProfileName {
    MIN,
    MAX,
    ZERO,
    DOP1,
    DOP2,
    DOP3,
    DOP4,
};

struct Speed {
    int maxS;
    int minS;
    double p;
    double d;
    int sEnc;
    int eEnc;
    int zEnc; // сверх иновационное финишное движение (с минимальной скоростью)

    static map <SpeedProfileName, Speed> speeds;

    Speed() {}

    Speed(int _maxS, int _minS, double _p, double _d, int _sEnc, int _eEnc, int _zEnc) {
        maxS = _maxS;
        minS = _minS;
        p = _p;
        d = _d;
        sEnc = _sEnc;
        eEnc = _eEnc;
        zEnc = _zEnc;
    }

    static void add(SpeedProfileName name, int _maxS, int _minS, double _p, double _d, int _sEnc, int _eEnc, int _zEnc) {
        speeds[name] = Speed(_maxS, _minS, _p, _d, _sEnc, _eEnc, _zEnc);
    }
};

map <SpeedProfileName, Speed> Speed::speeds = map <SpeedProfileName, Speed>();

#endif