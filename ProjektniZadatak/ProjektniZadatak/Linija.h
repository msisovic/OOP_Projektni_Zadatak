#ifndef LINIJA_H
#define LINIJA_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Linija {
public:
	Linija(const string& oznakaLinije, int prviPolazak, int poslednjiPolazak, int razmakPolazaka,const vector<int>& stajalista) : oznakaLinije(oznakaLinije), prviPolazak(prviPolazak), poslednjiPolazak(poslednjiPolazak), razmakPolazaka(razmakPolazaka), stajalista(stajalista) {};

	friend ostream& operator<<(ostream& os, const Linija& linija);

	const string dohOznaku() const { return oznakaLinije; }
	const vector<int> dohStajalista() const { return stajalista; }

private:
	string oznakaLinije;

	int prviPolazak;
	int poslednjiPolazak;
	int razmakPolazaka;

	vector<int> stajalista;
};

#endif