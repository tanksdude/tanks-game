#include "custom-power-interpreter.h"

#include <stdexcept>
#include <algorithm> //std::find
#include <filesystem> //only for catching ModProcessor's exceptions
#include <fstream>
#include <iostream>
#include <vector>

#include "mod-processor.h"
#include "powerup-data-governor.h" //for adding custom powers to the game
#include "basic-ini-parser.h" //for processing custom power files

#include "power-function-helper.h"

//const std::string CustomLevelInterpreter::ActionStartPhrase = "[LEVEL_START]";



void CustomPowerInterpreter::ProcessCustomPowers() noexcept {
	//TODO

	/*
	//get list of mods, interpret them, then add factories to LevelDataGovernor
	//should syntax version be a thing? likely features will only be added, but who knows

	auto modOrder_temp = ModProcessor::getListOfMods();
	if (!modOrder_temp.has_value()) {
		return;
	}
	std::vector<std::string>& modOrder_list = modOrder_temp.value();

	for (int i = 0; i < modOrder_list.size(); i++) {
		std::vector<std::string> levelOrder_list;
		try {
			levelOrder_list = ModProcessor::getListOfFiles("mods/" + modOrder_list[i] + "/levels");
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << "ModProcessor::getListOfFiles(" + ("mods/" + modOrder_list[i] + "/levels") + ") filesystem_error: " << e.what() << std::endl;
			continue; //hope things work out on the next mod
		}

		for (int j = 0; j < levelOrder_list.size(); j++) {
			//std::cout << "modOrder_list[" << i << "]: " << "levelOrder_list[" << j << "]: mods/" + modOrder_list[i] + "/levels/" << levelOrder_list[j] << std::endl;
			std::string levelPath = "mods/" + modOrder_list[i] + "/levels/" + levelOrder_list[j];
			CustomLevel* level;
			try {
				level = processCustomLevel(levelPath);
			}
			catch (const std::runtime_error& e) {
				std::cerr << "ERROR: Unable to process " + levelPath + ", reason: " << e.what() << std::endl;
				continue;
			}

			if (level == nullptr) { [[unlikely]]
				//std::cerr << "WARNING: Bad path \"" + levelPath + "\"" << std::endl;
				continue;
			} else {
				try {
					addCustomLevel(level);
				}
				catch (const std::runtime_error& e) {
					std::cerr << "ERROR: Unable to materialize " + levelPath + ", reason: " << e.what() << std::endl;
					delete level;
					continue;
				}
			}

			std::cout << "Processed custom level: " + levelPath << std::endl;
		}
	}

	std::cout << std::endl;
	*/
}
