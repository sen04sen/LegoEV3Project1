#ifndef constants_h
#define constants_h

const int maxv = 500;
const int ce = 21 * 2;
const int ver = 22;
const int black = 25;
const int bluck = 35;
const int bley = 45;
const double Pr = 0.3;
const int grey = 35;
const double dws = 130; // ���������� ����� ��������� � �������
const double dsl = 50; //����� �������� � �����
const int d90 = 250;
const int d180 = 500;
const int dovorot = 50;
const int turn1wheel = 525;
const int ndir = 0;
const int speed = 23;
const int speedD = 40;

vector<int> grad;

#define pb push_back

void buildDegreesConstants() {
    grad.pb(850);
    grad.pb(390);
    grad.pb(470);
    grad.pb(820);
    grad.pb(760);
    grad.pb(580);
    grad.pb(580);
    grad.pb(270);
    grad.pb(220);
    grad.pb(990);
    grad.pb(1100);
    grad.pb(880);
    grad.pb(470);
    grad.pb(250);
    grad.pb(670);
    grad.pb(230);
    grad.pb(210);
    grad.pb(410);
}

vector<vector<Edge> > g(maxv);

#endif