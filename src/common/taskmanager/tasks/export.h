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
