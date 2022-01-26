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
	int brojStanica;//broj stanica izmedju stanica koje grana povezuje
	int linija;
	int stanica;//stanica u koju ulazi grana
	bool direktanSmer;//da li je smer kretanja autobusa direktan
};

enum Kriterijum {
	NAJBRZE,
	NAJMANJE_PRESEDANJA
};

class DijkstraCvor {
public:
	DijkstraCvor(int stanicaId, int proslaStanicaId, Grana proslaGranaId, int vreme, int presedanja, Kriterijum kriterijum) :stanicaId(stanicaId), proslaStanicaId(proslaStanicaId), proslaGranaId(proslaGranaId), vreme(vreme), presedanja(presedanja), kriterijum(kriterijum) {};
	DijkstraCvor() = default;

	int dohStanicuId() const { return stanicaId; }
	int dohProsluStanicuId() const { return proslaStanicaId; }
	Grana dohProsluGranuId() const { return proslaGranaId; } 
	int dohVreme() const { return vreme; }
	int dohPresedanja() const { return presedanja; }
	Kriterijum dohKriterijum() const { return kriterijum; }

private:
	int stanicaId;
	int proslaStanicaId;
	Grana proslaGranaId;
	int vreme;
	int presedanja;
	Kriterijum kriterijum;
};

//Ova klasa postoji zbog poredjenja unutar priority_queue-a
class DijkstraFunkcijaKriterijuma {
public:
	bool operator()(const DijkstraCvor& prvi, const DijkstraCvor& drugi);
};

#endif