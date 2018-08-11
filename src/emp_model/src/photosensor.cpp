/*****************************************************************************
	Emp

    Copyright (C) 2018  German Molina (germolinal@gmail.com)

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

#include "./photosensor.h"

Photosensor::Photosensor(std::string n)
{
	name = n;
}


std::string Photosensor::getName()
{
	return name;
}

Vector3D Photosensor::getDirection()
{
	return direction;
}

void Photosensor::setDirection(Vector3D d)
{
	direction = d;
}


Point3D Photosensor::getPosition()
{
	return position;
}

void Photosensor::setPosition(Point3D p)
{
	position = p;
}
