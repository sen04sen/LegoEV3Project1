#ifndef edge_h
#define edge_h

using namespace std;

class Edge {
    int to, index;
    double time;
    bool active;

    void (*def)();

    // index объединяет ребра в группы для одновременной активации или дезактивации

public:

    Edge() {}

    Edge(int newTo, void (*const newDef)(), double newTime = 1.0, bool newActive = true, int newIndex = 0) : to(newTo),
                                                                                                             def(newDef),
                                                                                                             time(newTime),
                                                                                                             active(newActive),
                                                                                                             index(newIndex) {}

    void operator()(bool reWriteTime = true) {
        if (reWriteTime) {
            T_TimerId id = Timer_Start();
            def();
            time = Timer_GetTime(id);
        } else def();
    }

    int getTo() { return to; }

    double getTime() { return active ? time : 100000000.0; }

    double getIndex() { return index; }

    void open() { active = true; }

    void close() { active = false; }

};

#endif