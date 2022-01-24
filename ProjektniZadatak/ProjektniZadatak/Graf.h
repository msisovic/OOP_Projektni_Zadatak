#ifndef GRAF_H
#define GRAF_H

class Grana {
public:
	Grana(int stanica, int linija, bool direktanSmer, int brojStanica) :stanica(stanica), linija(linija), direktanSmer(direktanSmer), brojStanica(brojStanica) {};
	Grana() { linija = -1; };

	int dohLinija() const { return linija; }
	int dohStanica() const { return stanica; }
	int direktna() const { return direktanSmer; }
	int dohBrojStanica() const { return brojStanica; }

private:
	int brojStanica;
	int linija;
	int stanica;
	bool direktanSmer;
};

enum Kriterijum {
	NAJBRZE,
	NAJMANJE_PRESEDANJA
};

class DijkstraCvor {
public:
	DijkstraCvor(int stanica, int proslaStanica, Grana proslaGrana, int vreme, int presedanja, Kriterijum kriterijum) :stanica(stanica), proslaStanica(proslaStanica), proslaGrana(proslaGrana), vreme(vreme), presedanja(presedanja), kriterijum(kriterijum) {};
	DijkstraCvor() = default;

	int dohStanicu() { return stanica; }
	int dohProsluStanicu() { return proslaStanica; }
	Grana dohProsluGranu() { return proslaGrana; } 
	int dohVreme() { return vreme; }
	int dohPresedanja() { return presedanja; }
	Kriterijum dohKriterijum() { return kriterijum; }

private:
	int stanica;
	int proslaStanica;
	Grana proslaGrana;
	int vreme;
	int presedanja;
	Kriterijum kriterijum;
};

class DijkstraFunkcijaKriterijuma {
public:
	bool operator()(DijkstraCvor prvi, DijkstraCvor drugi);
};

#endif