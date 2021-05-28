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
    short int to;       ///< Номер вершины, куда ведет ребро

    void (*def)(); ///< Указатель на функцию проезда по ребру

    //static vector<vector<Edge> > g; ///< Вектор для деикстры
public:
    Edge() {} ///< Конструктор без параметров (не используй)

    /*!
    \param _to Номер вершины, куда ведет ребро
    \param _def Указатель на функцию
    \param _time Время проезда по ребру (по умолчанию 1.0)
    \param _active Активно ли ребро (по умолчанию true)
    \param _index Специальное число, которое позволяет объединять ребра в гупппы и одновременно их блокировать,
        делая длинну очень большой, позволяет, также разблокировать ребра обратно (по умолчанию 0)
    \return Возвращает ребро
    */
    Edge(int _to, void (*const _def)(), double _time = 1.0, bool _active = true, int _index = 0) : to(_to),def(_def) {}

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
            def();
        } else def();
    }

    int getTo() { return (int)to; }          ///< \brief Возвращет то, куда ведет ребро

    /*!
    \brief Возвращает время ребра (если ребро неактивно, то возвращает 100000000.0)
    */
    double getTime() { return 1.0; }

    /*double getIndex() { return index; } ///< \brief Возвращает индекс ребра

    void open() { active = true; }      ///< \brief Делает ребро активным

    void close() { active = false; }    ///< \brief Делает ребро неактивным*/

    /*static void close(int index) {
        for (int i = 0; i < g.size(); ++i)
            for (int j = 0; j < g[i].size(); ++i) {
                if (g[i][j].index == index)
                    g[i][j].close();
            }
    }

    static void open(int index) {
        for (int i = 0; i < g.size(); ++i)
            for (int j = 0; j < g[i].size(); ++i) {
                if (g[i][j].index == index)
                    g[i][j].open();
            }
    }*/
};
///@}

#endif