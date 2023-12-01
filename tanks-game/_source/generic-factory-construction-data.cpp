#include "generic-factory-construction-data.h"

GenericFactoryConstructionData::GenericFactoryConstructionData() {
	//nothing
}

GenericFactoryConstructionData::GenericFactoryConstructionData(int length, const void* dataPortion)
: GenericFactoryConstructionData() {
	pushData(length, std::shared_ptr<const void>(dataPortion, deleteArrFunc));
}

GenericFactoryConstructionData::GenericFactoryConstructionData(int length, std::shared_ptr<const void> dataPortion)
: GenericFactoryConstructionData() {
	pushData(length, dataPortion);
}

GenericFactoryConstructionData::~GenericFactoryConstructionData() {
	//nothing
}

void GenericFactoryConstructionData::pushData(int length, const void* dataPortion) {
	lengths.push_back(length);
	data.push_back(std::shared_ptr<const void>(dataPortion, deleteArrFunc));
}

void GenericFactoryConstructionData::pushData(int length, std::shared_ptr<const void> dataPortion) {
	lengths.push_back(length);
	data.push_back(dataPortion);
}

int GenericFactoryConstructionData::getDataCount() const {
	//return data.size();
	return lengths.size();
}

int GenericFactoryConstructionData::getDataPortionLength(int i) const {
	return lengths[i];
}

std::shared_ptr<const void> GenericFactoryConstructionData::getDataPortion(int i) const {
	return data[i];
}
