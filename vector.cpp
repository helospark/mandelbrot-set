#include "vector.h"
#include <algorithm>
#include <cstring>
#include "generalException.h"

Vector::Vector() {
	x = y = z = 0.0;
}
Vector::Vector(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(const Vector& vector) {
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
}

Vector Vector::operator+(const Vector& vector) {
	double newX = this->x + vector.x;
	double newY = this->y + vector.y;
	double newZ = this->z + vector.z;
	return Vector(newX, newY, newZ);
}

Vector Vector::operator*(double rhs) {
	double newX = this->x * rhs;
	double newY = this->y * rhs;
	double newZ = this->z * rhs;
	return Vector(newX, newY, newZ);
}

Vector& Vector::operator=(const Vector& vector) {
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
	return *this;
}

Vector Vector::interpolate(const Vector& vector, double factor) const {
	double newX = x + (vector.x - x) * factor;
	double newY = y + (vector.y - y) * factor;
	double newZ = z + (vector.z - z) * factor;
	return Vector(newX, newY, newZ);
}

double Vector::getX() {
	return x;
}

double Vector::getY() {
	return y;
}

double Vector::getZ() {
	return z;
}

Vector& Vector::parseFromString(const std::string& data) {
	std::string localData = data;
	const char* charsToRemove = " \t()";
  for (unsigned int i = 0; i < std::strlen(charsToRemove); ++i) {
    localData.erase(std::remove(localData.begin(), localData.end(), charsToRemove[i]), localData.end());
  }
  size_t firstSeparator = localData.find(",");
  size_t lastSeparator = localData.find_last_of(",");
  if (firstSeparator == std::string::npos || lastSeparator == std::string::npos) {
  	throw GeneralException("Cannot parse vector data, missing ',' separator(s)");
  }
  this->x = atof(localData.substr(0, firstSeparator).c_str());
  this->y = atof(localData.substr(firstSeparator + 1, lastSeparator - firstSeparator).c_str());
  this->z = atof(localData.substr(lastSeparator + 1, std::string::npos).c_str());
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Vector& vector) {
	out << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return out;
}
