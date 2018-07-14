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


#ifndef GROUNDHOG_MODEL_H
#define GROUNDHOG_MODEL_H

#include <vector>

#include "./src/windowgroup.h"
#include "./src/illumgroup.h"
//#include "./src/layer.h"
#include "./src/componentinstance.h"
#include "./src/workplane.h"
#include "./src/componentdefinition.h"
#include "./src/view.h"
#include "../common/utilities/date.h"
#include "./src/location.h"
#include "./src/photosensor.h"
#include "./src/rtraceoptions.h"

// Add materials
#include "./src/materials/dielectric.h"
#include "./src/materials/glass.h"
#include "./src/materials/glow.h"
#include "./src/materials/interface.h"
#include "./src/materials/light.h"
#include "./src/materials/metal.h"
#include "./src/materials/plastic.h"
#include "./src/materials/spotlight.h"
#include "./src/materials/trans.h"

// Add Otypes
#include "./src/otypes/bubble.h"
#include "./src/otypes/cone.h"
#include "./src/otypes/cup.h"
#include "./src/otypes/cylinder.h"
#include "./src/otypes/face.h"
#include "./src/otypes/ring.h"
#include "./src/otypes/source.h"
#include "./src/otypes/sphere.h"
#include "./src/otypes/tube.h"

#include "../taskmanager/task.h"

#include "json/json.hpp"
using nlohmann::json;

//! The main class, that contains all the Layer, Workplane and much more

/*!
The GroundhogModel contains all the information representing Geometry, Materials,
Components, Weather, Luminaires, and more (see its private members for more info).
*/

class GroundhogModel {

private:	
	
    std::vector <json> tasks = std::vector<json>(); //!< Contains all the tasks in the model, in JSON format
	std::vector <Layer *> layers = std::vector<Layer *>(); //!< Contains all the geometry that represents physical objects
	std::vector <Workplane *> workplanes = std::vector<Workplane *>(); //!< Contains the Workplane objects in the model
	std::vector <Photosensor *> photosensors = std::vector<Photosensor *>(); //!< Contains the Photosensor objects in the model
	std::vector <Material *> materials = std::vector<Material *>(); //!< Contains all the Materials in the model
	std::vector <View *> views = std::vector<View *>(); //!< Contains all the views that are saved in the model
	std::vector <ComponentDefinition *> definitions = std::vector<ComponentDefinition *>(); //!< Contains all the Component Definitions in the model
	std::vector <WindowGroup *> windowGroups = std::vector<WindowGroup *>(); //!< Contains the window group
    std::vector <IllumGroup *> illumGroups = std::vector<IllumGroup *>(); //!< Contains the window group
	//Luminaires
	Location location = Location(); //!< The location (i.e. longitude, latitude, timezone, etc.)
	Date date = Date(1, 1, 12, 12); //!< The current date
	RTraceOptions rtraceOptions = RTraceOptions(); //< The options related to Ray Tracing (RTRACE program)
	//Observers // **	
	double northCorrection; //!< The north correction (i.e. the model should be rotated when calculating)

public:

	//! Creates a GroundhogModel object
	/*!
	The object is absolutely empty, with a default date and location.

	@author German Molina
	*/
	GroundhogModel();

	//! Destroys the GroundhogModel
	/*!
	Deletes all the members inside of it

	@author German Molina
	*/
	~GroundhogModel();

	//! Adds a new empty Layer to the model
	/*!
	@author German Molina
	@param[in] layerName The layer name
	@todo Check if the layer already exists
	*/
	void addLayer(std::string * layerName);

	//! Adds an Otype to a certain layer
	/*!
	Will return false (and inform a fatal error) if the layer 
	was not found.

	@author German Molina
	@param[in] layerName The name of the layer
	@param[in] o The Otype to add
	@return success
	*/
	bool addObjectToLayer(std::string * layerName, Otype * o);

	
	//! Counts the Layer in the model
	/*!	
	@author German Molina
	@return the number of Layer
	*/
	size_t getNumLayers();

	//! Counts the ComponentDefinition in the model
	/*!
	@author German Molina
	@return the number of ComponentDefinition
	*/
	size_t getNumComponentDefinitions();

	//! Retrieves the reference to a Layer in the model
	/*!
	@author German Molina
	@param[in] i the index of the Layer to retrieve
	@return the Layer pointer
	*/
	Layer * getLayerRef(size_t i);

	//! Retrieves the reference to a ComponentDefinition in the model
	/*!
	@author German Molina
	@param[in] i the index of the ComponentDefinition to retrieve
	@return the ComponentDefinition pointer
	*/
	ComponentDefinition * getComponentDefinitionRef(size_t i);


	//! Adds a new ComponentDefinition to the model
	/*!
	@author German Molina
	@param[in] name The name of the new component definition
	@todo Check if a ComponentDefinition with the same name already exists
	*/
	void addComponentDefinition(std::string * name);

	//! Retrieves a ComponentDefinition from the model by name
	/*!
	Will return NULL if not found

	@author German Molina
	@param[in] definitionName The name of the definition
	@return the reference to the ComponentDefinition
	*/
	ComponentDefinition *  getComponentDefinitionByName(std::string * definitionName);
	
	//! Retrieves a Layer from the model by name
	/*!
	Will return NULL if not found

	@author German Molina
	@param[in] layerName The name of the definition
     @return the reference to the Layer; nullptr if not found
	*/
	Layer *  getLayerByName(std::string * layerName);

	//! Adds a new View to the model
	/*!
	@author German Molina
	@param[in] view The reference to the View add
	*/
	void addView(View * view);

	//! Retrieves the reference to a View in the model
	/*!
	@author German Molina
	@param[in] i the index of the View to retrieve
	@return the View pointer
	*/
	View * getViewRef(size_t i);

	//! Retrieves the reference to a View in the model
	/*!
	@author German Molina
	@param[in] name The name of the View we are looking for
	@return the View pointer
	*/
	View * getViewByName(std::string * name);


	//! Counts the number of view in the model
	/*!
	@author German Molina
	@return the number of views
	*/
	size_t getNumViews();

	//! Sets the North Correction
	/*!
	@author German Molina
	@param[in] nC the North Correction
	*/
	void setNorthCorrection(double nC);

	//! Retrieves the north correction
	/*!
	@author German Molina
	@return The north correction
	*/
	double getNorthCorrection();


	//! Adds a new polygon to a certain workplane
	/*!
	Searches for the corresponding workplane, and adds the polygon
	at the end of it. If it does not exist, it will create one with 
	the provided name

	@author German Molina
	@param[in] workplaneName The name of the workplane
	@param[in] polygon The Polygon3D to add
	*/
	void addPolygonToWorkplane(std::string * workplaneName, Polygon3D * polygon);

	//! Adds a new window to a certain Window Group
	/*!
	Searches for the corresponding window group, and adds the Faces
	at the end of it. If it does not exist, it will create one with
	the provided name

	@author German Molina
	@param[in] windowGroupName The name of the window group
	@param[in] face The Face to add
	*/
	void addWindowToGroup(std::string * windowGroupName, Face * face);
    
    //! Adds a new polygon to a certain Illum Group
    /*!
     Searches for the corresponding illum group, and adds the polygons
     at the end of it. If it does not exist, it will create one with
     the provided name
     
     @author German Molina
     @param[in] illumGroupName The name of the illum group
     @param[in] polygon The Polygon3D to add
     */
    void addIllumToGroup(std::string * illumGroupName, Polygon3D * polygon);


	//! Retrieves the number of window groups
	/*!
	@author German molina
	@return The number of window groups
	*/
	size_t getNumWindowGroups();
    
    //! Retrieves the number of illum groups
    /*!
     @author German molina
     @return The number of window groups
     */
    size_t getNumIllumGroups();

	//! Retrieves the number of workplanes
	/*!
	@author German Molina
	@return The number of workplanes
	*/
	size_t getNumWorkplanes();

	//! Retrieves the reference to a certain WindowGroup
	/*!
	@author German Molina
	@param[in] i The index of the WindowGroup
	@return The reference to the WindowGroup
	*/
	WindowGroup * getWindowGroupRef(size_t i);

    //! Retrieves the reference to a certain IllumGroup
    /*!
     @author German Molina
     @param[in] i The index of the IllumGroup
     @return The reference to the IllumGroup
     */
    IllumGroup * getIllumGroupRef(size_t i);

	//! Retrieves the reference to a certain Workplane
	/*!
	@author German Molina
	@param[in] i The index of the Workplane
	@return The reference to the Workplane
	*/
	Workplane * getWorkplaneRef(size_t i);


	//! Retrieves a workplane by name
	/*!
	@author German Molina
	@param[in] wp The name of the workplane
	@return The found workplane or NULL (if not found)
	*/
    Workplane * getWorkplaneByName(const std::string * const wp) const;

	//! Adds a Material to the Groundhogmodel
	/*!
	@author German Molina
	@param[in] j The json that represents a material
	@return The pointer to the new Material
	*/
	Material * addMaterial(json * j);

	//! Adds a Material to the Groundhogmodel
	/*!
	@author German Molina
	@param[in] m A pointer to a Material
	@return The pointer to the new Material
	*/
	void addMaterial(Material * m);

	//! Add the default material
	/*!
	@author German Molina
	@return the pointer to the Default Material
	*/
	Material * addDefaultMaterial();

	//! Add the default Glass (default Material for windows)
	/*!
	@author German Molina
	@return the pointer to the Default Gaterial
	*/
	Material * addDefaultGlass();

	//! Retrieves the number of Material objects
	/*!
	@author German Molina
	@return the number of Material
	*/
	size_t getNumMaterials();

	//! Retrieves the the pointer to a Material 
	/*!
	@author German Molina
	@param[in] i The index of the Material
	@return the pointer to the Material
	*/
	Material * getMaterialRef(size_t i);

    //! Retrieves a Layer from the model by name
    /*!
     Will return NULL if not found
     
     @author German Molina
     @param[in] materialName The name of the definition
     @return the reference to the Layer; nullptr if not found
     */
    Material *  getMaterialByName(std::string * materialName);

    
	//! Adds a Photosensor to the GroundhogModel
	/*!
	@author German Molina
	@param[in] p The Photosensor
	*/
	void addPhotosensor(Photosensor * p);

	//! Returns the number of Photosensor objects in the model
	/*!
	@author German Molina
	@return the number of Photosensor
	*/
	size_t countPhotosensors();

	//! Retrieves a pointer to a certain Photosensor in the model
	/*!
	@author German Molina
	@param[in] i The index of the Photosensor
	@return The pointer to the Photosensor
	*/
	Photosensor * getPhotosensorRef(size_t i);


	//! Retrieves the Location object
	/*!
	@author German Molina
	@return Pointer to the Location
	*/
	Location * getLocation();

	//! Retrieves the Date object
	/*!
	@author German Molina
	*/
	Date * getDate();

	//! Retrieves the RTraceOptions by pointer
	/*!
	@return The pointer to the RTraceOptions object
	*/
	RTraceOptions * getRTraceOptions();

    //! Checks if a material with a certain name already exists
    /*!
    @author German Molina
    @param matName The name of the material
    @return NULL if does not exist, the material if it exist
    */
    Material * hasMaterial(std::string * matName);
    
    //! Retrieves an Otype object from the model by name
    /*!
     Will return NULL if not found
     
     @author German Molina
     @param[in] objectName The name of the definition
     @return the reference to the Layer; nullptr if not found
     */
    const Otype *  const getOtypeByName(std::string * objectName) const;

    //! Adds a Task object to the model
    /*!
     
     @author German Molina
     @param[in] task The task to add
     */
    void  addTask(json task);
    
    //! Gets the number of tasks in the model
    /*!
     @author German Molina
     @return The number of tasks
     */
    size_t countTasks() const;
    
    //! Retrieves a specific task in the model
    /*!
     @author German Molina
     @param i The index of the task to retrieve
     @return The pointer to the task
     */
    const json * getTask(size_t i) const;
    
    //! Retrieves a specific task in the model
    /*!
     @author German Molina
     @param i The index of the task to retrieve
     @return The pointer to the task
     */
    const json * getTask(std::string name) const;

    

};

extern GroundhogModel GHModel;

#endif
