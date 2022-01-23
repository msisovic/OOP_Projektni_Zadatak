#include "Linija.h"

ostream& operator<<(ostream& os, const Linija& linija) {
	return os << linija.oznakaLinije;
}

Linija::Linija(const string& oznakaLinije, int prviPolazak, int poslednjiPolazak, int razmakPolazaka, const vector<int>& stajalista) :oznakaLinije(oznakaLinije), prviPolazak(prviPolazak), poslednjiPolazak(poslednjiPolazak), razmakPolazaka(razmakPolazaka), stajalista(stajalista) {
	for (int i = 0; i < stajalista.size(); i++) {
		mapaStajalista[stajalista[i]] = i;
	}
}

const int Linija::sledeciDolazak(int vreme, int stajaliste, bool direktanSmer) const {
	int idStajalista = mapaStajalista.at(stajaliste);
	int rbrStajalista;
	if (direktanSmer) {
		rbrStajalista = idStajalista;
	}
	else {
		rbrStajalista = 2 * stajalista.size() - idStajalista - 2;
	}
	int vremePocetne = vreme - rbrStajalista * vremeDoSledeceStanice;
	int sledeciPolazak = (vremePocetne - prviPolazak + razmakPolazaka - 1) / razmakPolazaka * razmakPolazaka;
	if (sledeciPolazak <= poslednjiPolazak) {
		return sledeciPolazak + rbrStajalista * vremeDoSledeceStanice;
	}
	else {
		return -1;
	}
}
