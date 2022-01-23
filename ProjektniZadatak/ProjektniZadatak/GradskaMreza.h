#ifndef GRADSKA_MREZA_H
#define GRADSKA_MREZA_H

#include <map>
using namespace std;

#include "Linija.h"
#include "Stajaliste.h"

class Grana {
public:
	Grana(int stanica, int linija, bool direktanSmer) :stanica(stanica), linija(linija), direktanSmer(direktanSmer) {};

	int dohLinija() { return linija; }
	int dohStanica() { return stanica; }
	int direktna() { return direktanSmer; }

private:
	int linija;
	int stanica;
	bool direktanSmer;
};

class GradskaMreza {
public:
	void test();

	GradskaMreza() { grafInicijalizovan = false; };

	void ucitajStajalista(const string& filepath);
	void ucitajLinije(const string& filepath);

	void eksportujStajaliste(const int sifraStajalista);
	void eksportujLiniju(const string& oznakaLinije);

private:
	vector<Stajaliste> stajalista;
	vector<Linija> linije;

	bool grafInicijalizovan;
	vector<vector<Grana>> graf;
	void inicijalizujGraf();

	map<int,int> mapaStajalista;
	map<string,int> mapaLinija;
	int idStajalista(const int sifraStajalista);
	int idLinije(const string& oznakaLinije);
};

#endif