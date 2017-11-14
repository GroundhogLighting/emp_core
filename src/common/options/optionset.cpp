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

#include "./optionset.h"
#include "common/utilities/io.h"

void OptionSet::addOption(std::string opt, double defValue)
{
	names.push_back(opt);
	values.push_back(defValue);
}

bool OptionSet::setOption(std::string opt, double v)
{
	size_t n = names.size();
	for (size_t i = 0; i < n; i++) {
		if (names[i] == opt) {
			values[i] = v;
			return true;
		}
	}
	fatal("Option '"+opt+"' not found", __LINE__, __FILE__);
	return false;
}


double OptionSet::getOption(std::string opt)
{
	size_t n = names.size();
	for (size_t i = 0; i < n; i++) {
		if (names[i] == opt) {
			return values[i];			
		}
	}
	fatal("Option '" + opt + "' not found", __LINE__, __FILE__);
	return NULL;
}


double OptionSet::getOption(size_t i)
{
	size_t n = names.size();
	if (i < 0 || i >= n) {
		fatal("Option set has less than " + std::to_string(i) + " options... impossible to retrieve such value", __LINE__, __FILE__);
		return NULL;
	}
	return values[i];
}

std::string OptionSet::getOptionName(size_t i)
{
	size_t n = names.size();
	if (i < 0 || i >= n) {
		fatal("Option set has less than " + std::to_string(i) + " options... impossible to retrieve such name", __LINE__, __FILE__);
		return NULL;
	}
	return names[i];
}

size_t OptionSet::countOptions()
{
	return names.size();
}

void OptionSet::print(std::string flnm)
{
	size_t n = names.size();
	for (size_t i = 0; i < n; i++) {
		std::cout << "-" << names[i] << " " << values[i] << std::endl;
	}
}