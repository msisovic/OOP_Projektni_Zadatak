#ifndef LINIJA_H
#define LINIJA_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

const int vremeDoSledeceStanice = 3;

class Linija {
public:
	Linija(const string& oznakaLinije, int prviPolazak, int poslednjiPolazak, int razmakPolazaka, const vector<int>& stajalista);

	friend ostream& operator<<(ostream& os, const Linija& linija);

	const string dohOznaku() const { return oznakaLinije; }
	const vector<int> dohStajalista() const { return stajalista; }
	const int brojPolazaka() const;

	const int sledeciDolazak(int vreme, int stajaliste, bool direktanSmer) const;
	const vector<int> staniceNaPutu(int prvaStanica, int poslednjaStanica) const;

private:
	string oznakaLinije;

	int prviPolazak;
	int poslednjiPolazak;
	int razmakPolazaka;

	vector<int> stajalista;

	map<int, int> mapaStajalista;
};

#endif