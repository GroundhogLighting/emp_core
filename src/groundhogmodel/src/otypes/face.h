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

#include "common/utilities/io.h"
#include "config_constants.h"

#include <string>
#include "./face.h"
#include "../material.h"
#include "../otype.h"

#include "common/geometry/polygon.h"

//! The integration of a Polygon3D and a Material

/*!
A Face represents physical objects of the model.
*/

class Face : public Otype {
private:
	Polygon3D * polygon; //!< The polygon that represents the geometry of the face

public:

	//! Creates a Face object
	/*!
	Assigns the name of the face and sets an empty Polygon3D

	@author German Molina
	@param[in] faceName The name of the face
	*/
	Face(std::string * faceName)
	{
		setName( faceName ); 
		polygon = new Polygon3D();
	}

	//! Destroys the Face
	/*!
	Also destroys the Polygon

	@author German Molina
	*/
	~Face()
	{
		delete polygon;	
	}

	

	//! Sets the Polygon3D of the Face
	/*!
	Deletes the current polygon, and adds the new one.

	@author German Molina
	@param[in] newPolygon
	*/
	void setPolygon(Polygon3D * newPolygon)
	{
		delete polygon;
		polygon = newPolygon;
	}

	//! Checks if the Face has inner Loop (or holes)
	/*!
	@author German Molina
	@return has inner Loop ?
	*/
	bool hasInnerLoops()
	{
		return polygon->hasInnerLoops();
	}

	//! Checks if the Face has too many inner Loop (or holes)
	/*!
	@author German Molina
	@return has too many inner Loop ?
	*/
	bool hasTooManyInnerLoops()
	{
		return polygon->countInnerLoops() > EMP_TOO_MANY_LOOPS;
	}

	//! Retrieves the reference to the Outer Loop of the Face
	/*!
	@author German Molina
	@return The reference to the Outer Loop
	*/
	Loop * getOuterLoopRef()
	{
		return polygon->getOuterLoopRef();
	}

	//! Retrieves the reference to a closed Loop of the Face Polygon3D
	/*!
	@author German Molina
	@return The reference to the new closed Loop
	@see Polygon3D::getClosedLoop()
	@note Should delete the closed Loop after its use
	*/
	Loop * getClosedLoop()
	{
		return polygon->getClosedLoop();
	}

    //! Writes a face in Radiance format
    /*!
    @author German Molina
    @param[in] file The file to write the face to
    @param[in] material The name of the Material to assign the face
    @param[in] transform The optional Transform object
    @return success
    */
    bool writeInRadianceFormat(FILE * file, const char * material, Transform * transform)
	{
				// get the name of the face
		std::string * faceName = getName();
		
		if (hasTooManyInnerLoops()) {
			WARN(warnMessage,"Ignoring face '" + *faceName + "' because it has TOO MANY inner loops.");
			// writeTriangulatedFace(file,face);
			return true;
		}


		// define the loop that will be written
		Loop * finalLoop = NULL;
		bool needToDelete = false;
		if (hasInnerLoops()) {
			finalLoop = getClosedLoop();
			needToDelete = true;
		}
		else {
			finalLoop = getOuterLoopRef();
		}

		fprintf(file, "%s polygon %s\n0\n0\n", material, &(faceName->at(0)));
		
		fprintf(file,"%zd\n",3 * finalLoop->realSize());

		// Print the loop
		size_t numVertices = finalLoop->size();

		for (int i = 0; i < numVertices; i++) {
			Point3D * point = finalLoop->getVertexRef(i);

			if (point == NULL)
			continue;

			fprintf(file, "\t");
			if (transform == NULL) {
			fprintf(file, "%f %f %f\n", point->getX(), point->getY(), point->getZ());      
			}
			else {
			Point3D p = point->transform(transform);
			fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
			}
		}


		if (needToDelete) {
			delete finalLoop;
		}

		return true;
	}

};
