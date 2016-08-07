#include "argumentParser.h"
	
void ArgumentParser::parse(int argc, char** argv, const std::vector<Argument>& arguments) {
	shortNameToArgument.clear();
	longNameToArgument.clear();
	for (Argument argument : arguments) {
		shortNameToArgument[argument.shortName] = argument;
		longNameToArgument[argument.longName] = argument;
	}
	std::vector<std::string> commandLineArguments;
	for (int i = 1; i < argc; ++i) {
		commandLineArguments.push_back(std::string(argv[i]));
	}
	for (unsigned int i = 0; i < commandLineArguments.size(); ++i) {
		parse(commandLineArguments, i);
	}
}

void ArgumentParser::parse(const std::vector<std::string>& stringArgument, unsigned int& index) {
	std::string currentArgument = stringArgument[index];
	if (currentArgument.length() < 1 || currentArgument[0] != '-') {
		throw GeneralException("Argument can only start with '-'");
	}
	if (currentArgument[1] == '-') {
		std::string argumentName = currentArgument.substr(2);
		if (longNameToArgument.find(argumentName) == longNameToArgument.end()) {
			throw GeneralException(std::string("Argument '") + argumentName + "' is invalid"); 
		}
		Argument argument = longNameToArgument.at(argumentName);
		parsedArguments[argument.shortName] = parseArgument(stringArgument, argument.type, index);
	} else {
		for (unsigned int i = 1; i < currentArgument.size(); ++i) {
			if (shortNameToArgument.find(currentArgument[i]) == shortNameToArgument.end()) {
				throw GeneralException(std::string("Argument '") + currentArgument[i] + "' is invalid"); 
			}
			Argument argument = shortNameToArgument.at(currentArgument[i]);
			if (argument.type != BOOLEAN_ARGUMENT && i < currentArgument.size() - 1) {
				throw GeneralException("Numeric argument cannot be in the middle of bool arguments");
			}
			parsedArguments[argument.shortName] = parseArgument(stringArgument, argument.type, index);
		}
	}
}

bool isPositiveNumeric(const std::string& str) {
  for (unsigned int i = 0; i < str.size(); ++i) {
    if (str[i] < '0' || str[i] > '9') {
      return false;
    }
  }
  return true;
}

std::string ArgumentParser::parseArgument(const std::vector<std::string>& stringArgument, const ArgumentType type, unsigned int& index) {
	if (type == BOOLEAN_ARGUMENT) {
		return "1";
	}
	else if (type == INTEGER_ARGUMENT) {
		++index;
		if (!isPositiveNumeric(stringArgument[index])) {
			throw GeneralException("Expect numeric value for argument");
		}
		return stringArgument[index];
	} else if (type == STRING_ARGUMENT) {
		++index;
		return stringArgument[index];
	} else {
		throw GeneralException("Unknown argument type");
	}
}

std::map<std::string, std::string> ArgumentParser::getRawMap() {
  std::map<std::string, std::string> result;
  for (auto it : parsedArguments) {
    const Argument& argument = shortNameToArgument[it.first];
    result[argument.longName] = it.second;
  }
  return result;
}
	
std::string ArgumentParser::getStringArgument(char shortName) const {
	return parsedArguments.at(shortName);
}

int ArgumentParser::getIntArgument(char shortName) const {
	return atoi(parsedArguments.at(shortName).c_str());
}

bool ArgumentParser::isArgumentPresent(char shortName) const {
	return parsedArguments.find(shortName) != parsedArguments.end();
}

std::string ArgumentParser::getStringArgumentOrReturnDefault(char shortName, const std::string& defaultValue) const {
	if (parsedArguments.find(shortName) != parsedArguments.end()) {
		return parsedArguments.at(shortName);
	} else {
		return defaultValue;
	}
}
int ArgumentParser::getIntArgumentOrReturnDefault(char shortName, int defaultValue) const {
	if (parsedArguments.find(shortName) != parsedArguments.end()) {
		return atoi(parsedArguments.at(shortName).c_str());
	} else {
		return defaultValue;
	}
}
