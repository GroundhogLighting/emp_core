/*****************************************************************************
	Emp

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

#pragma once

#include "point3d.h"

//! A segment joining two Point3D objects

class Segment {

private:
	double length = -1; //!< The length of the segment

public:
	
	Point3D * start; //!< One end of the segment
	Point3D * end; //!< Another end of the segment

	//! Creates a new segment that goes from A to B
	/*!
	@author German Molina
	@param[in] a The start of the segment
	@param[in] b The end of the segment
	*/
	Segment(Point3D * a, Point3D * b);

	//! Compare if they are the same segment
	/*!
	@author German Molina
	@param[in] otherSegment The other segment
	*/
	bool isEqual(Segment * otherSegment);

	//! Calculates the length of the segment
	/*!
	Only calculates it if the length has not been calculated
	already

	@author German Molina
	@return The length
	*/
	double getLength();

	//! Intersects a Segment with another Segment
	/*!
	@author German Molina
	@param[in] input The segment to intersect with
	@param[out] output The Point3D where the segments intersect
	@return True if they intersect, false if not
	*/
	bool intersect(Segment * input, Point3D * output);
	
	//! Returns the midpoint of the segment
	/*!
	@author German Molina
	@return The midpoint
	*/
	Point3D midpoint();

};
