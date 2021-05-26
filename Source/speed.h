/*!
\file
\brief Файл содержит модуль speed
*/

#ifndef speed_h
#define speed_h

using namespace std;

/*!
    \defgroup speed Скорость
    \brief Модуль, содержащий класс скорости
*/

///@{

/*!
	\brief Настройки скорости
*/

struct Speed {
    int max_sp;
    int min_sp;
    double p;
    double d;
    int normal_enc;
    int up_enc;
    int down_enc;
    int zero_enc;

    Speed() {}

    Speed(int _max_sp, int _min_sp, double _p, double _d, int _normal_enc, int _up_enc, int _end_enc, int _zero_enc) {
        max_sp = _max_sp;
        min_sp = _min_sp;
        p = _p;
        d = _d;
        normal_enc = _normal_enc;
        up_enc = _up_enc;
        down_enc = _end_enc;
        zero_enc = _zero_enc;
    }
};

class Speed_compiled {
    Speed p;
    bool uping, downing;
    int way;
public:
    Speed_compiled(Speed _p, int _way, bool _uping = true, bool _downing = true) {
        p = _p;
        way = _way;
        uping = _uping;
        downing = _downing;
    }

    int operator()(int now_dist) {
        int speed = p.max_sp;
        if (now_dist > way - p.zero_enc && downing) speed = p.min_sp;
        else if (now_dist < p.normal_enc && uping) speed = p.min_sp;
        else {
            int down_sp = p.max_sp, up_sp = p.max_sp;
            if (now_dist > way - p.zero_enc - p.down_enc && downing) {
                down_sp = double(p.max_sp) -
                          double(now_dist - (way - p.zero_enc - p.down_enc)) * double(p.max_sp - p.min_sp) /
                          double(p.down_enc);
            }
            if (now_dist < p.normal_enc + p.up_enc && uping) {
                up_sp = double(now_dist - p.normal_enc) * double(p.max_sp - p.min_sp) / double(p.up_enc) +
                        double(p.min_sp);
            }
            speed = min(down_sp, up_sp);
        }
        if (speed < p.min_sp) speed = p.min_sp;

        return speed;
    }
};

///}@

#endif