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
		void insert(std::string section, std::string property, std::string value) noexcept;

	public:
		bool exists(std::string section, std::string property) const noexcept;
		unsigned int length(std::string section, std::string property) const noexcept;
		std::string get(std::string section, std::string property) const;
		std::string get(std::string section, std::string property, int index) const;
	};

public:
	static BasicINIData ReadFile(std::string path);

public:
	//actual parsing
	static void removeLeftWhitespace(std::string&) noexcept;
	static void removeComments(std::string&) noexcept;
	static void removeRightWhitespace(std::string&) noexcept;
	static int findEndSectionIndex(const std::string&) noexcept;
	static int findSeparatorIndex(const std::string&) noexcept;
	static int findSubstringEndIndex(const std::string&) noexcept; //returns the index after the last character in the substring
	static void processEscapeSequences_most(std::string&) noexcept;
	static void processEscapeSequences_all(std::string&) noexcept; //includes \n and \t

private:
	BasicINIParser() = delete;
	BasicINIParser(const BasicINIParser&) = delete;
};
