#ifndef FRACTAL_PARSET_H
#define FRACTAL_PARSER_H
#include "fractalParams.h"
#include <string>
#include <vector>
#include <map>

class FractalParser {
	std::vector<std::string> readAllLinesFromFile(const std::string& fileName);
	std::vector<std::string>& removeCommentsAndWhitespaces(std::vector<std::string>& lines);
	std::map<std::string, std::string> parseProperties(const std::vector<std::string>& lines);
	int parsePropertyToInt(std::map<std::string, std::string>& properties, std::string propertyName, int defaultValue);
	double parsePropertyToDouble(std::map<std::string, std::string>& properties, std::string propertyName, double defaultValue);
	bool parsePropertyToBool(std::map<std::string, std::string>& properties, std::string propertyName, bool defaultValue);
	Complex parsePropertyToComplex(std::map<std::string, std::string>& properties, std::string propertyName, const Complex& defaultValue);
	Vector parsePropertyToVector(std::map<std::string, std::string>& properties, std::string propertyName, const Vector& defaultValue);
	std::string parsePropertyToString(std::map<std::string, std::string>& properties, std::string propertyName, const std::string& defaultValue);
	std::vector<ColorInterpolation> parseColors(const std::vector<std::string>& lines, double iterationCorrection);
	public:
		FractalParams readFractal(std::string fileName);
};
#endif
