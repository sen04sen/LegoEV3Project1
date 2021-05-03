//FUCKING SLAVE!!! Yes? vgnireeiv

#include <EV3_Motor.h>
#include "EV3_LCDDisplay.h"
#include "EV3_Sensor_Color.h"
#include "EV3_Sensor_UART.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "EV3_Thread.h"
#include <sstream>
#include <set>
#include "EV3_Timer.h"
#include "EV3_BrickUI.h"



using namespace ev3_c_api;
using namespace std;

#define pb push_back

const int maxv = 90;
int ce = 21 * 2;
int ver = 22;

struct edge {
	int dt, tp;
};

double stadegd;

double Pr = 0.3;
int wise = 130;
int proe = 50;
int d90 = 250;
int d180 = 500;
int black = 25;
int bluck = 35;
int grey = 35;
int bley = 45;
int dovorot = 50;
int pov1wheel = 525;

int ndir = 0;
int speed = 15;
int speedd = 15;

void wait(int time) {
	EV3_Sleep(time);
}

void stopb() {
	StopMotor(E_Port_B, 1);
}
void stopc() {
	StopMotor(E_Port_C, 1);
}
void stopd() {
	StopMotor(E_Port_D, 1);
}
void stopbc() {
	StopMotor(E_Port_BC, 1);
}
int s2() {
	return GetReflect(E_Port_2);
}
int s3() {
	return GetReflect(E_Port_3);
}
void startb(int sp) {
	SpeedMotor(E_Port_B, -1 * (sp));
}
void startc(int sp) {
	SpeedMotor(E_Port_C, sp);
}
void startd(int sp) {
	SpeedMotor(E_Port_D, sp);
}
void startbc(int sp, int uy = 0) {
	if (uy == 0) {
		SpeedMotor(E_Port_B, -1 * (sp));
		SpeedMotor(E_Port_C, sp);
	}
	else if (uy == 1) {
		SpeedMotor(E_Port_B, -1 * (sp));
		SpeedMotor(E_Port_C, -1 * sp);
	}
	else {
		SpeedMotor(E_Port_B, sp);
		SpeedMotor(E_Port_C, sp);
	}
}
void moveb(int sp, int dist, bool stop) {
	SpeedMotor(E_Port_B, -1 * (sp));
	double st = GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium);
	while (abs(GetMotor_RotationAngle(E_Port_B, E_MotorType_Medium) - st) < dist);
	if (stop)
		stopb();
}
void movec(int sp, int dist, bool stop) {
	SpeedMotor(E_Port_C, sp);
	double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
	while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dist);
	if (stop)
		stopc();
}
void moved(int sp, int dist) {
	dist = (double)dist + stadegd;
	double st = dist - GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium);
	if (st >= 0) {
		SpeedMotor(E_Port_D, sp);
		while (GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium) < dist);
	}
	else {
		SpeedMotor(E_Port_D, -sp);
		while (GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium) > dist);
	}
	startd(0);
}
void movebc(int sp, int dist, bool stop) {
	SpeedMotor(E_Port_B, -1 * (sp));
	SpeedMotor(E_Port_C, sp);
	double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
	while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dist);
	if (stop)
		stopbc();
}
void movebti(int sp, int time) {
	SpeedMotor_Time(E_Port_B, sp, time);
	wait(time);
	stopb();
}
void movecti(int sp, int time) {
	SpeedMotor_Time(E_Port_C, sp, time);
	wait(time);
	stopc();
}
void movedti(int sp, int time) {
	SpeedMotor_Time(E_Port_D, sp, time);
	wait(time);
	stopd();
}
void movebcti(int sp, int time) {
	SpeedMotor_Time(E_Port_BC, sp, time);
	wait(time);
	stopbc();
}

vector<vector<pair<int, int > > > g;
void buildg() {
	vector<pair<int, int> > vec;

	vec.pb(make_pair(1, 49));
	vec.pb(make_pair(2, 51));
	vec.pb(make_pair(2, 52));
	vec.pb(make_pair(3, 54));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(0, 42));
	vec.pb(make_pair(2, 53));
	vec.pb(make_pair(3, 55));
	vec.pb(make_pair(3, 56));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(6, 0));
	vec.pb(make_pair(0, 43));
	vec.pb(make_pair(0, 44));
	vec.pb(make_pair(1, 46));
	vec.pb(make_pair(3, 57));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(0, 45));
	vec.pb(make_pair(1, 47));
	vec.pb(make_pair(1, 48));
	vec.pb(make_pair(2, 50));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(0, 1));
	vec.pb(make_pair(5, 65));
	vec.pb(make_pair(6, 67));
	vec.pb(make_pair(6, 68));
	vec.pb(make_pair(7, 70));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(9, 2));
	vec.pb(make_pair(4, 58));
	vec.pb(make_pair(6, 69));
	vec.pb(make_pair(7, 71));
	vec.pb(make_pair(7, 72));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(4, 59));
	vec.pb(make_pair(4, 60));
	vec.pb(make_pair(5, 62));
	vec.pb(make_pair(7, 73));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(4, 61));
	vec.pb(make_pair(5, 63));
	vec.pb(make_pair(5, 64));
	vec.pb(make_pair(6, 66));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(9, 81));
	vec.pb(make_pair(10, 83));
	vec.pb(make_pair(10, 84));
	vec.pb(make_pair(11, 86));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(17, 6));
	vec.pb(make_pair(8, 74));
	vec.pb(make_pair(10, 85));
	vec.pb(make_pair(11, 87));
	vec.pb(make_pair(11, 88));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(14, 4));
	vec.pb(make_pair(8, 75));
	vec.pb(make_pair(8, 76));
	vec.pb(make_pair(9, 78));
	vec.pb(make_pair(11, 89));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(7, 3));
	vec.pb(make_pair(8, 77));
	vec.pb(make_pair(9, 79));
	vec.pb(make_pair(9, 80));
	vec.pb(make_pair(10, 82));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(8, 5));
	vec.pb(make_pair(13, 97));
	vec.pb(make_pair(14, 99));
	vec.pb(make_pair(14, 100));
	vec.pb(make_pair(15, 102));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(12, 90));
	vec.pb(make_pair(14, 101));
	vec.pb(make_pair(15, 103));
	vec.pb(make_pair(15, 104));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(12, 91));
	vec.pb(make_pair(12, 92));
	vec.pb(make_pair(13, 94));
	vec.pb(make_pair(15, 105));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(12, 93));
	vec.pb(make_pair(13, 95));
	vec.pb(make_pair(13, 96));
	vec.pb(make_pair(14, 98));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(17, 113));
	vec.pb(make_pair(18, 115));
	vec.pb(make_pair(18, 116));
	vec.pb(make_pair(19, 118));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(21, 8));
	vec.pb(make_pair(16, 106));
	vec.pb(make_pair(18, 117));
	vec.pb(make_pair(19, 119));
	vec.pb(make_pair(19, 120));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(16, 107));
	vec.pb(make_pair(16, 108));
	vec.pb(make_pair(17, 110));
	vec.pb(make_pair(19, 121));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(11, 7));
	vec.pb(make_pair(16, 109));
	vec.pb(make_pair(17, 111));
	vec.pb(make_pair(17, 112));
	vec.pb(make_pair(18, 114));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(24, 10));
	vec.pb(make_pair(21, 129));
	vec.pb(make_pair(22, 131));
	vec.pb(make_pair(22, 132));
	vec.pb(make_pair(23, 134));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(37, 16));
	vec.pb(make_pair(20, 122));
	vec.pb(make_pair(22, 133));
	vec.pb(make_pair(23, 135));
	vec.pb(make_pair(23, 136));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(20, 123));
	vec.pb(make_pair(20, 124));
	vec.pb(make_pair(21, 126));
	vec.pb(make_pair(23, 137));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(19, 9));
	vec.pb(make_pair(20, 125));
	vec.pb(make_pair(21, 127));
	vec.pb(make_pair(21, 128));
	vec.pb(make_pair(22, 130));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(25, 145));
	vec.pb(make_pair(26, 147));
	vec.pb(make_pair(26, 148));
	vec.pb(make_pair(27, 150));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(33, 14));
	vec.pb(make_pair(24, 138));
	vec.pb(make_pair(26, 149));
	vec.pb(make_pair(27, 151));
	vec.pb(make_pair(27, 152));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(22, 11));
	vec.pb(make_pair(24, 139));
	vec.pb(make_pair(24, 140));
	vec.pb(make_pair(25, 142));
	vec.pb(make_pair(27, 153));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(31, 12));
	vec.pb(make_pair(24, 141));
	vec.pb(make_pair(25, 143));
	vec.pb(make_pair(25, 144));
	vec.pb(make_pair(26, 146));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(29, 161));
	vec.pb(make_pair(30, 163));
	vec.pb(make_pair(30, 164));
	vec.pb(make_pair(31, 166));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(25, 13));
	vec.pb(make_pair(28, 154));
	vec.pb(make_pair(30, 165));
	vec.pb(make_pair(31, 167));
	vec.pb(make_pair(31, 168));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(28, 155));
	vec.pb(make_pair(28, 156));
	vec.pb(make_pair(29, 158));
	vec.pb(make_pair(31, 169));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(28, 157));
	vec.pb(make_pair(29, 159));
	vec.pb(make_pair(29, 160));
	vec.pb(make_pair(30, 162));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(33, 177));
	vec.pb(make_pair(34, 179));
	vec.pb(make_pair(34, 180));
	vec.pb(make_pair(35, 182));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(32, 170));
	vec.pb(make_pair(34, 181));
	vec.pb(make_pair(35, 183));
	vec.pb(make_pair(35, 184));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(32, 171));
	vec.pb(make_pair(32, 172));
	vec.pb(make_pair(33, 174));
	vec.pb(make_pair(35, 185));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(27, 15));
	vec.pb(make_pair(32, 173));
	vec.pb(make_pair(33, 175));
	vec.pb(make_pair(33, 176));
	vec.pb(make_pair(34, 178));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(37, 193));
	vec.pb(make_pair(38, 195));
	vec.pb(make_pair(38, 196));
	vec.pb(make_pair(39, 198));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(45, 20));
	vec.pb(make_pair(36, 186));
	vec.pb(make_pair(38, 197));
	vec.pb(make_pair(39, 199));
	vec.pb(make_pair(39, 200));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(42, 18));
	vec.pb(make_pair(36, 187));
	vec.pb(make_pair(36, 188));
	vec.pb(make_pair(37, 190));
	vec.pb(make_pair(39, 201));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(23, 17));
	vec.pb(make_pair(36, 189));
	vec.pb(make_pair(37, 191));
	vec.pb(make_pair(37, 192));
	vec.pb(make_pair(38, 194));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(36, 19));
	vec.pb(make_pair(41, 209));
	vec.pb(make_pair(42, 211));
	vec.pb(make_pair(42, 212));
	vec.pb(make_pair(43, 214));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(40, 202));
	vec.pb(make_pair(42, 213));
	vec.pb(make_pair(43, 215));
	vec.pb(make_pair(43, 216));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(40, 203));
	vec.pb(make_pair(40, 204));
	vec.pb(make_pair(41, 206));
	vec.pb(make_pair(43, 217));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(40, 205));
	vec.pb(make_pair(41, 207));
	vec.pb(make_pair(41, 208));
	vec.pb(make_pair(42, 210));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(45, 225));
	vec.pb(make_pair(46, 227));
	vec.pb(make_pair(46, 228));
	vec.pb(make_pair(47, 230));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(49, 22));
	vec.pb(make_pair(44, 218));
	vec.pb(make_pair(46, 229));
	vec.pb(make_pair(47, 231));
	vec.pb(make_pair(47, 232));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(44, 219));
	vec.pb(make_pair(44, 220));
	vec.pb(make_pair(45, 222));
	vec.pb(make_pair(47, 233));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(39, 21));
	vec.pb(make_pair(44, 221));
	vec.pb(make_pair(45, 223));
	vec.pb(make_pair(45, 224));
	vec.pb(make_pair(46, 226));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(52, 24));
	vec.pb(make_pair(49, 241));
	vec.pb(make_pair(50, 243));
	vec.pb(make_pair(50, 244));
	vec.pb(make_pair(51, 246));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(57, 26));
	vec.pb(make_pair(48, 234));
	vec.pb(make_pair(50, 245));
	vec.pb(make_pair(51, 247));
	vec.pb(make_pair(51, 248));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(48, 235));
	vec.pb(make_pair(48, 236));
	vec.pb(make_pair(49, 238));
	vec.pb(make_pair(51, 249));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(47, 23));
	vec.pb(make_pair(48, 237));
	vec.pb(make_pair(49, 239));
	vec.pb(make_pair(49, 240));
	vec.pb(make_pair(50, 242));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(53, 257));
	vec.pb(make_pair(54, 259));
	vec.pb(make_pair(54, 260));
	vec.pb(make_pair(55, 262));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(52, 250));
	vec.pb(make_pair(54, 261));
	vec.pb(make_pair(55, 263));
	vec.pb(make_pair(55, 264));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(50, 25));
	vec.pb(make_pair(52, 251));
	vec.pb(make_pair(52, 252));
	vec.pb(make_pair(53, 254));
	vec.pb(make_pair(55, 265));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(52, 253));
	vec.pb(make_pair(53, 255));
	vec.pb(make_pair(53, 256));
	vec.pb(make_pair(54, 258));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(57, 273));
	vec.pb(make_pair(58, 275));
	vec.pb(make_pair(58, 276));
	vec.pb(make_pair(59, 278));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(65, 30));
	vec.pb(make_pair(56, 266));
	vec.pb(make_pair(58, 277));
	vec.pb(make_pair(59, 279));
	vec.pb(make_pair(59, 280));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(62, 28));
	vec.pb(make_pair(56, 267));
	vec.pb(make_pair(56, 268));
	vec.pb(make_pair(57, 270));
	vec.pb(make_pair(59, 281));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(51, 27));
	vec.pb(make_pair(56, 269));
	vec.pb(make_pair(57, 271));
	vec.pb(make_pair(57, 272));
	vec.pb(make_pair(58, 274));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(56, 29));
	vec.pb(make_pair(61, 289));
	vec.pb(make_pair(62, 291));
	vec.pb(make_pair(62, 292));
	vec.pb(make_pair(63, 294));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(60, 282));
	vec.pb(make_pair(62, 293));
	vec.pb(make_pair(63, 295));
	vec.pb(make_pair(63, 296));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(60, 283));
	vec.pb(make_pair(60, 284));
	vec.pb(make_pair(61, 286));
	vec.pb(make_pair(63, 297));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(60, 285));
	vec.pb(make_pair(61, 287));
	vec.pb(make_pair(61, 288));
	vec.pb(make_pair(62, 290));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(68, 32));
	vec.pb(make_pair(65, 305));
	vec.pb(make_pair(66, 307));
	vec.pb(make_pair(66, 308));
	vec.pb(make_pair(67, 310));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(85, 40));
	vec.pb(make_pair(64, 298));
	vec.pb(make_pair(66, 309));
	vec.pb(make_pair(67, 311));
	vec.pb(make_pair(67, 312));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(64, 299));
	vec.pb(make_pair(64, 300));
	vec.pb(make_pair(65, 302));
	vec.pb(make_pair(67, 313));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(59, 31));
	vec.pb(make_pair(64, 301));
	vec.pb(make_pair(65, 303));
	vec.pb(make_pair(65, 304));
	vec.pb(make_pair(66, 306));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(72, 34));
	vec.pb(make_pair(69, 321));
	vec.pb(make_pair(70, 323));
	vec.pb(make_pair(70, 324));
	vec.pb(make_pair(71, 326));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(68, 314));
	vec.pb(make_pair(70, 325));
	vec.pb(make_pair(71, 327));
	vec.pb(make_pair(71, 328));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(66, 33));
	vec.pb(make_pair(68, 315));
	vec.pb(make_pair(68, 316));
	vec.pb(make_pair(69, 318));
	vec.pb(make_pair(71, 329));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(68, 317));
	vec.pb(make_pair(69, 319));
	vec.pb(make_pair(69, 320));
	vec.pb(make_pair(70, 322));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(80, 38));
	vec.pb(make_pair(73, 337));
	vec.pb(make_pair(74, 339));
	vec.pb(make_pair(74, 340));
	vec.pb(make_pair(75, 342));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(72, 330));
	vec.pb(make_pair(74, 341));
	vec.pb(make_pair(75, 343));
	vec.pb(make_pair(75, 344));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(70, 35));
	vec.pb(make_pair(72, 331));
	vec.pb(make_pair(72, 332));
	vec.pb(make_pair(73, 334));
	vec.pb(make_pair(75, 345));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(79, 36));
	vec.pb(make_pair(72, 333));
	vec.pb(make_pair(73, 335));
	vec.pb(make_pair(73, 336));
	vec.pb(make_pair(74, 338));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(77, 353));
	vec.pb(make_pair(78, 355));
	vec.pb(make_pair(78, 356));
	vec.pb(make_pair(79, 358));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(73, 37));
	vec.pb(make_pair(76, 346));
	vec.pb(make_pair(78, 357));
	vec.pb(make_pair(79, 359));
	vec.pb(make_pair(79, 360));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(76, 347));
	vec.pb(make_pair(76, 348));
	vec.pb(make_pair(77, 350));
	vec.pb(make_pair(79, 361));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(76, 349));
	vec.pb(make_pair(77, 351));
	vec.pb(make_pair(77, 352));
	vec.pb(make_pair(78, 354));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(81, 369));
	vec.pb(make_pair(82, 371));
	vec.pb(make_pair(82, 372));
	vec.pb(make_pair(83, 374));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(80, 362));
	vec.pb(make_pair(82, 373));
	vec.pb(make_pair(83, 375));
	vec.pb(make_pair(83, 376));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(74, 39));
	vec.pb(make_pair(80, 363));
	vec.pb(make_pair(80, 364));
	vec.pb(make_pair(81, 366));
	vec.pb(make_pair(83, 377));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(80, 365));
	vec.pb(make_pair(81, 367));
	vec.pb(make_pair(81, 368));
	vec.pb(make_pair(82, 370));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(85, 385));
	vec.pb(make_pair(86, 387));
	vec.pb(make_pair(86, 388));
	vec.pb(make_pair(87, 390));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(84, 378));
	vec.pb(make_pair(86, 389));
	vec.pb(make_pair(87, 391));
	vec.pb(make_pair(87, 392));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(84, 379));
	vec.pb(make_pair(84, 380));
	vec.pb(make_pair(85, 382));
	vec.pb(make_pair(87, 393));
	g.pb(vec);
	vec.clear();

	vec.pb(make_pair(67, 41));
	vec.pb(make_pair(84, 381));
	vec.pb(make_pair(85, 383));
	vec.pb(make_pair(85, 384));
	vec.pb(make_pair(86, 386));
	g.pb(vec);
	vec.clear();
}

edge make_edge(int dist, int tp) {
	edge f = { dist, tp };
	return f;
}
vector<edge> ed;
void builded() {
	ed.pb(make_edge(750, 7));
	ed.pb(make_edge(750, 4));
	ed.pb(make_edge(270, 6));
	ed.pb(make_edge(270, 6));
	ed.pb(make_edge(290, 8));
	ed.pb(make_edge(290, 5));
	ed.pb(make_edge(100, 4));
	ed.pb(make_edge(100, 7));
	ed.pb(make_edge(490, 7));
	ed.pb(make_edge(490, 4));
	ed.pb(make_edge(650, 5));
	ed.pb(make_edge(650, 5));
	ed.pb(make_edge(490, 4));
	ed.pb(make_edge(490, 6));
	ed.pb(make_edge(490, 4));
	ed.pb(make_edge(490, 7));
	ed.pb(make_edge(170, 6));
	ed.pb(make_edge(170, 6));
	ed.pb(make_edge(120, 4));
	ed.pb(make_edge(120, 5));
	ed.pb(make_edge(400, 4));
	ed.pb(make_edge(400, 7));
	ed.pb(make_edge(380, 7));
	ed.pb(make_edge(380, 4));
	ed.pb(make_edge(650, 8));
	ed.pb(make_edge(650, 5));
	ed.pb(make_edge(760, 6));
	ed.pb(make_edge(760, 6));
	ed.pb(make_edge(300, 8));
	ed.pb(make_edge(300, 5));
	ed.pb(make_edge(160, 7));
	ed.pb(make_edge(160, 7));
	ed.pb(make_edge(400, 4));
	ed.pb(make_edge(400, 5));
	ed.pb(make_edge(50, 7));
	ed.pb(make_edge(50, 4));
	ed.pb(make_edge(110, 4));
	ed.pb(make_edge(110, 5));
	ed.pb(make_edge(100, 4));
	ed.pb(make_edge(100, 6));
	ed.pb(make_edge(270, 4));
	ed.pb(make_edge(270, 6));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, -2));
	ed.pb(make_edge(d180, -2));
	ed.pb(make_edge(d180, -1));
	ed.pb(make_edge(d90, -1));
	ed.pb(make_edge(d90, 3));
	ed.pb(make_edge(d180, 2));
	ed.pb(make_edge(d180, 1));
	ed.pb(make_edge(d90, 0));
}

pair<pair<int, int>, int> grgb(int uy) {
	const void* a;
	if (uy == 3) {
		a = GetData_UART(E_Port_3, E_UART_Type_Color, 4);
	}
	else if (uy == 4) {
		a = GetData_UART(E_Port_4, E_UART_Type_Color, 4);
	}
	else if (uy == 2) {
		a = GetData_UART(E_Port_2, E_UART_Type_Color, 4);
	}
	unsigned char* d = reinterpret_cast<unsigned char*>(const_cast<void*>(a));
	int r = d[0];
	int g = d[2];
	int b = d[4];
	return make_pair( make_pair(r, g), b );
}

void line(int sp, int dist, int tp) {
	bool stop = 0;
	if (tp == 4) {
		grgb(3);
		EV3_Sleep(50);
	}
	double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
	if (tp == 4)
		grgb(3);
	while (!stop) {
		if (tp == 0 || tp == 6 || tp == 8) {
			if (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st > dist) {
				stop = 1;
			}
		}
		else {
			if (GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st > dist - 50) {
				if (tp == 1 && s2() < black && s3() < black)
					stop = 1;
				else if (tp == 2 && s3() < black)
					stop = 1;
				else if (tp == 3 && s2() < black)
					stop = 1;
				else if (tp == 4) {
					pair<int, int> rer = grgb(3).first;
					if (rer.first - rer.second > 70)
						stop = 1;
				}
				else if (tp == 5 && s2() > bluck)
					stop = 1;
				else if (tp == 7 && s3() < black)
					stop = 1;
			}
		}
		double del;
		if (tp == 5 || tp == 6) {
			del = (double)(s3() - bley) * Pr * 3 / 3;
		}
		else if (tp == 4) {
			del = (double)(grey - s2()) * Pr * 2 / 3;
		}
		else if (tp == 7 || tp == 8) {
			del = (double)(s2() - grey) * Pr * 3 / 3;
		}
		else
			del = (double)(s3() - s2()) * Pr;
		SpeedMotor(E_Port_B, -1 * (sp - del));
		SpeedMotor(E_Port_C, sp + del);
	}
	if (tp == 4)
		s3();
}

void pov(int sp, int dt, int tp) {
	if (tp >= 0) {
		if (tp < 2 || tp == 4) {
			SpeedMotor(E_Port_B, -1 * (sp));
			SpeedMotor(E_Port_C, -1 * (sp));
		}
		else {
			SpeedMotor(E_Port_B, sp);
			SpeedMotor(E_Port_C, sp);
		}
		
		double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
		while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt - 80);
		if (tp == 4)
			while (s3() > bluck);
		else if (tp == 5)
			while (s2() > bluck);
		else if (tp < 2) {
			while (s3() > black);
		}
		else {
			while (s2() > black);
		}
		st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
		while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dovorot);
		stopbc();
	}
	else {
		if (tp == -1) {
			SpeedMotor(E_Port_B, -1 * (sp));
			SpeedMotor(E_Port_C, -1 * (sp));
		}
		else {
			SpeedMotor(E_Port_B, sp);
			SpeedMotor(E_Port_C, sp);
		}
		double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
		while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < dt);
		stopbc();
	}
}

int gclr(int uy) {
	if (uy == 4) {
		return GetColor(E_Port_4);
	}
	else {
		return GetColor(E_Port_3);
	}
}

bool write(int x, int y, int uy) {
	stringstream s;
	s << uy;
	string a;
	s >> a;
	Draw_Text(x, y, E_Font_Normal, 0, &(a[0]));
}


pair<int, int> msgo[maxv];
int go(int sp, int fromm, int dd1, int totoo, int dd2, bool lst = 1) {
	int from = fromm * 4 + dd1;
	int toto = totoo * 4 + dd2;
	for (int i = 0; i < maxv; i++) {
		msgo[i].first = 0;
		msgo[i].second = -1;
	}
	msgo[from] = make_pair(-1, 0);
	set <pair<int, int> > st;
	st.insert(make_pair(0, from));
	bool end = 0;
	while (!st.empty()) {
		int dd = st.begin()->first;
		int v = st.begin()->second;
		st.erase(st.begin());
		for (int i = 0; i < g[v].size(); i++) {
			int to = g[v][i].first;
			if (msgo[to].second == -1) {
				msgo[to].first = v;
				msgo[to].second = g[v][i].second;
				if (to == toto) {
					end = 1;
					break;
				}
				st.insert(make_pair(dd + ed[msgo[to].second].dt, to));
			}
		}
		if (end)
			break;
	}
	vector<int> way;
	int nv = toto;
	while (nv != from) {
		way.pb(msgo[nv].second);
		nv = msgo[nv].first;
	}
	int kak = 1;
	for (int i = way.size() - 1; i >= 0; i--) {
		int nw = way[i];
		write(kak, 1, nw);
		kak += 20;
		int del = 0;
		if (lst == 0) {
			if (ed[nw].tp >= 4) {
				movebc(sp, proe, 0);
				del = wise - proe;
			}
			else if (ed[nw].tp >= -2)
				movebc(sp, wise, 1);
			else
				movebc(sp, wise, 0);
		}
		if (ed[nw].tp == -3) {
			movebc(sp, ed[nw].dt + del, 0);
			lst = 0;
			continue;
		}
		if (ed[nw].tp < 4) {
			pov(sp, ed[nw].dt, ed[nw].tp);
			lst = 1;
			continue;
		}
		line(sp, ed[nw].dt + del, ed[nw].tp - 4);
		lst = 0;
	}
	return way.size();
}

void vivod_4() {
	for (int i = 0; i < 100; i++) {
		const void* a = GetData_UART(E_Port_3, E_UART_Type_Color, 4);
		unsigned char* d = reinterpret_cast<unsigned char*>(const_cast<void*>(a));
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

void give2() {
	stopbc();
	stopd();
	wait(30);
	moved(-speedd, 400);
	movebc(speed, 200, 1);
	movebc(-speed, 110, 1);
	moved(speedd, 400);
	movedti(1, 500);
	stopd();
	moved(-5, 8);
	wait(100);
	movebc(-speed, 150, 1);
	moved(-speedd, 100);
	movebc(speed, 60, 1);
	moved(speedd, 120);
	startd(3);
}

void give4() {
	stopbc();
	stopd();
	wait(30);
	moved(-speedd, 400);
	movebc(speed, 200, 1);
	movebc(-speed, 200, 1);
	moved(speedd, 400);
}

pair<int, int> gtf() {
	movebc(speed, 35, 1);
	SpeedMotor(E_Port_C, -speed);
	double st = GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium);
	while (abs(GetMotor_RotationAngle(E_Port_C, E_MotorType_Medium) - st) < pov1wheel);
	stopc();
	movebc(speed, 365, 1);
	GetColor(E_Port_4);
	EV3_Sleep(500);
	int fi = gclr(4);
	if (fi == 7)
		fi = 4;
	movebc(speed, 120, 1);
	EV3_Sleep(500);
	int se = gclr(4);
	if (se == 7)
		se = 4;
	stopbc();
	Clear_Display();
	write(1, 1, fi);
	write(51, 1, se);
	return make_pair(fi, se);
}

void gtb() {
	movebc(-speed, 330, 1);
	pov(speed, d90, 0);
	line(speed, 50, 1);
}

pair<int, int> d1;
pair<int, int> d2;
pair<int, int> d3;
bool p1 = 0, p2 = 0, p3 = 0;
double st;
int gdeb = 3;

void end_4_green() {
	line(speed, 170, 2);
	movebc(-speed, 50, 1);
	pov(speed, d90, -1);
	movebc(10, 140, 1);
	moved(-speedd, 70);
	movebc(-speed, 140, 1);
	pov(speed, d90, 3);
	line(speed, 30, 2);
	movebc(speed, proe, 1);
	line(speed, 220, 0);
	stopbc();
	pov(speed, d90, -1);
	moved(10, 65);
	EV3_Sleep(1000);
	movebc(10, 140, 1);
	moved(-speedd, 65);
	movebc(-speed, 140, 1);
	pov(speed, d90, 3);
	line(speed, 400, 3);
	if (d1.first == 3 || d1.second == 3) {
		movebc(speed, wise, 1);
		pov(speed, d180, 1);
		line(speed, 780, 3);
		movebc(speed, proe, 0);
		line(speed, 170, 2);
		movebc(speed, proe, 0);
		line(speed, 590, 3);
		movebc(speed, wise, 1);
		pov(speed, d90, 3);
		line(speed, 200, 4);
		movebc(speed, 50, 1);
		moved(10, 110);
		movebc(-speed, 400, 1);
		pov(speed, d90, 0);
		SpeedMotor_Time(E_Port_D, -30, 2000);
		wait(2000);
		stopd();
		line(speed, 270, 2);
		movebc(speed, wise, 1);
		pov(speed, d90, 0);
		line(speed, 750, 0);
		movebc(speed, 300, 1);
		wait(10000);
	}
	else {
		movebc(speed, proe, 0);
		line(speed, 760, 2);
		movebc(speed, wise, 1);
		pov(speed, d90, 0);
		line(speed, 200, 4);
		movebc(speed, 50, 1);
		moved(10, 110);
		movebc(-speed, 400, 1);
		pov(speed, d90, 0);
		SpeedMotor_Time(E_Port_D, -30, 2000);
		wait(2000);
		stopd();
		line(speed, 760, 2);
		movebc(speed, proe, 0);
		line(speed, 780, 3);
		movebc(speed, proe, 0);
		line(speed, 170, 2);
		movebc(speed, proe, 0);
		line(speed, 590, 3);
		movebc(speed, proe, 0);
		line(speed, 270, 2);
		movebc(speed, wise, 1);
		pov(speed, d90, 0);
		line(speed, 750, 0);
		movebc(speed, 300, 1);
		wait(10000);
	}
}

void pov_bat() {
	stopbc();
	movec(speed, 80, 1);
	moveb(speed, 80, 1);
	movebc(speed, 130, 1);
	moved(speedd, 220);
	moveb(-speed, 30, 1);
	moved(speedd, 250);
	movebc(-speed, 90, 1);
	moved(speedd, 500);
	pov(speed, d90 - 20, 3);
	line(speed, 330, 0);
	stopbc();
	pov(speed, d90, -1);
	movebc(-speed, 60, 1);
	moved(speedd, 20);
	movebc(speed, 170, 1);
	moved(speedd, 220);
	moveb(-speed, 30, 1);
	moved(speedd, 250);
	movebc(-speed, 90, 1);
	moved(speedd, 500);
	movebc(-speed, 60, 1);
	pov(speed, d90 - 30, 0);
	pov(speed, 60, -1);
	line(speed, 70, 7);
	movebc(speed, proe, 0);
	line(speed, 550, 8);
	grgb(2);
	movebc(speed, 60, 0);
	startbc(speed);
	while (grgb(2).second < 100);
	s2();
	movebc(speed, 70, 1);
	pov(speed, d90, -2);
}

void get_4_blue() {
	movebc(speed, 40, 1);
	pov(speed, d90, -2);
	movebc(speed, 50, 0);
	line(speed, 460, 5);
	startbc(speed);
	while (s2() > black);
	stopbc();
	movebc(-speed, 30, 1);
	pov(speed, d90, -1);
	movebc(-speed, 50, 1);
	moved(speedd, 395);
	movebc(speed, 90, 1);
	moved(speedd, 280);
	movebc(-speed, 30, 1);
	pov(speed, d90, -2);
	movebc(speed, 130, 1);
	startbc(speed, 2);
	while (s3() > bluck);
	stopbc();
	line(speed, 150, 6);
	stopbc();
	pov(speed, d90, -1);
	movebc(-speed, 50, 1);
	moved(speedd, 395);
	movebc(speed, 90, 1);
	moved(speedd, 280);
	movebc(-speed, 60, 1);
	pov(speed, d90, -1);
	movebc(speed, 600, 0);
	startbc(speed);
	while (s2() > black);
	stopbc();
}

void* okonchanie(void* lpvoid) {
	while (!isBrickButtonPressed(E_BTN_ESC));
	exit(0);
}


signed EV3_main() {
	startd(-4);
	wait(1000);
	stadegd = GetMotor_RotationAngle(E_Port_D, E_MotorType_Medium);
	startd(0);
	CreateThread(okonchanie, 0);
	Clear_Display();
	pov_bat();
	return 0;
	movebc(speed, 250, 0);
	stopb();
	movec(speed, 220, 1);
	moved(speedd, 450);
	pov(speed, 120, -1);
	moved(-speedd, 100);
	movebc(speed, 10, 0);
	stopc();
	moveb(speed, 790, 1);
	moved(speedd, 100);
	startd(3);
	pov(speed, 50, -1);
	if (gclr(4) != 0) {
		gdeb = 4;
	}
	movebc(speed, 375, 0);
	startbc(speed);
	while (s3() > black);
	movebc(speed, wise + 20, 1);
	pov(speed, 130, 3);
	line(speed, 200, 3);
	go(speed, 1, 2, 3, 2, 0);
	d1 = gtf();
	gtb();
	if (d1.first == 4 || d1.second == 4) {
		stopbc();
		pov(speed, d180, 2);
		line(speed, 200, 4);
		p1 = 1;
		give4();
		pov(speed, d180, 2);
		line(speed, 250, 1);
	}
	go(speed, 2, 0, 6, 0, 0);
	pov_bat();
	moved(-speedd, 140);	
	movebc(speed, 320, 1);
	moved(speedd, 110);
	startd(0);
	movebc(-speed, 320, 1);
	pov(speed, d90 + 40, -1);
	startbc(speed);
	while (s2() > black);
	movebc(speed, wise + 10, 1);
	pov(speed, d90, 3);
	line(speed, 200, 4);
	d2 = gtf();
	movebc(-speed, 20, 1);
	pov(speed, d90, -1);
	movebc(speed, 10, 1);
	if (gclr(4) != 0) {
		gdeb = 2;
	}
	moveb(speed, pov1wheel, 1);
	startbc(speed);
	while (s3() > black);
	movebc(speed, wise, 1);
	if (d2.first == 4 || d2.second == 4) {
		pov(speed, d90, 0);
		line(speed, 300, 4);
		if (p1)
			give4();
		else
			give2();
		p2 = 1;
		pov(speed, d180, 2);
		line(speed, 600, 1);
	}
	else {
		pov(speed, d90, 3);
		line(speed, 200, 1);
	}
	go(speed, 12, 2, 15, 2, 0);
	d3 = gtf();
	gtb();
	if (d3.first == 4 || d3.second == 4) {
		pov(speed, d180, 2);
		line(speed, 200, 4);
		if (p1 || p2)
			give4();
		else
			give2();
		p3 = 1;
		pov(speed, d180, 2);
		line(speed, 250, 1);	
	}
	movebc(speed, wise, 1);
	pov(speed, d90, 0);
	line(speed, 100, 3);
	get_4_blue();
	return 0;
}
