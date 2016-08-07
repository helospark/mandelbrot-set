#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <string>
#include <functional>

class Vector {
	public:
		double x, y, z;
	public:
		Vector();
		Vector(double x, double y, double z);
		Vector(const Vector& vector);
		
		Vector operator+(const Vector& vector);
		Vector operator*(double rhs);
		Vector& operator=(const Vector& vector);
		bool operator==(const Vector& vector) const {
			return x == vector.x && y == vector.y && z == vector.z;
		
		}
		
		Vector interpolate(const Vector& vector, double factor) const;
		
		double getX();
		double getY();
		double getZ();
		
		Vector& parseFromString(const std::string& data);
		
		friend std::ostream& operator<<(std::ostream& out, const Vector& vector);
};

namespace std {
template <>
  struct hash<Vector> {
    size_t operator()(const Vector& vector) const {
        return hash<double>()(vector.x) ^ hash<double>()(vector.y) ^ hash<double>()(vector.z);
    }
  };
}
#endif
