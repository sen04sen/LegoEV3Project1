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
void print(int y, IT uy) {
    string a = str(uy);
    Draw_Text(0, (11 - y) * 10, E_Font_Normal, 0, &(a[0]));
}

void clear() { Clear_Display(); }

/*!
\brief Функция, которая запускается потоком и завершает работу программы по кнопке
*/
void* okonchanie(void *_) {
    wait(1000);
    while (!isBrickButtonPressed(E_BTN_ESC));
    StopMotorAll();
    wait(300);
    exit(0);
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