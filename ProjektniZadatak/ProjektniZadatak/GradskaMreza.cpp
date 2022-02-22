#include "GradskaMreza.h"
#include "Izuzeci.h"

#include <algorithm>
#include <fstream>
#include <queue>
#include <set>
using namespace std;

void GradskaMreza::ucitajStajalista(const string& filepath) {
	stajalista.clear();
	grafInicijalizovan = false;

	ifstream ulaz(filepath);
	if (ulaz.fail()) {
		throw INepostojecaDatotoeka();
	}

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
	if (ulaz.fail()) {
		throw INepostojecaDatotoeka();
	}

	string oznakaLinije;
	while (ulaz>>oznakaLinije) {
		char d1;
		ulaz >> d1;
		if (d1 != '[') {
			throw INepropisanFormatUlaza();
		}

		int prviSat;
		ulaz >> prviSat;
		ulaz >> d1;
		if (d1 != ':') {
			throw INepropisanFormatUlaza();
		}
		int prviMinut;
		ulaz >> prviMinut;
		int prviPolazak = 60 * prviSat + prviMinut;

		ulaz >> d1;
		if (d1 != '-') {
			throw INepropisanFormatUlaza();
		}

		int poslednjiSat;
		ulaz >> poslednjiSat;
		ulaz >> d1;
		if (d1 != ':') {
			throw INepropisanFormatUlaza();
		}
		int poslednjiMinut;
		ulaz >> poslednjiMinut;
		int poslednjiPolazak = 60 * poslednjiSat + poslednjiMinut;

		ulaz >> d1;
		if (d1 != '#') {
			throw INepropisanFormatUlaza();
		}

		int razmakPolazaka;
		ulaz >> razmakPolazaka;

		ulaz >> d1;
		if (d1 != ']') {
			throw INepropisanFormatUlaza();
		}

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

	//nalazimo sve linije koje prolaze kroz stajaliste tako sto vidimo pomocu kojih linija mozemo stici do susednih stanica, sto imamo u grafu
	vector<string> naziviLinija;
	for (Grana& grana : graf[id]) {
		if (grana.direktna() && grana.dohBrojStanica() == 1) {
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

void GradskaMreza::eksportujStatistiku(const string& oznakaLinije) {
	const int id = idLinije(oznakaLinije);
	const Linija& linija = linije[id];
	const string filepath = "statistika_" + oznakaLinije + ".txt";
	ofstream izlaz(filepath);

	izlaz << oznakaLinije << "\n";

	for (int i = 0; i < linije.size(); i++) {
		if (id == i) {
			continue;
		}
		if (linijeSeSeku(id, i)) {
			izlaz << linije[i].dohOznaku() << " ";
		}
	}
	izlaz << "\n";

	izlaz << linija.brojPolazaka();

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

//vraca indeks stajalista u nizu stajalista na osnovu njegove sifre
int GradskaMreza::idStajalista(int sifraStajalista) {
	if (mapaStajalista.find(sifraStajalista) == mapaStajalista.end()) {
		throw INepostojecaStanica();
	}
	return mapaStajalista[sifraStajalista];
}

void GradskaMreza::inicijalizujGraf() {
	if (grafInicijalizovan) {
		return;
	}

	//inicijalizuje mapiranje is sifre/oznake u indeks u nizu za stajalista/linije
	mapaLinija.clear();
	mapaStajalista.clear();
	for (int i = 0; i < stajalista.size(); i++) {
		mapaStajalista[stajalista[i].dohSifru()] = i;
	}
	for (int i = 0; i < linije.size(); i++) {
		mapaLinija[linije[i].dohOznaku()] = i;
	}

	//Za svake dve stanice kroz koje bilo koja linija prolazi dodajemo grane u grafu koje ih povezuju u uba smera
	graf.clear();
	graf.resize(stajalista.size());
	for (int i = 0; i < linije.size(); i++) {
		const vector<int> stajalistaLinije = linije[i].dohStajalista();
		int idTrenLinije = idLinije(linije[i].dohOznaku());
		for (int j = 0; j < stajalistaLinije.size(); j++) {
			for (int k = j + 1; k < stajalistaLinije.size(); k++) {
				int stajaliste1Sifra = stajalistaLinije[j], stajaliste2Sifra = stajalistaLinije[k];
				int stajaliste1Id = idStajalista(stajaliste1Sifra), stajaliste2Id = idStajalista(stajaliste2Sifra);
				int brojStanica = k - j;
				graf[stajaliste1Id].push_back(Grana(stajaliste2Id, idTrenLinije, true, brojStanica));
				graf[stajaliste2Id].push_back(Grana(stajaliste1Id, idTrenLinije, false, brojStanica));
			}
		}
	}

	grafInicijalizovan = true;
}

bool GradskaMreza::linijeSeSeku(int idLinije1, int idLinije2) {
	set<int> stajalistaPrve;
	for (int stajaliste : linije[idLinije1].dohStajalista()) {
		stajalistaPrve.insert(stajaliste);
	}

	for (int stajaliste : linije[idLinije2].dohStajalista()) {
		if (stajalistaPrve.find(stajaliste) != stajalistaPrve.end()) {
			return true;
		}
	}
	
	return false;
}

//vraca indeks u nizu linija na osnovu njene oznake
int GradskaMreza::idLinije(const string& oznakaLinije) {
	if (mapaLinija.find(oznakaLinije) == mapaLinija.end()) {
		throw INepostojecaLinija();
	}
	return mapaLinija[oznakaLinije];
}

//Pronalazi put izmedju dva stajalista a osnovu kriterijuma i vremena polaska, ako put ne postoji vraca prazan vektor
//Koristi dijkstrin algoritam
vector<Grana> GradskaMreza::nadjiPutanju(int pocetnoStajaliste, int krajnjeStajaliste, int satPolaska, int minutPolaska, Kriterijum kriterijum) {
	inicijalizujGraf();
	int krajnjeStajalisteId = idStajalista(krajnjeStajaliste);
	priority_queue<DijkstraCvor, vector<DijkstraCvor>, DijkstraFunkcijaKriterijuma> dijkstra;
	int polaznoVreme = 60 * satPolaska + minutPolaska;
	dijkstra.push(DijkstraCvor(idStajalista(pocetnoStajaliste), -1, Grana(), polaznoVreme, 0, kriterijum));

	vector<DijkstraCvor> cvorovi(stajalista.size());//Ovde cemo cuvati u kom smo stanju dosli do svakog stajalista
	vector<bool> posecen(stajalista.size());//Cuvamo da li smo obisli stajaliste

	while (!dijkstra.empty()) {
		DijkstraCvor trenutniCvor = dijkstra.top();
		dijkstra.pop();
		int trenutnoStajalisteId = trenutniCvor.dohStanicuId();

		if (posecen[trenutnoStajalisteId]) {
			continue;
		}
		posecen[trenutnoStajalisteId] = true;
		cvorovi[trenutnoStajalisteId] = trenutniCvor;//Pamtimo stanje kako bismo mogli da rekonstruisemo putanju

		//Proveravamo da li smo stigli do cilja i ako jesmo rekonstruisemo putanju
		if (trenutnoStajalisteId == krajnjeStajalisteId) {
			vector<Grana> put;
			while (trenutniCvor.dohProsluStanicuId() != -1) {
				put.push_back(trenutniCvor.dohProsluGranuId());
				trenutniCvor = cvorovi[trenutniCvor.dohProsluStanicuId()];
			}
			reverse(put.begin(), put.end());
			return put;
		}

		int vreme = trenutniCvor.dohVreme();
		Grana proslaGrana = trenutniCvor.dohProsluGranuId();

		for (Grana& grana : graf[trenutnoStajalisteId]) {
			int presedanja = trenutniCvor.dohPresedanja() + 1;//Znamo da cemo uvek da presedamo jer to osiguravamo u daljem ifu
			int proslaStanica = trenutniCvor.dohProsluStanicuId();
			int udaljenostCiljneStanice = grana.dohBrojStanica();
			//Obavezujemo se na presedanje jer imamo direktne grane u grafu i izmedju nesusednih stajalista
			if (proslaStanica != -1 && proslaGrana.dohLinija() == grana.dohLinija()) {
				continue;
			}
			int vremeDolaska = nadjiVremeDolaska(trenutnoStajalisteId, grana, vreme, udaljenostCiljneStanice);
			//Proveravamo da li uopste mozemo da dodjemo
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
	for (Grana& grana : putanja) {
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

//Nalazi vreme dolaska na ciljnu stanicu, udaljenu od trenutne stanice za udaljenostCiljneStanice, ako znamo sa koje stanice krecemo,
//u koje vreme i kojom linijom zelimo da idemo
int GradskaMreza::nadjiVremeDolaska(int idTrenStanice,const Grana& granaDoSledece, int vreme, int udaljenostCiljneStanice) {
	int sifraStajalista = stajalista[idTrenStanice].dohSifru();
	bool direktanSmer = granaDoSledece.direktna();
	Linija& linija = linije[granaDoSledece.dohLinija()];
	int sledeciDolazak = linija.sledeciDolazak(vreme, sifraStajalista, direktanSmer);
	if (sledeciDolazak == -1) {
		return -1;
	}
	return sledeciDolazak + udaljenostCiljneStanice * vremeDoSledeceStanice;
}
