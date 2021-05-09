#ifndef pole_h
#define pole_h

enum Color {
    NOREAD = -1,
    NONE = 0,
    YELLOW = 4,
    BLUE = 2,
    GREEN = 3,
};

struct DoubleMarker {
    Color left, right;

    DoubleMarker() : left(NOREAD), right(NOREAD) {}

    DoubleMarker(Color newLeft, Color newRight) : left(newLeft), right(newRight) {}

    DoubleMarker(int newLeft, int newRight) : left(Color(newLeft)), right(Color(newRight)) {}
};


struct Field {
    DoubleMarker house1, house2, house3;
    int how1, how2, how3;

    Color yellowA1, yellowA2, yellowA3, yellowA4;
    Color greenA1, greenA2, greenA3, greenA4;
    Color blueA1, blueA2, blueA3, blueA4;

    Color yellowB1, yellowB2, yellowB3, yellowB4;
    Color greenB1, greenB2, greenB3, greenB4;
    Color blueB1, blueB2, blueB3, blueB4;

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
        how1 = how2 = how3 = 0;

        yellowA1 = yellowA2 = yellowA3 = yellowA4 = NOREAD;
        greenA1 = greenA2 = greenA3 = greenA4 = NOREAD;
        blueA1 = blueA2 = blueA3 = blueA4 = NOREAD;

        yellowB1 = yellowB2 = yellowB3 = yellowB4 = NOREAD;
        greenB1 = greenB2 = greenB3 = greenB4 = NOREAD;
        blueB1 = blueB2 = blueB3 = blueB4 = NOREAD;

        batteryLeft = batteryRight = DONOTKNOW;
    }
};

Field StandartInit() {
    Field f = Field(Field::STANDART);
    f.setYellowA(YELLOW);
    f.setGreenA(GREEN);
    f.setBlueA(BLUE);
}

#endif