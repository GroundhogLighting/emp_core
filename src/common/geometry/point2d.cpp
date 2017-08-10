/*****************************************************************************
Glare

Copyright (C) 2017  German Molina (germolinal@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/
#include<cmath>

#include "./point2d.h"

Point2D::Point2D(double new_x, double new_y)
{
	x = new_x;
	y = new_y;
}


Point2D::Point2D(Point2D * point)
{ // clone implementation
	x = point->getX();
	y = point->getY();
}

double Point2D::getX()
{
	return x;
}

double Point2D::getY()
{
	return y;
}


double Point2D::squaredDistanceTo(Point2D * point)
{
	double dx = (x - point->x)*(x - point->x);
	double dy = (y - point->y)*(y - point->y);
	return dx + dy;
}



double Point2D::distanceTo(Point2D * point)
{
	return sqrt(squaredDistanceTo(point));
}