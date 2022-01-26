#ifndef IZUZECI_H
#define IZUZECI_H
#include <exception>
using namespace std;

class INepostojecaStanica :public exception {
public:
	INepostojecaStanica() :exception("Stanica ne postoji") {};
};

class INepostojecaLinija :public exception {
public:
	INepostojecaLinija() :exception("Linija ne postoji") {};
};

class INepostojecaDatotoeka :public exception {
public:
	INepostojecaDatotoeka() :exception("Nepostojeca datoteka") {};
};

class INepropisanFormatUlaza :public exception {
public:
	INepropisanFormatUlaza() :exception("Nepropisan format ulaza") {};
};

#endif
