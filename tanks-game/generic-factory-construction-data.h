#pragma once
#include <vector>

struct GenericFactoryConstructionData {
protected:
	std::vector<const void*> data;
	std::vector<int> lengths;

public:
	void pushData(int length, const void* dataPortion);

	int getDataCount() const;
	int getDataPortionLength(int i) const;
	const void* getDataPortion(int i) const;

	GenericFactoryConstructionData();
	GenericFactoryConstructionData(int length, const void* dataPortion); //start with one element

	~GenericFactoryConstructionData();

private:
	GenericFactoryConstructionData(const GenericFactoryConstructionData&) {} //not super important to disable
};
