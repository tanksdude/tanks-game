#pragma once
#include <memory>
#include <vector>

struct GenericFactoryConstructionData {
public:
	static void deleteArrFunc(const void* p) { delete[] p; }

protected:
	std::vector<std::shared_ptr<const void>> data;
	std::vector<int> lengths;

public:
	void pushData(int length, const void* dataPortion); //arrays (made with new[])
	void pushData(int length, std::shared_ptr<const void> dataPortion);

	int getDataCount() const;
	int getDataPortionLength(int i) const;
	std::shared_ptr<const void> getDataPortion(int i) const;

	GenericFactoryConstructionData();
	GenericFactoryConstructionData(int length, const void* dataPortion); //start with one element (made with new[])
	GenericFactoryConstructionData(int length, std::shared_ptr<const void> dataPortion); //start with one element

	~GenericFactoryConstructionData();

private:
	GenericFactoryConstructionData(const GenericFactoryConstructionData&) = default;
};
