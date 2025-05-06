#pragma once
#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

//#include <atomic>
//#include <memory> //when hashmaps resize, they call the copy constructor, which is deleted for atomics
#include <mutex>
#include <shared_mutex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class StatisticsHandler {
public:
	static void DumpData(GLFWwindow* window) noexcept;

protected:
	static const std::string StatisticsDirectoryName; // = "stats";
	//file name generated from timestamp (TODO)

	static std::unordered_map<std::string, int64_t> data;
	//static std::unordered_map<std::string, std::shared_ptr<std::atomic<int64_t>>> data;

	static void createEntryIfNotPresent(std::string name);

public:
	static std::shared_mutex rw_lock; //not really used as a RW lock

	static void addData(std::string name, int64_t count);

};
