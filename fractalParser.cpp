#include "fractalParser.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <cstring>
#include <sstream>
#include "generalException.h"
#include "FractalType.h"

std::vector<std::string> FractalParser::readAllLinesFromFile(const std::string& fileName) {
	std::vector<std::string> result;
	
	std::ifstream in(fileName.c_str());
	if (!in.is_open()) {
		throw GeneralException("File cannot be opened");
	}
	
	std::string line;
	while (std::getline(in, line)) {
		  result.push_back(line);
	}
	
	return result;
}

std::vector<std::string>& FractalParser::removeCommentsAndWhitespaces(std::vector<std::string>& lines) {
	for (int i = 0; i < lines.size(); ++i) {
		std::string line = lines[i];
		line = line.substr(0, line.find("#"));
		
		const char* charsToRemove = " \t";
	  for (unsigned int j = 0; j < std::strlen(charsToRemove); ++j) {
      line.erase(std::remove(line.begin(), line.end(), charsToRemove[j]), line.end());
    }
		
		lines[i] = line;
	}
	return lines;
}

std::map<std::string, std::string> FractalParser::parseProperties(const std::vector<std::string>& lines) {
	std::map<std::string, std::string> result;
	
	for (int i = 0; i < lines.size(); ++i) {
		const std::string& line = lines[i];
		size_t equalPosition = line.find("=");
		if (equalPosition != std::string::npos) {
			result.insert(std::pair<std::string, std::string>(line.substr(0, equalPosition), line.substr(equalPosition + 1, std::string::npos)));
		}
	}
	
	return result;
}

int FractalParser::parsePropertyToInt(std::map<std::string, std::string>& properties, std::string propertyName, int defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		try {
			return atoi(properties[propertyName].c_str());
		} catch (const std::exception& exception) {
			throw new GeneralException(std::string("Could not convert property ") + propertyName + " to int");
		}
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

double FractalParser::parsePropertyToDouble(std::map<std::string, std::string>& properties, std::string propertyName, double defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		try {
			return atof(properties[propertyName].c_str());
		} catch (const std::exception& exception) {
			throw new GeneralException(std::string("Could not convert property ") + propertyName + " to double");
		}
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}


bool FractalParser::parsePropertyToBool(std::map<std::string, std::string>& properties, std::string propertyName, bool defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		std::string& property = properties[propertyName];
		return (property == "true" || property == "1");
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

Complex FractalParser::parsePropertyToComplex(std::map<std::string, std::string>& properties, std::string propertyName, const Complex& defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		std::string& property = properties[propertyName];
		Complex result;
		try {		
			return result.parseFromString(property);
		} catch (const std::exception& exception) {
			throw new GeneralException(std::string("Could not convert property ") + propertyName + " to complex");
		}
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

std::string FractalParser::parsePropertyToString(std::map<std::string, std::string>& properties, std::string propertyName, const std::string& defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		return properties[propertyName];
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

Vector FractalParser::parsePropertyToVector(std::map<std::string, std::string>& properties, std::string propertyName, const Vector& defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		return Vector().parseFromString(properties[propertyName]);
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

std::vector<ColorInterpolation> FractalParser::parseColors(const std::vector<std::string>& lines, double iterationCorrection) {
	std::vector<ColorInterpolation> result;
	int startLine = 0;
	for (startLine = 0; startLine < lines.size(); ++startLine) {
		if (lines[startLine].find("colors") == 0) {
			break;
		}
	}
	int i = startLine + 1;
	// parses (x,y,z)(x,y,z)d
	while (i < lines.size() && lines[i].find("}") == std::string::npos) {
		size_t firstSeparator = lines[i].find(")");
		size_t lastSeparator = lines[i].find_last_of(")");
		Vector startColor;
		startColor.parseFromString(lines[i].substr(0, firstSeparator + 1));
		Vector endColor;
		endColor.parseFromString(lines[i].substr(firstSeparator + 1, lastSeparator - firstSeparator));
		double multiplier = atof(lines[i].substr(lastSeparator + 1, std::string::npos).c_str()) * iterationCorrection;
		result.push_back(ColorInterpolation(startColor, endColor, multiplier));
		++i;
	}
	if (i == lines.size()) {
		std::cout << "Warning: missing '}'" << std::endl;
	}
	return result;
}

FractalParams FractalParser::readFractal(std::string fileName) {
	FractalParams result;
	std::vector<std::string> lines = readAllLinesFromFile(fileName);
	removeCommentsAndWhitespaces(lines);
	std::map<std::string, std::string> properties = parseProperties(lines);
	result.width = parsePropertyToInt(properties, "width", 1920);
	result.height = parsePropertyToInt(properties, "height", 1200);
	result.iterationLimit = parsePropertyToInt(properties, "iteration_limit", 1000);
	result.minX = parsePropertyToDouble(properties, "min_x", -2);
	result.maxX = parsePropertyToDouble(properties, "max_x", 2);
	result.minY = parsePropertyToDouble(properties, "min_y", -1.8);
	result.maxY = parsePropertyToDouble(properties, "max_y", 1.8);
	double iterationCorrection = result.iterationLimit / parsePropertyToInt(properties, "color_iteration_count", 1000);
	result.fileName = parsePropertyToString(properties, "filename", "mandelbrot");
	result.type = parsePropertyToString(properties, "type", FractalType::MANDELBROT);
	result.maxIterationReachedColor = parsePropertyToVector(properties, "max_iteration_reached_color", Vector(0,0,0));
	if (result.type == FractalType::JULIA) {
		result.juliaSetStartParameter = parsePropertyToComplex(properties, "julia_set_start_parameter", Complex(-0.4, 0.6));
	}
	if (result.type == FractalType::JULIA_AROUND_MANDELBROT) {
    result.outlineMandelbrotWidth = parsePropertyToInt(properties, "outlineMandelbrotWidth", 1920);
	  result.outlineMandelbrotHeight = parsePropertyToInt(properties, "outlineMandelbrotHeight", 1080);
	  result.outlineInterpolationCount = parsePropertyToInt(properties, "outlineInterpolationCount", 3);
	}
	
	std::vector<ColorInterpolation> colors = parseColors(lines, iterationCorrection);
	result.colors = colors;
	return result;
}
