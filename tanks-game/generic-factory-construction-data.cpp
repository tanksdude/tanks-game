#include "generic-factory-construction-data.h"

GenericFactoryConstructionData::GenericFactoryConstructionData() {
	//nothing
}

GenericFactoryConstructionData::GenericFactoryConstructionData(int length, void* dataPortion)
: GenericFactoryConstructionData() {
	pushData(length, dataPortion);
}

GenericFactoryConstructionData::~GenericFactoryConstructionData() {
	//TODO: is this supposed to free memory?
	/*
	for (int i = 0; i < data.size(); i++) {
		delete[] data[i];
	}
	*/
	//data.clear();
}

void GenericFactoryConstructionData::pushData(int length, void* dataPortion) {
	lengths.push_back(length);
	data.push_back(dataPortion);
}

int GenericFactoryConstructionData::getDataCount() const {
	//return data.size();
	return lengths.size();
}

int GenericFactoryConstructionData::getDataPortionLength(int i) {
	return lengths[i];
}

void* GenericFactoryConstructionData::getDataPortion(int i) {
	return data[i];
}
