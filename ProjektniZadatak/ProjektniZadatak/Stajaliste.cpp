#include "Stajaliste.h"

ostream& operator<<(ostream& os, const Stajaliste& stajaliste) {
    return os << stajaliste.sifra << " " << stajaliste.naziv;
}
