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

#pragma once

#include <vector>

#include "./point3d.h"
#include "./polygon.h"
#include "./triangle.h"

#define MAX_ASPECT_RATIO 1


class Triangulation {

private:
	std::vector < Point3D * > points; //!< The current points in the triangulations
	std::vector < Segment * > constraints; //!< The contraints to apply
	std::vector < Triangle * > triangles; //!< The current triangles
	Polygon3D * polygon; //!< The polygon to triangulate
	size_t nTriangles = 0; //!< The number of triangles available

public:

	Triangulation(Polygon3D * polygon);

	~Triangulation();

	size_t addTriangle(Triangle * t);
	//void addContraintsFromLoop(Loop * loop);


	//bool setSuperRectangle();

	size_t getNumTriangles();
	Triangle * getTriangleRef(size_t i);

	bool addPoint(Point3D * point);

	//bool doDT();


	void restoreDelaunay();

	bool splitTriangle(size_t i, Point3D * p);

	bool splitEdge(size_t i, Point3D * p, int code, bool constraint);

	Point3D * getOpositeVertex(Triangle * t, int i);

	void deleteTriangle(size_t i);

	double getBestAspectRatio(Triangle * t, int i, bool * changed);


	void flipDiagonal(size_t index, int i, bool constraint);

	bool isConvex(Point3D * a, Point3D * b, Point3D * c, Point3D * d);

	//void applyConstraint(Segment * s);


	bool addPointToTriangle(size_t index, Point3D * point, int code);

	//bool doCDT();

	void clean();


	void refine(double maxLength);

	bool mesh(double maxArea);

	//bool applyConstraintToTriangle(Triangle * tri, Segment * constraint);

	//bool mergeIfPossible(Triangle * tri, int neighbor);

	void poly2tri();

	void resetNeighborhoods();
};