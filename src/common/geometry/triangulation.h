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

#pragma once

#include <vector>

#include "./point3d.h"
#include "./polygon.h"
#include "./triangle.h"

#define MAX_ASPECT_RATIO 1.3

// Represents a Triangulation.
/*!
This class is used before exporting a Workplane object, which has to be 
subdivided in little Triangle objects.

A core part of this class is the fast-poly3tri library (https://github.com/MetricPanda/fast-poly2tri)
which is licensed MIT.
*/


class Triangulation {

private:
	std::vector < Triangle * > triangles; //!< The current triangles
	Polygon3D * polygon; //!< The polygon to triangulate
	size_t nTriangles = 0; //!< The number of triangles available    

public:

  //! Default constructor
  /*!
  @author German Molina
  */
  Triangulation();

  //! Constructor
  /*!
  @author German Molina
  @param[in] polygon The polygon to triangulate
  */
  Triangulation(Polygon3D * polygon);

  //! Destructor
  /*!	
  Destroys all the triangles as well.
  @author German Molina
  */
  ~Triangulation();

  //! Adds a triangle to the Triangulation
  /*!
  Receives a pointer to a Triangle, and adds it to the
  Triangulation. It also updates the nTriangles
  member and sets the index of the added Triangle

  @author German Molina
  @param[in] t The triangle to add.
  @return The index of the new triangle
  */
  size_t addTriangle(Triangle * t);
	
  //! Returns the number of Triangle objects in the Triangulation
  /*!
  @author German Molina
  @return the number of triangles
  */
  size_t getNumTriangles();
	
  //! Gets the pointer to the triangle with certain index
  /*!
  @author German Molina
  @param[in] i The index of the Triangle to retrieve
  @return The reference to the triangle
  */
  Triangle * getTriangleRef(size_t i);

  //! Adds a Point3D to a certain Triangle
  /*!
  Receives a Point3D to be added to a Triangle, and the code
  that represent its location (see Triangle::testPoint()); and
  'decides' whether to split the triangle, split an edge or
  just ignore the Point3D added (if it is a vertex of the Triangle)

  @author German Molina
  @param[in] index The index of the Triangle to which the Point3D will be added
  @param[in] point The pointer to the Point3D to add
  @param[in] code The code of the position of point in triangle (see Triangle::testPoint())
  @return success
  */
  bool addPointToTriangle(size_t index, Point3D * point, int code);

  //! Adds a Point3D to the Triangulation
  /*!
  Looks for the first Triangle on which the Point3D
  given is contained, and adds it to it. 

  Neighboring Triangles will be updated if needed 
  (see Triangulation::splitTriangle() and 
  Triangulation::splitEdge())

  @author German Molina
  @param[in] point The Point3D to add
  @param[in] warn Warn if point is not in triangulation?
  @return success
  */
  bool addPoint(Point3D * point, bool warn);
	
  //! Flips a diagonal of a Triangle and one of its neighbor
  /*!
  @author German Molina
  @param[in] index The index of the Triangle to flip
  @param[in] neighbor The neighbor with which the diagonal will be flipped
  */
  void flipDiagonal(size_t index, int neighbor);

  //! Checks if a combination of four Point3D objects would form a convex polygon
  /*!
  This function is required to see if the diagonal can be flipped 
  (see Triangulation::flipDiagonal()). Non-convex shapes do not allow
  flipping diagonals

  @author German Molina
  @return True or False
  */
  bool isConvex(Point3D * a, Point3D * b, Point3D * c, Point3D * d);

  //! Tests the best aspect ratio of a Triangle and one of its neighbors
  /*!
  Returns the best aspect ratio achievable by flipping (or not) the 
  diagonal of a Triangle and one of its neighbors.

  If (best aspect ratio) < (current aspect ratio), then flip

  @author German Molina
  @param[in] t A Triangle to test
  @param[in] edge The edge
  @return the best aspect ratio
  */
  double getBestAspectRatio(Triangle * t, int edge);

  //! Restore the best possible Constrained Delaunay Triangulation
  /*!
  Checks all the Triangle objects in the Triangulation, and
  decides if flipping the diagonal makes sense or not.

  @author German Molina
  */
  void restoreDelaunay();

  //! Splits a Triangle by inserting a Point3D inside it
  /*!
  When a Point3D added to the Triangulation falls inside a Triangle,
  the Triangle will be splitted in 3 new Triangle objects.

  All neighborhoods and constraints are updated, and the original
  Triangle is deleted.
 
  @author German Molina
  @param[in] index The index of the Triangle to split
  @param[in] p The pointer to the Point3D to add
  @return success
  */
  bool splitTriangle(size_t index, Point3D * p);

  //! Splits a Triangle's edge by inserting a Point3D 
  /*!
  When a Point3D added to the Triangulation falls on an edge of 
  a Triangle,	the Triangle will be splitted in 2 new Triangle objects.

  All neighborhoods and constraints are updated, and the original
  Triangle is deleted.

  @author German Molina
  @param[in] index The index of the Triangle to split
  @param[in] p The pointer to the Point3D to add
  @param[in] code The code that represents the position of p in the Triangle
  @return success;
  */
  bool splitEdge(size_t index, Point3D * p, int code);

  //! Returns the Point3D that is part of a neighbor Triangle but not of itself
  /*!
  Two neighboring Triangles form a 4-vertex polygon.
  This function returns a pointer to the Point3D that is
  part of the neighbor and not of the given triangle

  @author German Molina
  @param[in] t The triangle
  @param[in] nei The neighbor number
  @return the pointer to the oposite vertex
  */
  Point3D * getOpositeVertex(Triangle * t, int nei);

  //! Deletes a Triangle
  /*!
  Deletes a Triangle from the Triangulation, and sets NULL
  to its position in the trinangles vector.

  @author German Molina
  @param[in] i The index of the Triangle to delete
  */
  void deleteTriangle(size_t i);

  //! Removes all Triangles that are not part of the Polygon3D of the Triangulation
  /*!
  This method tests the center of the Triangle (average of 3 vertices)
  and if it is not inside the Polygon3D of the Triangulation, deletes it.

  @author German Molina
  */
  void clean();

  //! Perfoms further subdivision of the current Triangulation
  /*!
  Somehow inspired on "Ruppert, J. (1995). A Delaunay refinement 
  algorithm for quality 2-dimensional mesh generation. Journal of 
  algorithms, 18(3), 548-585."

  @param[in] maxArea The maximum desired area of each Triangle
  @param[in] maxAspectRatio The maximum aspect ratio allowed
  @todo Check if the maxArea param makes any sense.
  */
  void refine(double maxArea, double maxAspectRatio);

  //! Performs constrained delaunay triangulation and refines a Triangulation
  /*!
  @author German Molina
  @param[in] maxArea The maxArea param given to Triangulation::refine() method
  @param[in] maxAspectRatio The maximum aspect ratio allowed
  */
  bool mesh(double maxArea, double maxAspectRatio);

  //! Analizes the Triangle objects in the Triangulation, and sets all the neighbors
  /*!
  @author German Molina
  @note It is assumed that all the triangles have 3 NULL neighbors. Otherwise, weird stuff may happen. 
  */
  void resetNeighborhoods();

  //! Generates a Constrained Delaunay Triangulation
  /*!
  This function calls the fast-poly2tri library in order
  to perform a Constrained Delaunay Triangulation of the
  Triangulation 's Polygon3D.

  @author German Molina
  @return success
  */
  bool doCDT();

  //! Retrieves the pointer to the Polygon3D
  /*!
  @author German Molina
  @return The reference to the Polygon3D
  */
  Polygon3D * getPolygon();

  //! Counts the non-NULL triangles
  /*!
  @author German Molina
  @return Triangles
  */
  size_t realSize();

  //! Removes NULL triangles from the triangles array
  /*!
  @author German Molina
  */
  void purge();

};

extern Triangulation triangulation;

