#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "generalException.h"

enum ArgumentType {
	BOOLEAN_ARGUMENT,
	INTEGER_ARGUMENT,
	STRING_ARGUMENT
};

struct Argument {
	char shortName;
	std::string longName;
	std::string description;
	ArgumentType type;
	
	Argument(){};
	
	Argument(char shortName, std::string longName, std::string description, ArgumentType type) {
		this->shortName = shortName;
		this->longName = longName;
		this->description = description;
		this->type = type;
	}
};

/**
 * Quick and dirty implementation of command line argument parser.
 */
class ArgumentParser {
	std::map<char, std::string> parsedArguments;
	std::map<char, Argument> shortNameToArgument;
	std::map<std::string, Argument> longNameToArgument;
	void parse(const std::vector<std::string>& stringArgument, unsigned int& index);
	std::string parseArgument(const std::vector<std::string>& stringArgument, const ArgumentType type, unsigned int& index);
public:
	void parse(int argc, char** argv, const std::vector<Argument>& argument);
	
	std::string getStringArgument(char shortName) const;
	int getIntArgument(char shortName) const;
	bool isArgumentPresent(char shortName) const;
	
	std::string getStringArgumentOrReturnDefault(char shortName, const std::string& defaultValue) const;
	int getIntArgumentOrReturnDefault(char shortName, int defaultValue) const;

	std::map<std::string, std::string> getRawMap();
};

#endif
