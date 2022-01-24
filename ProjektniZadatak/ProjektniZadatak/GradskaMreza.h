#ifndef GRADSKA_MREZA_H
#define GRADSKA_MREZA_H

#include <map>
using namespace std;

#include "Graf.h"
#include "Linija.h"
#include "Stajaliste.h"

class GradskaMreza {
public:
	void test();

	GradskaMreza() { grafInicijalizovan = false; };

	void ucitajStajalista(const string& filepath);
	void ucitajLinije(const string& filepath);

	void eksportujStajaliste(int sifraStajalista);
	void eksportujLiniju(const string& oznakaLinije);

	void najbrzaPutanja(int pocetnoStajaliste, int krajnjeStajaliste, int satPolaska, int minutPolaska);
	void najmanjePresedanjaPutanja(int pocetnoStajaliste, int krajnjeStajaliste, int satPolaska, int minutPolaska);

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

	vector<Grana> nadjiPutanju(int pocetnoStajaliste, int krajnjeStajaliste, int satPolaska, int minutPolaska, Kriterijum kriterijum);
	void ispisiPutanju(int pocetnoStajaliste, int krajnjeStajaliste, vector<Grana> putanja);
	int nadjiVremeDolaska(int idStanice, const Grana& granaDoSledece, int vreme, int brojStanica);
};

#endif