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
#include "../task.h"
#include "common/taskmanager/tasks/sky.h"
#include "calculations/radiance.h"

class CalcDF : public Task {
private:
	std::string workplane;
	std::string octreename;
	double albedo;
	std::string dir;
	GroundhogModel * model;
	std::vector<RAY> rays = std::vector<RAY>();
	RTraceOptions * options;

public:

	CalcDF(std::string theDir, GroundhogModel * ghmodel, std::string theWorkplane, RTraceOptions * theOptions)
	{
		setName("Calculate DF for workplane "+workplane);

		options = theOptions;
		dir = theDir;
		model = ghmodel;
		albedo = model->getLocation()->getAlbedo();
		workplane = theWorkplane;
		octreename = "df.oct";
		addDependency(new DFOconv(model,dir,octreename));	
		
		addDependency(new TriangulateWorkplane(model, theWorkplane)); // Dependency 1


	}

	bool isEqual(Task * t)
	{
		return (
			model == static_cast<CalcDF *>(t)->getModel() &&
			dir == static_cast<CalcDF *>(t)->getDir() &&
			albedo == static_cast<CalcDF *>(t)->getAlbedo()
		);
	}

	bool solve()
	{
		Workplane * wp = model->getWorkplaneByName(workplane);

		if (wp == NULL)
			return false;

		Triangulation * triangulation = static_cast<TriangulateWorkplane *>(getDependencyRef(1))->getResult();

		rays.resize(triangulation->getNumTriangles());
		
		rtrace_I(triangulation, options, GLARE_TMP_DIR, octreename, "df.amb",&rays);
		
		for (size_t i = 0; i < rays.size(); i++) {

			// Check if triangle still exists
			if (triangulation->getTriangleRef(i) == NULL)
				continue;

			// USE RESULTS
			double red = colval(rays[i].rcol, RED);
			double green = colval(rays[i].rcol, GRN);
			double blue = colval(rays[i].rcol, BLU);

			std::cout << "Ray origin: P(" << colval(rays[i].rorg, RED) << "," << colval(rays[i].rorg, GRN) << "," << colval(rays[i].rorg, BLU);
			std::cout << "," << ") -  Color: (" << red << "," << green << "," << blue << ")";
			std::cout << " -- " << GETLUX(red,green,blue) <<  std::endl;
		}
		return true;
	}

	GroundhogModel * getModel()
	{
		return model;
	}

	std::string getDir()
	{
		return dir;
	}

	double getAlbedo()
	{
		return albedo;
	}

	std::string getWorkplane()
	{
		return workplane;
	}
};