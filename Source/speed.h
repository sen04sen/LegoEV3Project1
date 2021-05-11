#ifndef speed_h
#define speed_h


using namespace std;

struct Speed {
    int maxS;
    int minS;
    double p;
    double d;
    int sEnc;
    int eEnc;
    int zEnc; // сверх иновационное финишное движение (с минимальной скоростью)


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
};


#endif