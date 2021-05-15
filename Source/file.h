/*!
\file
\brief Модуль для работы с файлами (в разработке)
\todo Допилить файлы
*/

#ifndef file_H
#define file_H

#include <string>

#include "EV3_File.h"

#include "utils.h"

using namespace ev3_c_api;
using namespace std;

class File {
public:
    T_HandleFile headler;
    T_TimerId timerId;
    bool time;

    File(const char *name, bool _time = true, bool clear = false) {
        time = _time;
        if (clear) DeleteFile(name);
        headler = OpenFile(name, E_File_ReadWrite);
        if (headler == 0) headler = CreateFile(name);
        timerId = Timer_Start();
    }

    string getTime() {
        int time = Timer_GetTime(timerId);
        string ans;
        if (time > 60000) {
            ans += str(time / 60000);
            while (ans.size() < 3) ans = str(" ") + ans;
            ans += str(":");
        }
        if (time > 1000) {
            string seconds = str(time % 60000 / 1000);
            while (seconds.size() < 2) seconds = str("0") + seconds;
            ans += str(":");
        }
        string millis = str(time % 1000);
        while (millis.size() < 3) millis = str("0") + millis;
        while (ans.size() < 3 + 1 + 2 + 1 + 3) ans = str(" ") + ans;
        return ans;
    }

    void write(string text) {
        if (time) WriteFileString(headler, &((getTime() + "  " + text)[0]));
        else WriteFileString(headler, &((text)[0]));
    }

    ~File() {
        write(getTime() + str("    FILE CLOSED"));
        CloseFile(headler);
    }


    string readLine() {
        char buffer[10000];
        int len = ReadFileLine(headler, buffer, 10000);
        buffer[len] = '\0';
        return str(buffer);
    }

};

/*class DB {
public:
    T_HandleFile headler;
    T_TimerId timerId;
    bool time;
    DB(const char *name, bool _time = true, bool clear = false) {
        time = _time;
        if (clear) DeleteFile(name);
        headler = OpenFile(name, E_File_ReadWrite);
        if (headler == 0) headler = CreateFile(name);
        timerId = Timer_Start();
    }
    string getTime() {
        int time = Timer_GetTime(timerId);
        string ans;
        if (time > 60000) {
            ans += str(time / 60000);
            while (ans.size() < 3) ans = str(" ") + ans;
            ans += str(":");
        }
        if (time > 1000) {
            string seconds = str(time % 60000 / 1000);
            while (seconds.size() < 2) seconds = str("0") + seconds;
            ans += str(":");
        }
        string millis = str(time % 1000);
        while (millis.size() < 3) millis = str("0") + millis;
        while (ans.size() < 3 + 1 + 2 + 1 + 3) ans = str(" ") + ans;
        return ans;
    }
    void write(string text) {
        if (time) WriteFileString(headler, &((getTime() + "  " + text)[0]));
        else WriteFileString(headler, &((text)[0]));
    }
    ~DB() {
        write(getTime() + str("    FILE CLOSED"));
        CloseFile(headler);
    }
    string readLine() {
        char buffer[10000];
        int len = ReadFileLine(headler, buffer, 10000);
        buffer[len] = '\0';
        return str(buffer);
    }
};*/

#endif