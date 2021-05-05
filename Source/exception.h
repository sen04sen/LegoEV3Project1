#ifndef exception_h
#define exception_h

#include "str.h"

using namespace std;

class Exception {
    // Тут просто класс наших исключений
private:
    std::string m_error;

public:
    Exception(string error) : m_error(error) {}

    Exception(int line) : m_error(str(line)) {}

    const char *what() { return m_error.c_str(); }
};

#endif