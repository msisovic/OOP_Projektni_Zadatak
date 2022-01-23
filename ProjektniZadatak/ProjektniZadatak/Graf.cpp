#include "Graf.h"

bool DijkstraFunkcijaKriterijuma::operator()(DijkstraCvor prvi, DijkstraCvor drugi) {
	switch (prvi.dohKriterijum()) {
	case NAJBRZE:
		return drugi.dohVreme() < prvi.dohVreme();
		break;
	case NAJMANJE_PRESEDANJA:
		return drugi.dohPresedanja() < prvi.dohPresedanja();
	}
}
