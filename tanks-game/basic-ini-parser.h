#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class BasicINIParser {
	//only reads, no writing

public:
	struct BasicINIData {
		//to support writing, would probably need list with every line
		friend class BasicINIParser;
		BasicINIData() {}

	protected:
		std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> data;
		void insert(std::string section, std::string property, std::string value);

	public:
		bool exists(std::string section, std::string property) const;
		int length(std::string section, std::string property) const;
		std::string get(std::string section, std::string property) const;
		std::string get(std::string section, std::string property, int index) const;
	};

private:
	//actual parsing
	static void removeLeftWhitespace(std::string&);
	static void removeComments(std::string&);
	static void removeRightWhitespace(std::string&);
	static int findEndSectionIndex(const std::string&);
	static int findSeparatorIndex(const std::string&);
	static int findSubstringEndIndex(const std::string&); //returns the index after the last character in the substring
	static void processEscapeSequences_most(std::string&);
	static void processEscapeSequences_all(std::string&); //includes \n and \t

public:
	static BasicINIData readFile(std::string path);

private:
	BasicINIParser() = delete;
	BasicINIParser(const BasicINIParser&) = delete;
};
