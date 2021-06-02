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
enum Clr  {
    NOREAD = -1,    ///< Не читали, не знаем что там
    NONE = 0,       ///< Там пусто
    YELLOW = 4,     ///< Желтый
    BLUE = 2,       ///< Голубой
    GREEN = 3,      ///< Зеленый
};

/*!
    \brief Хранит 2 объекта Color
*/
struct DM {
    Clr left;  ///< Цвет левого маркера
    Clr right; ///< Цвет правого маркера
    
    DM() : left(NOREAD), right(NOREAD) {} ///< Конструктор без параметров (не используй) (задает оба NOREAD)

    /*!
    \param _left Цвет левого маркера
    \param _right Цвет правого маркера
    \return Возвращает Двойной маркер
    */
    DM(Clr _left, Clr _right) : left(_left), right(_right) {}

    /*!
    \brief Для обратной совместимости с твоими уже сложившимися номерами цветов
    \param _left Цыфра левого маркера
    \param _right Цыфра правого маркера
    \return Возвращает Двойной маркер
    */
    DM(int _left, int _right) : left(Clr(_left)), right(Clr(_right)) {}
};



/*!
    \brief Структура для хранения поля
    \todo Добавить хранение состояния робота
    \todo Документация в разработке
*/
struct Field {
    DM house[4];       ///< Двойной маркер у 1 дома

    vector<Clr> hom1;
    vector<Clr> hom2;
    vector<Clr> hom3;

    int cnt1;                  ///< Количество кубиков в 1 доме
    int cnt2;                  ///< Количество кубиков в 2 доме
    int cnt3;                  ///< Количество кубиков в 3 доме
    int cntutils;

    Clr B;

    Clr yellowA1;            ///< Кубик в ближайшей желтой A зоне
    Clr yellowA2;
    Clr yellowA3;
    Clr yellowA4;

    Clr greenA1;
    Clr greenA2;
    Clr greenA3;
    Clr greenA4;

    Clr blueA1;
    Clr blueA2;
    Clr blueA3;
    Clr blueA4;

    Clr yellowB1;
    Clr yellowB2;
    Clr yellowB3;
    Clr yellowB4;

    Clr greenB1;
    Clr greenB2;
    Clr greenB3;
    Clr greenB4;

    Clr blueB1;
    Clr blueB2;
    Clr blueB3;
    Clr blueB4;
	
	struct Robot {
    Clr cfront1, cfront2, cback1, cback2;
    int how_front, how_back, now_position;

    Robot() {
        how_front = 0;
        cfront1 = NONE;
        cfront2 = NONE;
        cback1 = NONE;
        cback2 = NONE;
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

    void setYellowA(Clr color = YELLOW) {
        yellowA1 = yellowA2 = yellowA3 = yellowA4 = color;
    }

    void setGreenA(Clr color = GREEN) {
        greenA1 = greenA2 = greenA3 = greenA4 = color;
    }

    void setBlueA(Clr color = BLUE) {
        blueA1 = blueA2 = blueA3 = blueA4 = color;
    }

    void setYellowB(Clr color = YELLOW) {
        yellowB1 = yellowB2 = yellowB3 = yellowB4 = color;
    }

    void setGreenB(Clr color = GREEN) {
        greenB1 = greenB2 = greenB3 = greenB4 = color;
    }

    void setBlueB(Clr color = BLUE) {
        blueB1 = blueB2 = blueB3 = blueB4 = color;
    }

    Field(Mode newMode) : mode(newMode) {
        house[1] = house[2] = house[3] = DM(NOREAD, NOREAD);
        cnt1 = cnt2 = cnt3 = cntutils = 0;

        yellowA1 = yellowA2 = yellowA3 = yellowA4 = NOREAD;
        greenA1 = greenA2 = greenA3 = greenA4 = NOREAD;
        blueA1 = blueA2 = blueA3 = blueA4 = NOREAD;

        yellowB1 = yellowB2 = yellowB3 = yellowB4 = NOREAD;
        greenB1 = greenB2 = greenB3 = greenB4 = NOREAD;
        blueB1 = blueB2 = blueB3 = blueB4 = NOREAD;

        batteryLeft = batteryRight = DONOTKNOW;
    }

    void gdeb() {
        Clr ans = NONE;
        if (yellowB1 == YELLOW || yellowB2 == YELLOW || yellowB3 == YELLOW || yellowB4 == YELLOW) ans = YELLOW;
        if (greenB1 == GREEN || greenB2 == GREEN || greenB3 == GREEN || greenB4 == GREEN) ans = GREEN;
        if (blueB1 == BLUE || blueB2 == BLUE || blueB3 == BLUE || blueB4 == BLUE) ans = BLUE;
        B = ans;
    }

    int how_1(Clr clr) {
        int ans = 0;
        for (int i = 0; i < hom1.size(); ++i) if (hom1[i] = clr) ans++;
        return  ans;
    }
    int how_2(Clr clr) {
        int ans = 0;
        for (int i = 0; i < hom2.size(); ++i) if (hom2[i] = clr) ans++;
        return  ans;
    }
    int how_3(Clr clr) {
        int ans = 0;
        for (int i = 0; i < hom3.size(); ++i) if (hom3[i] = clr) ans++;
        return  ans;
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