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

/*!
@file config_constants.h
*/

/*!
@defgroup configConstants Configuration constants

This module defines constants that are used in the rest of the program.
*/

/* @{ */

/// The version of the program
#define GLARE_VERSION "0.0.0" //!< The version of the program

/// The separator used when writing files
#define GLARE_TAB "\t" //!< This is the separator used when writing Radiance files

/// Maximum interior loops
#define GLARE_TOO_MANY_LOOPS 40 //!< The number of interior loops that are considered too many in a face 

/// Huge number
#define GLARE_HUGE 9e9 //!< This is a huge number that may be used by several sections of the program

/// Minus huge number
#define GLARE_MINUS_HUGE -GLARE_HUGE //!< This is a negative huge number that may be used by several sections of the program

/// Tiny number
#define GLARE_TINY 1e-9 //!< This is a very small number that may be used by several sections of the program

/// Minus tiny number
#define GLARE_MINUS_TINY -GLARE_TINY //!< This is a very small number that may be used by several sections of the program

/// Maximum number of points in a triangulation before refining
#define GLARE_MAX_POINTS_IN_WORKPLANE 10000 //!< This number is required on the poly2tri function

/// Subfolder to export Layer objects
#define GLARE_LAYERS_SUBFOLDER "Geometry" //!< The default subfolder where the Layer objects will be exported

/// Subfolder to export ComponentDefinition objects
#define GLARE_COMPONENTS_SUBFOLDER "Components" //!< The default subfolder where the ComponentInstance objects will be exported

/// Subfolder to export View objects
#define GLARE_VIEWS_SUBFOLDER "Views" //!< The default subfolder where the View objects will be exported

/// Subfolder to export ComponentDefinition objects
#define GLARE_WINDOWS_SUBFOLDER "Windows" //!< The default subfolder where the WindowGroup objects will be exported

/// Subfolder to export Workplane objects
#define GLARE_WORKPLANES_SUBFOLDER "Workplanes" //!< The default subfolder where the Workplane objects will be exported

/// Subfolder to export Material objects
#define GLARE_MATERIALS_SUBFOLDER "Materials" //!< The default subfolder where the Workplane objects will be exported

/// Subfolder to export Photosensor objects
#define GLARE_PHOTOSENSORS_SUBFOLDER "Photosensors" //!< The default subfolder where the Photosensor objects will be exported

/// Subfolder to export Skies
#define GLARE_SKY_SUBFOLDER "Skies" //!< The default subfolder where the Sky will be exported

/// Name of the file with the model info
#define GLARE_MODEL_INFO_FILE "model_info.txt" //!< The default file where the model information will be exported

/// Name of the file with the whole scene is stored
#define GLARE_SCENE_FILE "scene.rad" //!< The default file where the "scene file" file objects will be exported


/* @} */