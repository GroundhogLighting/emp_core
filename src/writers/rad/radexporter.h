#pragma once

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
	*/
	void writeModelInfo();
	
	//! Writes all the views in Radiance format in different files
	/*!
	A './Views' directory is created; and each View is stored in a '$viewName.vf' file

	@author German Molina
	@return success
	*/
	bool writeViews();

	//! Writes all the component definitions in different files
	/*!
	A './Components' directory is created; and a '$componentName.rad' file
	is created for each ComponentDefinition

	The files will be autocontained; that is, they have all the materials
	and geometry that defines the ComponentDefinition

	@author German Molina
	@return success
	*/
	bool writeComponentDefinitions();
	
	//! Writes all the layers in different files
	/*!
	A './Geometry' directory is created; and a 'layerName.rad' file
	is created for each Layer.

	The files will contain the references to ComponentDefinitions (using xform)
	and the geometry. Materials will be stored somewhere else in the directory

	@author German Molina
	@return success
	*/
	bool writeLayers();

	//! Writes a ComponentInstance in Radiance format
	/*!
	@author German Molina
	@param[in] file The file to write this in
	@param instance The ComponentInstance to write
	*/
	void writeComponentInstance(std::ofstream * file, ComponentInstance * instance);
	
	//! Writes a Loop in Radiance format
	/*!
	@author German Molina
	@param[in] file The file to write this in
	@param loop The Loop to write
	*/
	void writeLoop(std::ofstream * file, Loop * loop);
	
	//! Writes a Face in Radiance format
	/*!
	This is used when the Face does not Face::hasTooManyLoops

	@author German Molina
	@param[in] file The file to write this in
	@param face The Face to write
	*/
	void writeClosedFace(std::ofstream * file, Face * face);

	//! Writes a Face in Radiance format
	/*!
	@author German Molina
	@param[in] file The file to write this in
	@param face The Face to write
	*/
	void writeFace(std::ofstream * file, Face * face);

};