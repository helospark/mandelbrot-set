#include "vector.h"

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

std::ostream& operator<<(std::ostream& out, const Vector& vector) {
	out << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")" << std::endl;
	return out;
}
