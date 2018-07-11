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

#ifndef RAD_EXPORTER_H
#define RAD_EXPORTER_H

#include "../../groundhogmodel/groundhogmodel.h"

#include "../../common/geometry/transform.h"

//! The main object for exporting a GroundhogModel in Radiance format.
/*!
The file distribution will be the one used by Groundhog (www.groundhoglighting.com).

The main function to be called is RadExporter::exportModel()
*/
class RadExporter {
private:	
	GroundhogModel * model; //!< The GroundhogModel to export

public:

	//! Creates a RadExporter object
	/*!
	Each input is assigned to the corresponding member

	@author German Molina
	*/
	RadExporter(GroundhogModel * model);
	
	
	//! Writes the information of the model (north correction and location)
	/*!
	The information is written in the file 'model_info.txt'

	@author German Molina
	@return success
	@param[in] filename The subdirectory to export
	*/
	bool writeModelInfo(const char * filename) const;
	
	//! Writes all the View objects in Radiance format in different files
	/*!
	A directory is created; and each View is stored in a '$viewName.vf' file

	@author German Molina
	@return success
	@param[in] dir The subdirectory to export
	*/
	bool writeViews(const char * dir) const;

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
	bool writeComponentDefinitions(const char * dir) const;
	
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
	bool writeLayers(const char * dir) const;

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
    bool writeLayersInOneFile(FILE * file, std::string * newMaterial) const;

    //! Writes all the layers in a single file
    /*!
     
     All the geometry in the Layers will be written in a single
     file.
     
     @author German Molina
     @return success
     @param[in] file The file
     @param[in] newMaterial The name of the material
     */
    bool writeLayersInOneFile(FILE * file) const;

    
	//! Writes an XFORM call to a ComponentInstance in Radiance format
	/*!
	@author German Molina
	@param[in] file The file to write this in
	@param[in] instance The ComponentInstance to write
	*/
	void writeComponentInstance(FILE * file, const ComponentInstance * const instance) const;
	
    //! Writes a ComponentInstance in Radiance format
    /*!
    Instead of writing an XFORM command, it will expand the
    geometry according to a transform. If a "newMaterial" is provided,
    all the material names will be replaced by this

    @author German Molina
    @param[in] file The file to write this in
    @param[in] instance The ComponentInstance to write
    @param[in] transform A transformation to apply to the instance geometry
    @param[in] scale The scale to apply to the instance
    @param[in] A material name that overrides the actual materials of the geometry (i.e. xform -m 'newMaterial' option)
    */
    void writeComponentInstance(FILE * file, const ComponentInstance * const instance, Transform * transform, double scale, std::string * newMaterial) const;


	//! Writes all the window groups in Radiance format
	/*!
	@author German Molina
	@return success
	@param[in] dir The subdirectory to export
	*/
	bool writeWindows(const char * dir) const;

    //! Writes all the windows in a single file
    /*!
    @author German Molina
    @return success
    @param[in] file the file
    */
    bool writeWindows(FILE * file) const;

	//! Writes all the Material objects in Radiance format
	/*!
	@author German Molina
	@return success
	@param[in] dir The subdirectory to export
     @param[in] filename The name of the file that references all materials
	*/
	bool writeMaterials(const char * dir, const std::string filename) const;

    //! Writes all the Material objects in a single FILE * object
    /*!
    @author German Molina
    @return success
    @param[in] file The FILE object to write to
    */
    bool writeMaterials(FILE * file) const;

	//! Writes the standard Clear Sky
	/*!
	@author German Molina
	@param[in] file The File to write the sky to
	@return [Boolean] Success
	*/
	bool writeSky(FILE * file) const;

    //! Writes the standard Clear Sky
    /*!
    @author German Molina
    @param[in] dir The directory to write to
     @param[in] filename The name of the file to write
    @return [Boolean] Success
    */
    bool writeSky(const char * dir, const std::string filename) const;

	//! Writes the scene file
	/*!
	@author German Molina
	@return [Boolean] Success
	@param[in] dir The directory to export
	@todo There is a bug that would cause an error if the exporting Directory of Layers and Views changes.
	*/
	bool writeSceneFile(const char * dir, OptionSet * options) const;

    //! Writes the rif file
    /*!
     @author German Molina
     @return [Boolean] Success
     @param[in] dir The directory to export
     @todo There is a bug that would cause an error if the exporting Directory of Layers changes.
     @todo Get the bounding box of the model
     */
    bool writeRifFile(const char * dir, OptionSet * options) const;

    
	//! Writes the Photosensors
	/*!
	@author German Molina
	@param[in] dir The directory to export
	@return [Boolean] Success
	*/
	bool writePhotosensors(const char * dir) const;

	//! Writes the weather file
	/*!
	@author German Molina
	@param[in] dir The directory
     @param[in] filename The name of the file to write
	@return sucess
	*/
    bool writeWeather(const char * dir, const std::string filename) const;
    
    //! Writes an Object in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeOtype(const Otype * const object, FILE * file, const std::string * const material, Transform * transform, double scale) const;
    
    //! Writes an Object in Radiande Format
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @return success
     */
    bool writeOtype(const Otype * const object, FILE * file) const;
    
    //! Writes an Object in Radiance Format, potentially changing the material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param material An optional material name that will override the one assigned to the object
     @return success
     */
    bool writeOtype(const Otype * const object, FILE * file, std::string * material) const;

    //! Writes an Bubble in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param name The name of the object
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeBubble(const Bubble * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes an Cone in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param name The name of the object
     @param file The file to write the object in
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeCone(const Cone * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes an Cup in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param name The name of the object
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeCup(const Cup * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes an Cylinder in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param name The name of the object
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeCylinder(const Cylinder * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes an Face in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param name The name of the object
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeFace(const Face * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes an Ring in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param name The name of the object
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeRing(const Ring * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes an Source in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param name The name of the object
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeSource(const Source * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes an Sphere in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param name The name of the object
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeSphere(const Sphere * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes an Tube in Radiance Format, potentially applying a transformation, scale and changing material
    /*!
     @author German Molina
     @param object The Object to write in Radiance format
     @param file The file to write the object in
     @param name The name of the object
     @param material An optional material name that will override the one assigned to the object
     @param transform An optional transformation to apply to the object before writing it
     @param scale The scale to apply to the object before writing it. Ignored if transform is NULL
     @return success
     */
    bool writeTube(const Tube * const object, const std::string * name, FILE * file, const char * material, Transform * transform, double scale) const;

    //! Writes a material in a File, in Radiance format
    /*!
    @author German Molina
    @param material The material to write
    @param file The file to write the material to
    @return success
    */
    bool writeMaterial(const Material * const material, FILE * file) const;
};

extern RadExporter radexporter;
#endif
