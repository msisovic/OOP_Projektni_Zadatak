#include "GradskaMreza.h"

#include <fstream>
#include <algorithm>
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

void GradskaMreza::eksportujStajaliste(const int sifraStajalista) {
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
			if (j != 0) {
				int proslo = stajalistaLinije[j - 1], trenutno = stajalistaLinije[j];
				int idProslog = idStajalista(proslo), idTrenutnog = idStajalista(trenutno);
				graf[idProslog].push_back(Grana(idTrenutnog, idTrenLinije, true));
				graf[idTrenutnog].push_back(Grana(idProslog, idTrenLinije, false));
			}
		}
	}

	grafInicijalizovan = true;
}

int GradskaMreza::idLinije(const string& oznakaLinije) {
	return mapaLinija[oznakaLinije];
}
