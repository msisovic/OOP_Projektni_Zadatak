#ifndef STAJALISTE_H
#define STAJALISTE_H

#include <string>
#include <iostream>
using namespace std;

class Stajaliste {
public:
	Stajaliste(int sifra, const string& naziv) :sifra(sifra), naziv(naziv) {};

	friend ostream& operator<<(ostream& os, const Stajaliste& stajaliste);

	const int dohSifru() const { return sifra; }
	const string dohNaziv() const { return naziv; }

private:
	int sifra;
	string naziv;
};

#endif
