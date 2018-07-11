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
class Layer;
#include "./layer.h"


//class ComponentDefinition;

//! Groups of Face (or other geometry) meant to be placed several times in a GroudhogModel

/*!
They are referenced within other ComponentInstance or Layers by mean of ComponentInstance
*/
class ComponentDefinition : public Layer {


public:

	//! Creates a ComponentDefinition object
	/*!

	Assigns the name. All other things are empty

	@author German Molina
	@param[in] componentName The name of the ComponentDefinition
    @todo allow windows, sensors and workplanes to be within ComponentDefinitions
	*/
    ComponentDefinition(std::string * componentName) : Layer(componentName)
    {
        
    }    
    
};
