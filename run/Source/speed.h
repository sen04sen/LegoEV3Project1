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
        max_sp = (char)_max_sp;
        min_sp = (char)_min_sp;
        p = _p;
        d = _d;
        normal_enc = _normal_enc;
        up_enc = _up_enc;
        down_enc = _end_enc;
        zero_enc = _zero_enc;
    }

    Speed(const Speed &copy) {
        max_sp  = copy.max_sp;
        min_sp = copy.max_sp;
        p = copy.p;
        d = copy.d;
        normal_enc = copy.normal_enc;
        up_enc = copy.up_enc;
        down_enc = copy.down_enc;
        zero_enc = copy.zero_enc;
    }

    Speed& operator=(const Speed& copy) {
        max_sp = copy.max_sp;
        min_sp = copy.max_sp;
        p = copy.p;
        d = copy.d;
        normal_enc = copy.normal_enc;
        up_enc = copy.up_enc;
        down_enc = copy.down_enc;
        zero_enc = copy.zero_enc;
    }

    int get_max_sp() { return (int)max_sp; }
    int get_min_sp() { return (int)min_sp; }
    double get_p() { return p; }
    double get_d() { return d; }
    int get_normal_enc() { return normal_enc; }
    int get_up_enc() { return up_enc; }
    int get_down_enc() { return down_enc; }
    int get_zero_enc() { return zero_enc; }
};

class Speed_compiled {
    Speed p;
    bool uping, downing;
    int way;
public:
    Speed_compiled(Speed _p, int _way, bool _uping = true, bool _downing = true) {
        p = _p;
        way = abs(_way);
        uping = _uping;
        downing = _downing;
    }

    int operator()(int now_dist) {
        int speed = p.get_max_sp();
        if (now_dist > way - p.get_zero_enc() && downing) speed = p.min_sp;
        else if (now_dist < p.get_normal_enc() && uping) speed = p.min_sp;
        else {
            int down_sp = p.get_max_sp(), up_sp = p.get_max_sp();
            if (now_dist > way - p.get_zero_enc() - p.get_down_enc() && downing) {
                down_sp = double(p.get_max_sp()) -
                          double(now_dist - (way - p.get_zero_enc() - p.get_down_enc())) * double(p.get_max_sp() - p.get_min_sp()) /
                          double(p.get_down_enc());
            }
            if (now_dist < p.get_normal_enc() + p.get_up_enc() && uping) {
                up_sp = double(now_dist - p.get_normal_enc()) * double(p.get_max_sp() - p.get_min_sp()) / double(p.get_up_enc()) +
                        double(p.get_min_sp());
            }
            speed = min(down_sp, up_sp);
        }
        if (speed < p.get_min_sp()) speed = p.get_min_sp();

        return speed;
    }
};

///}@

#endif