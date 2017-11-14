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

#include "groundhogmodel/groundhogmodel.h"
#include "writers/rad/radexporter.h"
#include "common/geometry/triangulation.h"
#include "common/utilities/io.h"

class ExportRadianceDir : public Task {
private:
	std::string dir;
	GroundhogModel * model;
	bool verbose;

public:

	ExportRadianceDir(std::string theDir, GroundhogModel * ghmodel, bool verb)
	{
		setName("Export model in Radiance format in directory '" + theDir + "'");

		dir = theDir;
		model = ghmodel;
		verbose = verb;
	}

	bool isEqual(Task * t)
	{
		return (
					model == static_cast<ExportRadianceDir *>(t)->getModel() &&
					dir == static_cast<ExportRadianceDir *>(t)->getDir()  
				);
	}

	bool solve()
	{
		RadExporter r = RadExporter(model, dir, verbose);
		return r.exportModel();
	}

	std::string getDir()
	{
		return dir;
	}

	GroundhogModel * getModel()
	{
		return model;
	}
};





class TriangulateWorkplane : public Task {
private:
	std::string workplane;
	GroundhogModel * model;
	Triangulation * result;

public:
	TriangulateWorkplane(GroundhogModel * theModel, std::string theWorkplane)
	{
		setName("Triangulating workplane " + theWorkplane);
		workplane = theWorkplane;
		model = theModel;
	}

	bool isEqual(Task * t)
	{	
		TriangulateWorkplane * otherT = static_cast<TriangulateWorkplane *>(t);
		return workplane == otherT->getWorkplane();
	}

	bool solve()
	{
		Workplane * wp = model->getWorkplaneByName(workplane);
		if (wp == NULL) {
			warn("Workplane " + workplane + " was not found in model");
			return false;
		}

		result = new Triangulation(wp->getPolygonRef(0));

		result->mesh(0.5);
		std::cerr << result << std::endl;
		return true;
	}

	std::string getWorkplane()
	{
		return workplane;
	}

	Triangulation * getResult()
	{
		return result;
	}

};

class ExportRadianceDirWithWorkplanes : public Task {
private:
	std::string dir;
	GroundhogModel * model;
	bool verbose;

public:

	ExportRadianceDirWithWorkplanes(std::string theDir, GroundhogModel * ghmodel, bool verb)
	{
		setName("Export model in Radiance format in directory '" + theDir + "' including workplanes");

		dir = theDir;
		model = ghmodel;
		verbose = verb;
	}

	bool isEqual(Task * t)
	{
		return (
			model == static_cast<ExportRadianceDir *>(t)->getModel() &&
			dir == static_cast<ExportRadianceDir *>(t)->getDir()
			);
	}

	bool solve()
	{
		RadExporter r = RadExporter(model, dir, verbose);
		return r.exportModelWithWorkplanes();
	}

	std::string getDir()
	{
		return dir;
	}

	GroundhogModel * getModel()
	{
		return model;
	}
};