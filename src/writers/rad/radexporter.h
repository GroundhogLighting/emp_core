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
#include "groundhogmodel/groundhogmodel.h"

#include "common/geometry/transform.h"

//! The main object for exporting a GroundhogModel in Radiance format.
/*!
The file distribution will be the one used by Groundhog (www.groundhoglighting.com).

The main function to be called is RadExporter::exportModel()
*/
class RadExporter {
private:	
	GroundhogModel * model; //!< The GroundhogModel to export
	std::string exportDir; //!< The directory to exporit to
	bool verbose; //!< Should we inform progress?

public:

	//! Creates a RadExporter object
	/*!
	Each input is assigned to the corresponding member

	@author German Molina
	*/
	RadExporter(GroundhogModel * model, std::string exportDir, bool verbose);
	
	//! Destroys a RadExporter object
	/*!
	@author German Molina
	*/
	~RadExporter();

	//! Exports a GroundhogModel in Radiance format.
	/*!
	Will call other functions of the object with the purpose of creating
	directories and writing different files

	@author German Molina
	@return success
	*/
	bool exportModel();
	
	//! Writes the information of the model (north correction and location)
	/*!
	The information is written in the file 'model_info.txt'

	@author German Molina
	@return success
	@param[in] filename The subdirectory to export
	*/
	bool writeModelInfo(const char * filename);
	
	//! Writes all the View objects in Radiance format in different files
	/*!
	A directory is created; and each View is stored in a '$viewName.vf' file

	@author German Molina
	@return success
	@param[in] dir The subdirectory to export
	*/
	bool writeViews(const char * dir);

	//! Writes all the component definitions in different files
	/*!
	A directory is created; and a '$componentName.rad' file
	is created for each ComponentDefinition

	The files will be autocontained; that is, they have all the materials
	and geometry that defines the ComponentDefinition

	@author German Molina
	@return success
	@param[in] dir The subdirectory to export
	*/
	bool writeComponentDefinitions(const char * dir);
	
	//! Writes all the layers in different files
	/*!
	A directory is created; and a 'layerName.rad' file
	is created for each Layer.

	The files will contain the references to ComponentDefinitions (using xform)
	and the geometry. Materials will be stored somewhere else in the directory

	@author German Molina
	@return success
	@param[in] dir The subdirectory to export
	*/
	bool writeLayers(const char * dir);

    //! Writes all the layers in a single file
    /*!

    All the geometry in the Layers will be written in a single 
    file, optionally converting all the material names into the 
    "newMaterial" (this is turn off when newMaterial is a NULL
    pointer).

    @author German Molina
    @return success
    @param[in] file The file
    @param[in] newMaterial The name of the material
    */
    bool writeLayers(FILE * file, const char * newMaterial);

	//! Writes an XFORM call to a ComponentInstance in Radiance format
	/*!
	@author German Molina
	@param[in] file The file to write this in
	@param[in] instance The ComponentInstance to write
	*/
	void writeComponentInstance(FILE * file, ComponentInstance * instance);
	
    //! Writes a ComponentInstance in Radiance format
    /*!
    Instead of writing an XFORM command, it will expand the
    geometry according to a transform. If a "newMaterial" is provided,
    all the material names will be replaced by this

    @author German Molina
    @param[in] file The file to write this in
    @param[in] instance The ComponentInstance to write
    @param[in] transform A transformation to apply to the instance geometry
    @param[in] A material name that overrides the actual materials of the geometry (i.e. xform -m 'newMaterial' option)
    */
    void writeComponentInstance(FILE * file, ComponentInstance * instance, Transform * transform, const char * newMaterial);


	//! Writes all the window groups in Radiance format
	/*!
	@author German Molina
	@return success
	@param[in] dir The subdirectory to export
	*/
	bool writeWindows(const char * dir);

    //! Writes all the windows in a single file
    /*!
    @author German Molina
    @return success
    @param[in] file the file
    */
    bool writeWindows(FILE * file);

	//! Writes all the Material objects in Radiance format
	/*!
	@author German Molina
	@return success
	@param[in] dir The subdirectory to export
	*/
	bool writeMaterials(const char * dir);

    //! Writes all the Material objects in a single FILE * object
    /*!
    @author German Molina
    @return success
    @param[in] file The FILE object to write to
    */
    bool writeMaterials(FILE * file);

	//! Writes the standard Clear Sky
	/*!
	@author German Molina
	@param[in] file The File to write the sky to
	@return [Boolean] Success
	*/
	bool writeSky(FILE * file);

    //! Writes the standard Clear Sky
    /*!
    @author German Molina
    @param[in] dir The directory to write to
    @return [Boolean] Success
    */
    bool writeSky(const char * dir);

	//! Writes the scene file
	/*!
	@author German Molina
	@return [Boolean] Success
	@param[in] dir The directory to export
	@todo There is a bug that would cause an error if the exporting Directory of Layers changes.
	*/
	bool writeSceneFile(const char * dir);

	//! Writes the Photosensors
	/*!
	@author German Molina
	@param[in] dir The directory to export
	@return [Boolean] Success
	*/
	bool writePhotosensors(const char * dir);

	//! Writes the weather file
	/*!
	@author German Molina
	@param[in] dir The directory
	@return sucess
	*/
	bool writeWeather(const char * dir);
};
