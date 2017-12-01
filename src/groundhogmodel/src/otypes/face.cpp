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


#include "./face.h"

#include "common/utilities/io.h"
#include "config_constants.h"

Face::Face(std::string faceName) 
{
	DEBUG_MSG("Creating face " + faceName);

	setName(faceName);
	polygon = new Polygon3D();

}

Face::~Face() 
{
	//destroy polygon
	delete polygon;	
	DEBUG_MSG("Destroying face "+getName());
}



void Face::setPolygon(Polygon3D * newPolygon) 
{
	delete polygon;
	polygon = newPolygon;
}

bool Face::hasInnerLoops() 
{
	return polygon->hasInnerLoops();
}

bool Face::hasTooManyInnerLoops() 
{
	return polygon->countInnerLoops() > GLARE_TOO_MANY_LOOPS;
}

Loop * Face::getOuterLoopRef() 
{
	return polygon->getOuterLoopRef();
}

Loop * Face::getClosedLoop() 
{
	return polygon->getClosedLoop();
}

bool Face::writeInRadianceFormat(FILE * file)
{
  // get the name of the face
  std::string faceName = getName();
  
  if (hasTooManyInnerLoops()) {
    warn("Ignoring face '" + faceName + "' because it has TOO MANY inner loops.");
    // writeTriangulatedFace(file,face);
    return true;
  }


  // get the material
  Material * mat = getMaterial();
  if (mat == NULL) {
    fatal("Face " + faceName + " has no Material... it has been ignored", __LINE__, __FILE__);
    return false;
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

  fprintf(file, "%s polygon %s\n0\n0\n", mat->getName().c_str(), &faceName[0]);
  
  fprintf(file,"%zd\n",3 * finalLoop->realSize());

  // Print the loop
  size_t numVertices = finalLoop->size();

  for (int i = 0; i < numVertices; i++) {
    Point3D * point = finalLoop->getVertexRef(i);

    if (point == NULL)
      continue;

      fprintf(file, "\t");
      fprintf(file, "%f %f %f\n", point->getX(), point->getY(), point->getZ());      
    }


    if (needToDelete) {
      delete finalLoop;
    }

    return true;
}