#ifndef POINT_2D
#define POINT_2D

class Point2d {
public:
	Point2d(int px, int py) : x(px), y(py) {}
	int x, y;
	
	bool operator==(const Point2d& other) {
		return x == other.x && y == other.y;
	}
	
	bool operator!=(const Point2d& other) {
		return !(*this == other);
	}
	
	friend std::ostream& operator<<(std::ostream& out, const Point2d& point){
		out << "(" << point.x << ", " << point.y << ") ";
	}
};


#endif
