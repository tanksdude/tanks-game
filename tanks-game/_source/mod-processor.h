#pragma once
#include <string>
#include <vector>
#include <optional>

class ModProcessor {
public:
	static void ProcessMods() noexcept; //adds the custom powers and levels

protected:
	static const std::string ModOrderPath; // = "mods/order.txt";
	static const std::string IgnoreListPath; // = "mods/ignore.txt";

	static std::vector<std::string> getListOfKnownMods(); //from order.txt
	static std::vector<std::string> getListOfUnknownMods(); //from <filesystem>
	static std::vector<std::string> getListOfIgnoredMods(); //from ignore.txt

public:
	static std::optional<std::vector<std::string>> getListOfMods() noexcept;

	static std::vector<std::string> getListOfFiles(std::string modPath); //use to get list of levels or powers
};
