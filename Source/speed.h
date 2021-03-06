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
    int maxS;    ///< Максимальная скорость
    int minS;    ///< Минимальная скорость
    double p;    ///< P коэффициент
    double d;    ///< D коэффициент
    int sEnc;    ///< Расстояние, которое робот разгоняется
    int eEnc;    ///< Расстояние, которое робот тормозит
    int zEnc;    ///< Сверх иновационное финишное движение (с минимальной скоростью) Столько последних он едет максимальное медленнно


    Speed() {} ///< Конструктор без параметров (не используй)

    /*!
    \param _maxS Максимальная скорость
    \param _minS Минимальная скорость
    \param _p P коэффициент
    \param _d D коэффициент
    \param _sEnc Расстояние, которое робот разгоняется
    \param _eEnc Расстояние, которое робот тормозит
    \param _zEnc Финишное медленное движение
    \return Возвращает Speed
    */
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

///}@

#endif