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

#include "common/options/optionset.h"

#define OCONV_INCLUDE_WINDOWS "include_windows"
#define OCONV_USE_BLACK_GEOMETRY "black_geometry"
#define OCONV_LIGHTS_ON "lights_on"


class OconvOptions : public OptionSet {
public:
  //! Constructor
  /*!
  Creates an OptionSet and fills the corresponding
  options and default values

  @author German Molina
  */
  OconvOptions()
  {
    addOption(OCONV_INCLUDE_WINDOWS, true);
    addOption(OCONV_USE_BLACK_GEOMETRY, false);
    addOption(OCONV_LIGHTS_ON, false);
  };
};
