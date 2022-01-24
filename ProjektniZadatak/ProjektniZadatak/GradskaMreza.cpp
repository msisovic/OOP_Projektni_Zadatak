#include "GradskaMreza.h"

#include <algorithm>
#include <fstream>
#include <queue>
using namespace std;

void GradskaMreza::test() {
	for (Stajaliste s : stajalista) {
		cout << s << "\n";
	}
	cout << "\n";
	for (Linija l : linije) {
		cout << l << "\n";
	}
}

void GradskaMreza::ucitajStajalista(const string& filepath) {
	stajalista.clear();
	grafInicijalizovan = false;

	ifstream ulaz(filepath);
	int sifraStajalista;
	string nazivStajalista;
	while (ulaz >> sifraStajalista) {
		ulaz.get();
		getline(ulaz, nazivStajalista);
		stajalista.push_back(Stajaliste(sifraStajalista, nazivStajalista));
	}
	ulaz.close();
}

void GradskaMreza::ucitajLinije(const string& filepath) {
	linije.clear();
	grafInicijalizovan = false;

	ifstream ulaz(filepath);
	string oznakaLinije;
	while (ulaz>>oznakaLinije) {
		char d1, d2;
		ulaz >> d1 >> d2;

		int prviSat;
		ulaz >> prviSat;
		ulaz >> d1;
		int prviMinut;
		ulaz >> prviMinut;
		int prviPolazak = 60 * prviSat + prviMinut;

		ulaz >> d1;

		int poslednjiSat;
		ulaz >> poslednjiSat;
		ulaz >> d1;
		int poslednjiMinut;
		ulaz >> poslednjiMinut;
		int poslednjiPolazak = 60 * poslednjiSat + poslednjiMinut;

		ulaz >> d1;

		int razmakPolazaka;
		ulaz >> razmakPolazaka;

		ulaz >> d1;

		vector<int> stajalista;
		int sifraStajalista;
		while (ulaz.get(d1)) {
			if (d1 == '\n') {
				break;
			}
			ulaz >> sifraStajalista;
			stajalista.push_back(sifraStajalista);
		}

		linije.push_back(Linija(oznakaLinije, prviPolazak, poslednjiPolazak, razmakPolazaka, stajalista));
	}
	ulaz.close();
}

void GradskaMreza::eksportujStajaliste(int sifraStajalista) {
	inicijalizujGraf();
	const int id = idStajalista(sifraStajalista);
	const string filepath = "stajaliste_" + to_string(sifraStajalista) + ".txt";
	ofstream izlaz(filepath);

	izlaz << stajalista[id];

	vector<string> naziviLinija;
	for (Grana grana : graf[id]) {
		if (grana.direktna()) {
			const int idLinije = grana.dohLinija();
			const string nazivLinije = linije[idLinije].dohOznaku();
			naziviLinija.push_back(nazivLinije);
		}
	}
	sort(naziviLinija.begin(), naziviLinija.end());

	izlaz << " [";
	for (int i = 0; i < naziviLinija.size(); i++) {
		izlaz << naziviLinija[i];
		if (i != naziviLinija.size() - 1) {
			izlaz << " ";
		}
	}
	izlaz << "]";

	izlaz.close();
}

void GradskaMreza::eksportujLiniju(const string& oznakaLinije) {
	inicijalizujGraf();
	const int id = idLinije(oznakaLinije);
	const string filepath = "linija_" + oznakaLinije + ".txt";
	ofstream izlaz(filepath);

	izlaz << oznakaLinije << " ";

	vector<int> stajalistaLinije = linije[id].dohStajalista();
	int idPrvog = idStajalista(stajalistaLinije[0]), idPoslednjeg = idStajalista(stajalistaLinije.back());
	izlaz << stajalista[idPrvog].dohNaziv() << "->" << stajalista[idPoslednjeg].dohNaziv();
	for (int stajaliste : stajalistaLinije) {
		int idTren = idStajalista(stajaliste);
		izlaz << "\n" << stajalista[idTren];
	}

	izlaz.close();
}

void GradskaMreza::najbrzaPutanja(int pocetnoStajaliste, int krajnjeStajaliste, int satPolaska, int minutPolaska) {
	vector<Grana> put = nadjiPutanju(pocetnoStajaliste, krajnjeStajaliste, satPolaska, minutPolaska, NAJBRZE);
	ispisiPutanju(pocetnoStajaliste, krajnjeStajaliste, put);
}

void GradskaMreza::najmanjePresedanjaPutanja(int pocetnoStajaliste, int krajnjeStajaliste, int satPolaska, int minutPolaska) {
	vector<Grana> put = nadjiPutanju(pocetnoStajaliste, krajnjeStajaliste, satPolaska, minutPolaska, NAJMANJE_PRESEDANJA);
	ispisiPutanju(pocetnoStajaliste, krajnjeStajaliste, put);
}

int GradskaMreza::idStajalista(int sifraStajalista) {
	return mapaStajalista[sifraStajalista];
}

void GradskaMreza::inicijalizujGraf() {
	if (grafInicijalizovan) {
		return;
	}

	mapaLinija.clear();
	mapaStajalista.clear();
	for (int i = 0; i < stajalista.size(); i++) {
		mapaStajalista[stajalista[i].dohSifru()] = i;
	}
	for (int i = 0; i < linije.size(); i++) {
		mapaLinija[linije[i].dohOznaku()] = i;
	}

	graf.clear();
	graf.resize(stajalista.size());
	for (int i = 0; i < linije.size(); i++) {
		const vector<int> stajalistaLinije = linije[i].dohStajalista();
		int idTrenLinije = idLinije(linije[i].dohOznaku());
		for (int j = 0; j < stajalistaLinije.size(); j++) {
			for (int k = j + 1; k < stajalistaLinije.size(); k++) {
				int stajaliste1 = stajalistaLinije[j], stajaliste2 = stajalistaLinije[k];
				int stajaliste1Id = idStajalista(stajaliste1), stajaliste2Id = idStajalista(stajaliste2);
				int brojStanica = k - j;
				graf[stajaliste1Id].push_back(Grana(stajaliste2Id, idTrenLinije, true, brojStanica));
				graf[stajaliste2Id].push_back(Grana(stajaliste1Id, idTrenLinije, false, brojStanica));
			}
		}
	}

	grafInicijalizovan = true;
}

int GradskaMreza::idLinije(const string& oznakaLinije) {
	return mapaLinija[oznakaLinije];
}

vector<Grana> GradskaMreza::nadjiPutanju(int pocetnoStajaliste, int krajnjeStajaliste, int satPolaska, int minutPolaska, Kriterijum kriterijum) {
	inicijalizujGraf();
	int krajnjeStajalisteId = idStajalista(krajnjeStajaliste);
	priority_queue<DijkstraCvor, vector<DijkstraCvor>, DijkstraFunkcijaKriterijuma> dijkstra;
	int polaznoVreme = 60 * satPolaska + minutPolaska;
	dijkstra.push(DijkstraCvor(idStajalista(pocetnoStajaliste), -1, Grana(), polaznoVreme, 0, kriterijum));
	vector<DijkstraCvor> cvorovi(stajalista.size());
	vector<bool> posecen(stajalista.size());
	while (!dijkstra.empty()) {
		DijkstraCvor trenutniCvor = dijkstra.top();
		dijkstra.pop();
		int trenutnoStajalisteId = trenutniCvor.dohStanicu();

		if (posecen[trenutnoStajalisteId]) {
			continue;
		}
		posecen[trenutnoStajalisteId] = true;
		cvorovi[trenutnoStajalisteId] = trenutniCvor;
		//cout << stajalista[trenutnoStajalisteId].dohSifru() << " " << trenutniCvor.dohVreme() - polaznoVreme<<"\n";

		if (trenutnoStajalisteId == krajnjeStajalisteId) {
			vector<Grana> put;
			while (trenutniCvor.dohProsluStanicu() != -1) {
				put.push_back(trenutniCvor.dohProsluGranu());
				trenutniCvor = cvorovi[trenutniCvor.dohProsluStanicu()];
			}
			reverse(put.begin(), put.end());
			return put;
		}

		int vreme = trenutniCvor.dohVreme();
		Grana proslaGrana = trenutniCvor.dohProsluGranu();

		for (Grana grana : graf[trenutnoStajalisteId]) {
			int presedanja = trenutniCvor.dohPresedanja() + 1;
			int proslaStanica = trenutniCvor.dohProsluStanicu();
			int brojStanica = grana.dohBrojStanica();
			if (proslaStanica != -1 && proslaGrana.dohLinija() == grana.dohLinija()) {
				continue;
			}
			int vremeDolaska = nadjiVremeDolaska(trenutnoStajalisteId, grana, vreme, brojStanica);
			if (vremeDolaska == -1) {
				continue;
			}
			dijkstra.push(DijkstraCvor(grana.dohStanica(), trenutnoStajalisteId, grana, vremeDolaska, presedanja, kriterijum));
		}
	}
	return vector<Grana>();
}

void GradskaMreza::ispisiPutanju(int pocetnoStajaliste, int krajnjeStajaliste, vector<Grana> putanja) {
	string filepath = "putanja_" + to_string(pocetnoStajaliste) + "_" + to_string(krajnjeStajaliste) + ".txt";
	ofstream izlaz(filepath);

	if (putanja.empty()) {
		izlaz << "Ne moze se doci do zadate stanice!";
		return;
	}

	int trenStanica = pocetnoStajaliste;
	string proslaLinija = "";
	for (Grana grana : putanja) {
		Linija& linija = linije[grana.dohLinija()];
		izlaz << proslaLinija << "->" << linija.dohOznaku() << "\n";
		int sledecaStanica = stajalista[grana.dohStanica()].dohSifru();
		vector<int> staniceNaPutu = linija.staniceNaPutu(trenStanica, sledecaStanica);
		for (int stanica : staniceNaPutu) {
			izlaz << stanica << " ";
		}
		izlaz << "\n";
		proslaLinija = linija.dohOznaku();
		trenStanica = sledecaStanica;
	}
}

int GradskaMreza::nadjiVremeDolaska(int idStanice,const Grana& granaDoSledece, int vreme, int brojStanica) {
	int sifraStajalista = stajalista[idStanice].dohSifru();
	bool direktanSmer = granaDoSledece.direktna();
	Linija& linija = linije[granaDoSledece.dohLinija()];
	int sledeciDolazak = linija.sledeciDolazak(vreme, sifraStajalista, direktanSmer);
	if (sledeciDolazak == -1) {
		return -1;
	}
	return sledeciDolazak + brojStanica * vremeDoSledeceStanice;
}
