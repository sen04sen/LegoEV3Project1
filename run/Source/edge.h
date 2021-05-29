/*!
\file
\brief Файл содержит модуль edge
*/

#ifndef edge_h
#define edge_h

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

    void set_to(short _to) { to = _to; }
    short get_to() { return to; }

    
    void set_time(short _time) {
        time = abs(_time);
        if (_time > 126 * convert_ms)
            _time = 126 * convert_ms;

        _time = _time / convert_ms;

        if (time > 0) time = char(_time);
        else time == char(-_time);
    }

    short get_time() {
        if (time > 0) 
            return short(convert_ms) * time;
        else 
            return 32767;
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
    Edge(short _to, void (*const _def)(), int _time = 1000, bool _active = true, short _index = 0) {
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

    double getIndex() { return index; } ///< \brief Возвращает индекс ребра

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

};
///@}

#endif