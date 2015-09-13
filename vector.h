#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>

class Vector {
	private: 
		double x, y, z;
	public:
		Vector();
		Vector(double x, double y, double z);
		Vector(const Vector& vector);
		
		Vector operator+(const Vector& vector);
		Vector operator*(double rhs);
		Vector& operator=(const Vector& vector);
		
		Vector interpolate(const Vector& vector, double factor) const;
		
		double getX();
		double getY();
		double getZ();
		
		friend std::ostream& operator<<(std::ostream& out, const Vector& vector);
};
#endif
