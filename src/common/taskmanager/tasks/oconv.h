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
#include "./export.h"

#include <iostream>
#include <stdio.h>

#include "common/utilities/os.h"
#include "common/utilities/file.h"

#include "common/taskmanager/tasks/sky.h"

class WholeSceneOconv : public Task {
private:
	GroundhogModel * model;
	std::string baseDir;
	std::string octname;

public:

	WholeSceneOconv(GroundhogModel * ghm, std::string dir, std::string name)
	{
		setName("Whole scene oconv "+dir+"/"+name);
		model = ghm;
		baseDir = dir;
		octname = name;

		addDependency(new ExportRadianceDir(dir,model,false));
	}

	bool isEqual(Task * t)
	{
		WholeSceneOconv * otherT = static_cast<WholeSceneOconv *>(t);
		return (
			model == otherT->getModel()		&&
			baseDir == otherT->getBaseDir() && 
			octname == otherT->getOctName()
		);
	}

	bool solve()
	{		
		CHDIR(&baseDir[0]);

		FILE *in;
		char buff[512];

		std::string materials = "./Materials/materials.mat";
		std::string scene = "scene.rad";
		std::string sky = "./"+ std::string(GLARE_SKY_SUBFOLDER) +"/sky.rad";
		std::string window = fexists("./Windows/windows.rad") ? "./Windows/windows.rad" : "";

		std::string cmd = "oconv " + materials + " " + sky + " " + scene + " " + window + " > " + octname;

		if (!(in = POPEN(&cmd[0]))) {
			return false;
		}

		while (fgets(buff, sizeof(buff), in) != NULL) {
			std::cout << buff;
		}
		PCLOSE(in);
		CHDIR("..");

		return true;
	}

	std::string getBaseDir()
	{
		return baseDir;
	}

	std::string getOctName()
	{
		return octname;
	}

	GroundhogModel * getModel()
	{
		return model;
	}

};


class DFOconv : public Task {
private:
	GroundhogModel * model;
	std::string baseDir;
	std::string octname;
	double albedo;
	std::string skyName;

public:

	DFOconv(GroundhogModel * ghm, std::string dir, std::string name)
	{
		setName("Whole scene oconv " + dir + "/" + name);
		model = ghm;
		baseDir = dir;
		octname = name;
		albedo = model->getLocation()->getAlbedo();

		addDependency(new ExportRadianceDir(dir, model, false));
		WriteDFSky * writeSky = new WriteDFSky(dir, albedo, model);
		skyName = writeSky->getSkyName();
		addDependency(writeSky);
	}

	bool isEqual(Task * t)
	{
		DFOconv * otherT = static_cast<DFOconv *>(t);
		return (
			model == otherT->getModel() &&
			baseDir == otherT->getBaseDir() &&
			octname == otherT->getOctName() &&
			albedo == otherT->getAlbedo()
		);
	}

	bool solve()
	{
		CHDIR(&baseDir[0]);

		FILE *in;
		char buff[512];

		std::string materials = "./Materials/materials.mat";
		std::string scene = "scene.rad";
		std::string sky = "./" + std::string(GLARE_SKY_SUBFOLDER) + "/" + skyName;
		std::string window = fexists("./Windows/windows.rad") ? "./Windows/windows.rad" : "";
		
		std::string cmd = "oconv " + materials + " " + sky + " " + scene + " " +window + " > " + octname;

		if (!(in = POPEN(&cmd[0]))) {
			return false;
		}

		while (fgets(buff, sizeof(buff), in) != NULL) {
			std::cout << buff;
		}
		PCLOSE(in);
		CHDIR("..");

		return true;
	}

	std::string getBaseDir()
	{
		return baseDir;
	}

	std::string getOctName()
	{
		return octname;
	}

	GroundhogModel * getModel()
	{
		return model;
	}

	double getAlbedo()
	{
		return albedo;
	}
};