/*
 * Rectangle.h
 *
 *  Created on: Jul 24, 2016
 *      Author: black
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_


struct Rectangle {
	double x0, y0;
	double x1, y1;

	Rectangle(double x0, double x1, double y0, double y1) {
		this->x0 = x0;
		this->x1 = x1;
		this->y0 = y0;
		this->y1 = y1;
	}
};


#endif /* RECTANGLE_H_ */
