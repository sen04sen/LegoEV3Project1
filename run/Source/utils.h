/*!
\file
\brief Файл содержит модуль utils
*/

#ifndef utils_h
#define utils_h

using namespace ev3_c_api;
using namespace std;

/*!
    \defgroup utils Полезные функции
    \brief Модуль, содержащей куче полезных функций для работы и отладки
*/

///@{

/// \brief Задержка
void wait(int time) {
    // время сна в миллисекундах
    EV3_Sleep(time);
}

/*!
\brief Делает string из чего угодно
\param forConvert Что-то, что можно через << выводить
\return string
*/
template<class IT>
string str(IT forConvert) {
    ostringstream ss;
    ss << forConvert;
    return ss.str();
}

/*!
	\brief Просто класс наших исключений (не пытайся разобраться)
*/
class Exception {
private:
    string m_error;
public:

    Exception(string error) : m_error(error) {}

    Exception(char *error) : m_error(str(error)) {}

    Exception(int line) : m_error(str(line)) {}

    string what() { return str(m_error); }
};

/*!
\brief Выводит что угодно на экран
\param uy Что-то для вывода
Работает аналогично str()
*/
template<class IT>
void write(int x, int y, IT uy) {
    string a = str(uy);
    Draw_Text(x, y, E_Font_Normal, 0, &(a[0]));
}

template<class IT>
void line_print(int y, IT uy) {
    string a = str(uy);
    Draw_Text(0, (11 - y) * 10, E_Font_Normal, 0, &(a[0]));
}

vector <string> docs = vector <string>();

void clear_docs() {
    docs.clear();
    Clear_Display();
}

short write_docs(short start = 0) {
    Clear_Display();
    start = max(start, (short)0);
    start = min(start, short(abs(int(docs.size() - 12))));
    try {
        for (int i = 0; i < 12; ++i)
            line_print(i, docs.at(docs.size() - 1 - start - i));
    }
    catch(...){}
    return start;
}

template<class IT>
void print(IT message) {
    string s = str(message);
    if (s.size() > 30) s.resize(30);
    docs.push_back(s);
    write_docs();
}

void read_docs() {
    int now = write_docs();
    while (true) {
        if (GetBrickButtonPressed() == 1) now--;
        else if (GetBrickButtonPressed() == 4) now++;
        wait(101);
        write_docs(now);
    }
}



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

///}@

/*

*/

#endif