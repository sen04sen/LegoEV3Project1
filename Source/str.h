#ifndef str_h
#define str_h

using namespace std;

template<class T>
string str(T forConvert) {
    ostringstream ss;
    ss << forConvert;
    return ss.str();
}

#endif