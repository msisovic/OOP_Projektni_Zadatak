#ifndef INTERAKTIVNI_SISTEM_H
#define INTERAKTIVNI_SISTEM_H

#include "GradskaMreza.h"

class InteraktivniSistem {
public:
	void start();

private:
	GradskaMreza gradskaMreza;
	string stajalistaFajl, linijeFajl;

	void unos_fajlova();
	void odabirFunkcijeGradskeMreze();
	void informacijeStajaliste();
	void informacijeLinija();
	void statistikaLinija();
	void pronalazakPutanje();
};

#endif