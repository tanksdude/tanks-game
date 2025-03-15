#include "statistics-handler.h"

#include <filesystem> //for creating the directory, nothing more
#include <fstream>
#include <iostream>

const std::string StatisticsHandler::StatisticsDirectoryName = "stats";

std::unordered_map<std::string, int64_t> StatisticsHandler::data;

std::shared_mutex StatisticsHandler::rw_lock;

void StatisticsHandler::DumpData(GLFWwindow*) noexcept {
#if 0
	rw_lock.lock();

	try {
		if (!std::filesystem::exists(StatisticsDirectoryName)) {
			std::filesystem::create_directory(StatisticsDirectoryName);
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::string junk;
		std::cerr << "Error creating directory \"" + StatisticsDirectoryName + "\": " << e.what() << std::endl;
		std::cin >> junk;
		rw_lock.unlock();
		return;
	}

	const std::string stats_path = StatisticsDirectoryName + "/temp.txt"; //no easy way to generate a timestamp for a name; might do localtime() if there's nothing else

	std::ofstream stats_file;
	stats_file.open(stats_path);
	if (stats_file.is_open()) {
		for (auto& data_entry : StatisticsHandler::data) {
			stats_file << data_entry.first << " " << data_entry.second << std::endl;
		}
	} else {
		std::string junk;
		std::cerr << "Could not open/create file \"" + stats_path + "\"" << std::endl;
		std::cin >> junk;
		//do cin because this will happen when the program is closed
	}

	rw_lock.unlock();
#endif
}

void StatisticsHandler::createEntryIfNotPresent(std::string name) {
#if 0
	rw_lock.lock_shared();
	if (data.find(name) != data.end()) {
		//found
		rw_lock.unlock_shared();
		return;
	}
	rw_lock.unlock_shared();

	//not found, so need to create the entry, then add
	rw_lock.lock();
	if (data.find(name) != data.end()) {
		//need to search again in case the entry was added
		rw_lock.unlock();
		return;
	}

	//definitely not there, so add it finally
	data.insert({ name, 0 });
	rw_lock.unlock();
#endif
}

void StatisticsHandler::addData(std::string name, int64_t count) {
#if 0
	createEntryIfNotPresent(name);

	rw_lock.lock();
	data[name] += count;
	//data[name]->fetch_add(count); //std::atomic_fetch_add(data[name].get(), count);
	rw_lock.unlock();
#endif
}
