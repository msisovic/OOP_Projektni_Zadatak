#include <iostream>
#include <string>
using namespace std;

#include "GradskaMreza.h"



int main() {
	GradskaMreza gradskaMreza;
	gradskaMreza.ucitajStajalista("stajalista.txt");
	gradskaMreza.ucitajLinije("linije.txt");
	gradskaMreza.eksportujStajaliste(422);
	gradskaMreza.eksportujLiniju("12");
	gradskaMreza.najmanjePresedanjaPutanja(123, 1221, 12, 0);
}