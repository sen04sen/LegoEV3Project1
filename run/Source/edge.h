/*!
\file
\brief Файл содержит модуль edge
*/

#ifndef edge_h
#define edge_h

#include "utils.h"

using namespace std;
using namespace ev3_c_api;

/*!
    \defgroup edge Ребро
    \brief Модуль, содержащей в себе класс ребра и функции для работы с ребрами
    \todo Доделать фуционал закрытия ребер по индексам
*/

///@{

/*!
	\brief Ребро

	\warning Тут не к чему не надо обращаться через точку. Так нельзя:
    \warning \code
    Edge e = Edge(...);
    e.to
    \endcode
    \warning Только через методы:
    \warning \code
    Edge e = Edge(...);
    e.getTo()
    \endcode
*/
class Edge {
    void (*def)(); ///< Указатель на функцию проезда по ребру

    short to;       ///< Номер вершины, куда ведет ребро

    char time;
    
    unsigned char index;

public:
    static vector<vector<Edge> > g; ///< Вектор для деикстры
    static int convert_ms;

    Edge() {} ///< Конструктор без параметров (не используй)

    void set_to(int _to) { to = (short)_to; }
    short get_to() { return to; }

    void set_def(void (* const _def)()) { def = _def; }
    
    void set_time(int _time) {
        short short_time = short(_time);
        time = abs(short_time);
        if (short_time > 126 * convert_ms)
            short_time = 126 * convert_ms;

        short_time = short(int32_t(short_time) / convert_ms);

        if (time > 0) time = char(short_time);
        else time == char(-short_time);
    }

    int get_time() {
        if (time > 0) 
            return short(convert_ms) * time;
        else 
            return 9999999;
    }

    void set_active(bool _active) {
        if (_active)
            time = abs(time);
        else
            time = -abs(time);
    }

    bool get_active() { return time > 0; }

    void set_index(short _index) { index = (unsigned char)_index; }
    short get_index() { return (short)index; }

    /*!
    \param _to Номер вершины, куда ведет ребро
    \param _def Указатель на функцию
    \param _time Время проезда по ребру (по умолчанию 1.0)
    \param _active Активно ли ребро (по умолчанию true)
    \param _index Специальное число, которое позволяет объединять ребра в гупппы и одновременно их блокировать,
        делая длинну очень большой, позволяет, также разблокировать ребра обратно (по умолчанию 0)
    \return Возвращает ребро
    */
    Edge(short _to, void (*const _def)(), short _time = 1000, bool _active = true, short _index = 0) {
        to = _to;
        def = _def;
        set_time(_time);
        set_active(_active);
        set_index(_index);
    }

    /*!
    \brief Запускает функцию проезда по ребру с перезаписью времени ребра
    \param reWriteTime Надо ли переписывать время (по умолчанию true)

    \warning Успользуй так:
    \warning \code
    Edge e = Edge(...);
    e();
    e(0);
    \endcode
    */
    void operator()(bool reWriteTime = true) {
        if (reWriteTime) {
            T_TimerId t = Timer_Start();
            def();
            set_time(Timer_Destroy(t));
        } else def();
    }


    /*!
    \brief Возвращает время ребра (если ребро неактивно, то возвращает 100000000.0)
    */


    void open() { set_active(true); }      ///< \brief Делает ребро активным

    void close() { set_active(false); }    ///< \brief Делает ребро неактивным

    static void close(short index) {
        for (int i = 0; i < g.size(); ++i)
            for (int j = 0; j < g[i].size(); ++j)
                if (g[i][j].get_index() == index)
                    g[i][j].close();
    }

    static void open(short index) {
        for (int i = 0; i < g.size(); ++i)
            for (int j = 0; j < g[i].size(); ++j)
                if (g[i][j].get_index() == index)
                    g[i][j].open();
    }

    static void close_point(short point) {
        for (int i = 0; i < g.size(); ++i)
            for (int j = 0; j < g[i].size(); ++j)
                if (g[i][j].get_to() == point)
                    g[i][j].close();
        for (int i = 0; i < g[point].size(); ++i)
            g[point][i].close();
    }

    static void open_point(short point) {
        for (int i = 0; i < g.size(); ++i)
            for (int j = 0; j < g[i].size(); ++j)
                if (g[i][j].get_to() == point)
                    g[i][j].open();
        for (int i = 0; i < g[point].size(); ++i)
            g[point][i].open();
    }

    static void change_def(short from, short to, void (* const _def)()) {
        bool doIt = 0;
        for (int i = 0; i < g.size(); ++i)
            if (i == from)
                for (int j = 0; j < g[i].size(); ++j)
                    if (g[i][j].get_to() == to) {
                        g[i][j].set_def(_def);
                        if (doIt)
                            throw Exception("Change2Def");
                        doIt = 1;
                    }
        if (!doIt)
            throw Exception("noForChangeDef");
    }

};
///@}

#endif