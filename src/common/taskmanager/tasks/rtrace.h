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
#include "groundhogmodel/groundhogmodel.h"
#include "common/utilities/os.h"
#include "./oconv.h"
#include "./Radiance/src/rt/ray.h"


#include "Radiance/src/common/otypes.h"


class Rtrace : public Task {
private:
	GroundhogModel * model;
	std::string baseDir;
	std::string octname;

public:

	Rtrace(GroundhogModel * ghm, std::string dir, std::string name)
	{
		setName("Rtrace over octree "+dir+"/"+name);		
		baseDir = dir;
		octname = name;
		model = ghm;

		addDependency(new WholeSceneOconv(model,dir, name));
	}

	bool isEqual(Task * t)
	{
		return (
			model == static_cast<Rtrace * >(t)->getModel() &&
			baseDir == static_cast<Rtrace * >(t)->getBaseDir() &&
			octname == static_cast<Rtrace * >(t)->getOctName()
		);
	}

	bool solve()
	{

		// Move to the base directory
		CHDIR(&baseDir[0]);

		do_irrad = 0; // +i
		bool imm_irrad = true; // +I
		ambounce = 3;
		ambacc = 0.1;

		RAY myRay;
		FVECT rorg = { 0, 0, 0 };
		FVECT rdir = { 0, 0, 1 };


		VCOPY(myRay.rorg, rorg);
		VCOPY(myRay.rdir, rdir);
		myRay.rmax = 0;

		RAYPARAMS rp;
		ray_save(&rp);
		ray_init(&octname[0]);
		ray_trace(&myRay);

		double red = colval(myRay.rcol, RED);
		double green = colval(myRay.rcol, GRN);
		double blue = colval(myRay.rcol, BLU);
		
		std::cout << " Color: (" << red <<"," << green << "," << blue << ")" << std::endl;
		ray_done(1);


		// Return to the original
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