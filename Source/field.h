/*!
\file
\brief Файл содержит модуль field и межмодульное преречисление Color
*/

#ifndef field_h
#define field_h

/*!
    \defgroup field Поле
    \brief Модуль, содержащей в себе класс поля и сопутствующие ему элементы
*/

///@{

/*!
    \brief Набор возможных состояний места для како-го либо элемента на поле, например маркера ли кубика
    Имеет обратную совместимость с твоими номерами цветов.
    Пример ииспользования:
    \code
    Color a = NOREAD;
    if (a == YELLOW) ...;
    a = GREEN;
    \endcode
*/
enum Color {
    NOREAD = -1,    ///< Не читали, не знаем что там
    NONE = 0,       ///< Там пусто
    YELLOW = 4,     ///< Желтый
    BLUE = 2,       ///< Голубой
    GREEN = 3,      ///< Зеленый
};

/*!
    \brief Хранит 2 объекта Color
*/
struct DoubleMarker {
    Color left;  ///< Цвет левого маркера
    Color right; ///< Цвет правого маркера

    DoubleMarker() : left(NOREAD), right(NOREAD) {} ///< Конструктор без параметров (не используй) (задает оба NOREAD)

    /*!
    \param _left Цвет левого маркера
    \param _right Цвет правого маркера
    \return Возвращает Двойной маркер
    */
    DoubleMarker(Color _left, Color _right) : left(_left), right(_right) {}

    /*!
    \brief Для обратной совместимости с твоими уже сложившимися номерами цветов
    \param _left Цыфра левого маркера
    \param _right Цыфра правого маркера
    \return Возвращает Двойной маркер
    */
    DoubleMarker(int _left, int _right) : left(Color(_left)), right(Color(_right)) {}
};



/*!
    \brief Структура для хранения поля
    \todo Добавить хранение состояния робота
    \todo Документация в разработке
*/
struct Field {
    DoubleMarker house1;       ///< Двойной маркер у 1 дома
    DoubleMarker house2;       ///< Двойной маркер у 2 дома
    DoubleMarker house3;       ///< Двойной маркер у 3 дома

    int cnt1;                  ///< Количество кубиков в 1 доме
    int cnt2;                  ///< Количество кубиков в 2 доме
    int cnt3;                  ///< Количество кубиков в 3 доме

    Color yellowA1;            ///< Кубик в ближайшей желтой A зоне
    Color yellowA2;
    Color yellowA3;
    Color yellowA4;

    Color greenA1;
    Color greenA2;
    Color greenA3;
    Color greenA4;

    Color blueA1;
    Color blueA2;
    Color blueA3;
    Color blueA4;


    Color yellowB1;
    Color yellowB2;
    Color yellowB3;
    Color yellowB4;

    Color greenB1;
    Color greenB2;
    Color greenB3;
    Color greenB4;

    Color blueB1;
    Color blueB2;
    Color blueB3;
    Color blueB4;
	
	struct Robot {
    Color front, back;
    int how_front, how_back, now_position;

    Robot() {
        front = NONE;
        how_front = 0;
        back = NONE;
        how_back = 0;
        now_position = -1;
    }
	};


    Robot robot;

    enum Battery {
        DONOTKNOW = -1,
        TURNED,
        UNTURNED,
    };

    Battery batteryLeft, batteryRight;

    enum Mode {
        STANDART,
        DONOTKNOWANY,
    };

    Mode mode;

    void setYellowA(Color color = YELLOW) {
        yellowA1 = yellowA2 = yellowA3 = yellowA4 = color;
    }

    void setGreenA(Color color = GREEN) {
        greenA1 = greenA2 = greenA3 = greenA4 = color;
    }

    void setBlueA(Color color = BLUE) {
        blueA1 = blueA2 = blueA3 = blueA4 = color;
    }

    void setYellowB(Color color = YELLOW) {
        yellowB1 = yellowB2 = yellowB3 = yellowB4 = color;
    }

    void setGreenB(Color color = GREEN) {
        greenB1 = greenB2 = greenB3 = greenB4 = color;
    }

    void setBlueB(Color color = BLUE) {
        blueB1 = blueB2 = blueB3 = blueB4 = color;
    }

    Field(Mode newMode) : mode(newMode) {
        house1 = house2 = house3 = DoubleMarker(NOREAD, NOREAD);
        cnt1 = cnt2 = cnt3 = 0;

        yellowA1 = yellowA2 = yellowA3 = yellowA4 = NOREAD;
        greenA1 = greenA2 = greenA3 = greenA4 = NOREAD;
        blueA1 = blueA2 = blueA3 = blueA4 = NOREAD;

        yellowB1 = yellowB2 = yellowB3 = yellowB4 = NOREAD;
        greenB1 = greenB2 = greenB3 = greenB4 = NOREAD;
        blueB1 = blueB2 = blueB3 = blueB4 = NOREAD;

        batteryLeft = batteryRight = DONOTKNOW;
    }

    int gdeb() {
        int ans = 0;
        if (yellowB1 == YELLOW || yellowB2 == YELLOW || yellowB3 == YELLOW || yellowB4 == YELLOW) ans = 4;
        if (greenB1 == GREEN || greenB2 == GREEN || greenB3 == GREEN || greenB4 == GREEN) ans = 3;
        if (blueB1 == BLUE || blueB2 == BLUE || blueB3 == BLUE || blueB4 == BLUE) ans = 2;
        return ans;
    }
};

Field StandartInit() {
    Field f = Field(Field::STANDART);
    f.setYellowA(YELLOW);
    f.setGreenA(GREEN);
    f.setBlueA(BLUE);
}
///@}

#endif