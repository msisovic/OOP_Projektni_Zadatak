#include "Izuzeci.h"
#include "Linija.h"

ostream& operator<<(ostream& os, const Linija& linija) {
	return os << linija.oznakaLinije;
}

//Pored inicijalizacije polja inicijalizuje mapiranje iz sifre stajalista u indeks u nizu
Linija::Linija(const string& oznakaLinije, int prviPolazak, int poslednjiPolazak, int razmakPolazaka, const vector<int>& stajalista) :oznakaLinije(oznakaLinije), prviPolazak(prviPolazak), poslednjiPolazak(poslednjiPolazak), razmakPolazaka(razmakPolazaka), stajalista(stajalista) {
	for (int i = 0; i < stajalista.size(); i++) {
		mapaStajalista[stajalista[i]] = i;
	}
}

const int Linija::brojPolazaka() const {
	return (poslednjiPolazak - prviPolazak) / razmakPolazaka + 1;
}

//Izracunava vreme sledeceg dolaska na datu stanicu u zavisnosti od trenutnog vremena i zeljenog smera kretanja, vraca -1 ako bus nece doci
const int Linija::sledeciDolazak(int vreme, int stajaliste, bool direktanSmer) const {
	int idStajalista;
	try {
		idStajalista = mapaStajalista.at(stajaliste);
	}
	catch (exception& e) {
		throw INepostojecaStanica();
	}

	int rbrStajalista;
	if (direktanSmer) {
		rbrStajalista = idStajalista;
	}
	else {
		rbrStajalista = 2 * stajalista.size() - idStajalista - 2;
	}
	int vremePocetne = max(vreme - rbrStajalista * vremeDoSledeceStanice, prviPolazak);//Vreme kada bus koji cekamo krece sa pocetne stanice
	int sledeciPolazak = (vremePocetne - prviPolazak + razmakPolazaka - 1) / razmakPolazaka * razmakPolazaka + prviPolazak;
	if (sledeciPolazak <= poslednjiPolazak) {
		return sledeciPolazak + rbrStajalista * vremeDoSledeceStanice;
	}
	else {
		return -1;
	}
}

//pronalazi i vraca niz stanica koje se redom nalaze na putu izmedju zadate dve stanice
const vector<int> Linija::staniceNaPutu(int prvaStanica, int poslednjaStanica) const {
	int prvaStanicaId, poslednjaStanicaId;
	try {
		prvaStanicaId = mapaStajalista.at(prvaStanica), poslednjaStanicaId = mapaStajalista.at(poslednjaStanica);
	}
	catch (exception& e) {
		throw INepostojecaStanica();
	}

	vector<int> put;
	if (prvaStanicaId <= poslednjaStanicaId) {
		for (int i = prvaStanicaId; i <= poslednjaStanicaId; i++) {
			put.push_back(stajalista[i]);
		}
	}
	else {
		for (int i = prvaStanicaId; i >= poslednjaStanicaId; i--) {
			put.push_back(stajalista[i]);
		}
	}
	return put;
}
