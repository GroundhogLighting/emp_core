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

#include "./groundhogmodel/groundhogmodel.h"
#include "../task.h"
#include "config_constants.h"

#include <fstream>

class WriteDFSky: public Task {
private:
	std::string dir;
	std::string skyName;
	double albedo;

public:
	WriteDFSky(std::string theDir, double theAlbedo,GroundhogModel * model)
	{
		dir = theDir;
		albedo = theAlbedo;
		skyName = "cie_DF_sky_"+std::to_string(albedo)+".sky";
		setName("Write DF sky " + std::to_string(albedo*100.0) + "% at "+ theDir + "'");

		addDependency(new ExportRadianceDir(GLARE_TMP_DIR,model,false));
	}

	bool isEqual(Task * t)
	{
		return albedo == static_cast<WriteDFSky *>(t)->getAlbedo();
	}

	bool solve()
	{

		std::ofstream file;
		file.open(dir + "/" + GLARE_SKY_SUBFOLDER + "/" + skyName);

		file << "!gensky -ang 45 40 -c -B 0.5586592 -g " << albedo << std::endl;
		file << "skyfunc glow skyglow 0 0 4 0.99 0.99 1.1 0" << std::endl;
		file << "skyglow source skyball 0 0 4 0 0 1 360" << std::endl;

		file.close();
		return true;
	}

	double getAlbedo()
	{
		return albedo;
	}
	
	std::string getSkyName()
	{
		return skyName;
	}

};
