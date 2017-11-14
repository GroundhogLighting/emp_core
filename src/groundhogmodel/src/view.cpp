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


#include "common/utilities/io.h"
#include "./view.h"

View::View() {

	DEBUG_MSG("Creating a view ");
}

View::~View() {
	DEBUG_MSG("Destroying view " + name);
	delete viewPoint;
	delete viewDirection;
	delete viewUp;
}

void View::setName(std::string newName) {
	name = newName;
}

std::string View::getName() {
	return name;
}

void View::setViewPoint(Point3D * point) {
	viewPoint = point;
}
Point3D * View::getViewPoint() {
	return viewPoint;
}


void View::setViewDirection(Vector3D * vector) {
	viewDirection = vector;
}

Vector3D * View::getViewDirection() {
	return viewDirection;
}

void View::setViewUp(Vector3D * vector) {
	viewUp = vector;
}

Vector3D * View::getViewUp() {
	return viewUp;
}

void View::setViewHorizontal(double vH) {
	viewHorizontal = vH;
}

double View::getViewHorizontal() {
	return viewHorizontal;
}

void View::setViewVertical(double vV) {
	viewVertical = vV;
}

double View::getViewVertical() {
	return viewVertical;
}

void View::setViewType(int vT) {
	viewType = vT;
}

int View::getViewType() {
	return viewType;
}