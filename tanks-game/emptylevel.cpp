#include "emptylevel.h"

void EmptyLevel::initialize() {
	return;
}

Level* EmptyLevel::factory() {
	return new EmptyLevel();
}

EmptyLevel::EmptyLevel() { return; }
