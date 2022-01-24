#include "Graf.h"

bool DijkstraFunkcijaKriterijuma::operator()(DijkstraCvor prvi, DijkstraCvor drugi) {
	switch (prvi.dohKriterijum()) {
	case NAJBRZE:
		if (drugi.dohVreme() == prvi.dohVreme()) {
			return drugi.dohPresedanja() < prvi.dohPresedanja();
		}
		return drugi.dohVreme() < prvi.dohVreme();
		break;
	case NAJMANJE_PRESEDANJA:
		if (drugi.dohPresedanja() == prvi.dohPresedanja()) {
			return drugi.dohVreme() < prvi.dohVreme();
		}
		return drugi.dohPresedanja() < prvi.dohPresedanja();
		break;
	}
}
