#include "InteraktivniSistem.h"

void InteraktivniSistem::start() {
	bool greska = false;
	while (true) {
		if (!greska) {
			cout << "\n";
			cout << "Dobrodosli u simulator mreze gradskog prevoza. Molimo Vas, odaberite opciju:\n";
			cout << "1. Ucitavanje podataka o mrezi gradskog prevoza\n";
			cout << "0. Kraj rada\n";
		}
		cout << "\n";
		greska = false;
		int opcija;
		cin >> opcija;
		cin.get();
		switch (opcija) {
		case 0:
			return;
			break;
		case 1:
			unos_fajlova();
			odabirFunkcijeGradskeMreze();
			break;
		default:
			cout << "\n";
			cout << "Izabrali ste nepostojecu opciju, molimo Vas pokusajte ponovo:\n";
			cout << "\n";
			greska = true;
		}
	}
}

void InteraktivniSistem::unos_fajlova() {
	cout << "\n";
	cout << "Molimo Vas, unesite putanju do fajla sa stajalistima:\n";
	cout << "\n";
	getline(cin, stajalistaFajl);

	cout << "\n";
	cout << "Molimo Vas, unesite putanju do fajla sa linijama gradskog prevoza:\n";
	cout << "\n";
	getline(cin, linijeFajl);
}

void InteraktivniSistem::odabirFunkcijeGradskeMreze() {
	gradskaMreza.ucitajStajalista(stajalistaFajl);
	gradskaMreza.ucitajLinije(linijeFajl);

	bool greska = false;
	while (true) {
		if (!greska) {
			cout << "\n";
			cout << "Mreza gradskog prevoza je uspesno ucitana. Molimo Vas, odaberite opciju:\n";
			cout << "1. Prikaz informacija o stajalistu\n";
			cout << "2. Prikaz osnovnih informacija o liniji gradskog prevoza\n";
			cout << "3. Prikaz statistickih informacija o liniji gradskog prevoza\n";
			cout << "4. Pronalazak putanje izmedju dva stajalista\n";
			cout << "0. Kraj rada\n";
		}
		greska = false;
		cout << "\n";
		
		int opcija;
		cin >> opcija;
		switch (opcija) {
		case 0:
			return;
		case 1:
			informacijeStajaliste();
			break;
		case 2:
			informacijeLinija();
			break;
		case 3:
			statistikaLinija();
			break;
		case 4:
			pronalazakPutanje();
			break;
		default:
			greska = true;
			cout << "\n";
			cout << "Izabrali ste nepostojecu opciju, molimo Vas pokusajte ponovo:\n";
			cout << "\n";
		}
	}
}

void InteraktivniSistem::informacijeStajaliste() {
	cout << "\n";
	cout << "Molimo Vas, unesite stajaliste cije informacije zelite da prikazete:\n";
	cout << "\n";

	int stajaliste;
	cin >> stajaliste;
	cin.get();

	gradskaMreza.eksportujStajaliste(stajaliste);

	cout << "\n";
	cout << "Generisan je fajl stajaliste_" << stajaliste << ".txt sa informacijama o stajalistu " << stajaliste << ".\n";
	cout << "\n";
}

void InteraktivniSistem::informacijeLinija() {
	cout << "\n";
	cout << "Molimo Vas, unesite liniju cije informacije zelite da prikazete:\n";
	cout << "\n";

	string linija;
	cin >> linija;
	cin.get();

	gradskaMreza.eksportujLiniju(linija);

	cout << "\n";
	cout << "Generisan je fajl linija_" << linija << ".txt sa informacijama o liniji " << linija << ".\n";
	cout << "\n";
}

void InteraktivniSistem::statistikaLinija() {
	cout << "\n";
	cout << "Molimo Vas, unesite liniju ciju statistiku zelite da prikazete:\n";
	cout << "\n";

	string linija;
	cin >> linija;
	cin.get();

	gradskaMreza.eksportujStatistiku(linija);

	cout << "\n";
	cout << "Generisan je fajl statistika_" << linija << ".txt sa statistikom linije " << linija << ".\n";
	cout << "\n";
}

void InteraktivniSistem::pronalazakPutanje() {
	cout << "\n";
	cout << "Molimo Vas, unesite polaznu stanicu:\n";
	cout << "\n";

	int polaznaStanica;
	cin >> polaznaStanica;
	cin.get();

	cout << "\n";
	cout << "Molimo Vas, unesite stanicu na koju zelite da stignete:\n";
	cout << "\n";

	int krajnjaStanica;
	cin >> krajnjaStanica;
	cin.get();

	cout << "\n";
	cout << "Molimo Vas, unesite polazno vreme u formatu hh:mm\n";
	cout << "\n";

	int sati, minuti;
	cin >> sati;
	cin.get();
	cin >> minuti;
	cin.get();

	cout << "\n";
	cout << "Molimo Vas, odaberite tip putanje:\n";
	cout << "1. Najbrza putanja\n";
	cout << "2. Putanja sa najmanje presedanja\n";
	cout << "\n";

	bool unesenaOpcija = false;
	while (!unesenaOpcija) {
		int opcija;
		cin >> opcija;
		cin.get();
		switch (opcija) {
		case 1:
			unesenaOpcija = true;
			gradskaMreza.najbrzaPutanja(polaznaStanica, krajnjaStanica, sati, minuti);
			break;
		case 2:
			unesenaOpcija = true;
			gradskaMreza.najmanjePresedanjaPutanja(polaznaStanica, krajnjaStanica, sati, minuti);
			break;
		default:
			cout << "\n";
			cout << "Izabrali ste nepostojecu opciju, molimo Vas pokusajte ponovo:\n";
			cout << "\n";
		}
	}

	cout << "\n";
	cout << "Generisan je fajl putanja_" << polaznaStanica<<"_"<<krajnjaStanica << ".txt sa zeljenom putanjom.\n";
	cout << "\n";
}
