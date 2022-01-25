#pragma once
#include <vector>

struct GenericFactoryConstructionData {
protected:
	std::vector<void*> data;
	std::vector<int> lengths;

public:
	void pushData(int length, void* dataPortion);

	int getDataCount() const;
	int getDataPortionLength(int i);
	void* getDataPortion(int i);

	GenericFactoryConstructionData();
	GenericFactoryConstructionData(int length, void* dataPortion);
	~GenericFactoryConstructionData();

private:
	GenericFactoryConstructionData(const GenericFactoryConstructionData&) {} //not super important to disable
};
