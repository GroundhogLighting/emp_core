
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

#ifndef AVOID_SKP

#include "groundhogmodel/groundhogmodel.h"
#include "common/geometry/polygon.h"
#include "common/geometry/loop.h"

#include <SketchUpAPI/model/vertex.h>
#include <SketchUpAPI/model/attribute_dictionary.h>

#include <vector>


#include "json/json.hpp"
using nlohmann::json;

#define SKP_GROUNDHOG_DICTIONARY "Groundhog"
#define SKP_LABEL "Label"
#define SKP_WINDOW "window"
#define SKP_WORKPLANE "workplane"
#define SKP_ILLUM "illum"
#define SKP_NAME "Name"
#define SKP_VALUE "Value"
#define SKP_WINGROUP "Win_Group"
#define SKP_MATERIAL "rad_material"
#define SKP_VALUE "Value"
#define SKP_PHOTOSENSOR "illuminance_sensor"
#define SKP_SOLVED_WORKPLANE "solved_workplane"
#define SKP_WEATHER "Weather"

#define TO_M(x) x*0.0254
#define TO_M2(x) x*0.00064516
#define TO_DEGREE(x) x*180.0/3.141592654 


//! Object that reads a a SketchUp model and fills a GroundhogModel
/*!
This object has a short life, and it is only meant to parse a SKP file
into a GroundhogModel.

The main function used by the Emp object is the parseSKPModel function.
*/

class SKPReader {
private:
	SUModelRef suModel; //!< The SketchUp model to be read.
	SUStringRef groundhogDictionaryName; //!< The Groundhog Dictionary name (in SketchUp format)
	GroundhogModel * model; //!< The model to be populated
	bool verbose = true; //!< Should we inform progress?

public:

	//! Creates a SKPReader object.
	/*!
	It assigns a SU_INVALID value to the suModel member, creates the SUStringRef
	object with the correct name to find the information and initializes the SketchUp API

	@author German Molina
	@param[in] model The GroundhogModel to populate
	@param[in] verbose Option for inform progress
	*/
	SKPReader(GroundhogModel * model, bool verbose);

	//! Destroys a SKPReader object
	/*!
	Destroys the suModel as well as the groundhogDictionaryName.
	It also terminates the SketchUp API

	@author German Molina
	*/
	~SKPReader();

	//! Check if a returned SUResult has a certain error or not.
	/*!
	This function intends to help verify that the SketchUp API is trated apropriately.

	@author German Molina
	@param[in] res The SUResult to check
	@param[in] functionName The name of the function
	@param[in] ln The line where the function is called
	*/
	bool checkSUResult(SUResult res, std::string functionName, int ln);

	//! Reads the SKP model and fills a GroundhogModel
	/*!
	@author German Molina
	@param[in] inputFile The name of the SKP file to read

	@return success
	*/
	bool parseSKPModel(std::string inputFile);

	//! Retrieves a string from a SUShadowInfoRef object.
	/*!
	@author German Molina
	@param[in] shadowInfo The SUShadowInfoRef object to be read
	@param[in] key The key of the object to read
	@param[out] value The string where the value will be placed
	@return success
	*/
	bool getStringFromShadowInfo(SUShadowInfoRef shadowInfo, const char * key, std::string * value);

	//! Retrieves a double from a SUShadowInfoRef object.
	/*!
	@author German Molina
	@param[in] shadowInfo The SUShadowInfoRef object to be read
	@param[in] key The key of the object to read
	@param[out] value The double where the value will be placed
	@return success
	*/
	bool getDoubleFromShadowInfo(SUShadowInfoRef shadowInfo, const char * key, double * value);

	//! Retrieves a time (epoch) from a SUShadowInfoRef object.
	/*!
	@author German Molina
	@param[in] shadowInfo The SUShadowInfoRef object to be read
	@param[out] value The object where the value will be placed
	@return success
	*/
	bool getTimeFromShadowInfo(SUShadowInfoRef shadowInfo, int64_t * value);

	//! Loads North Correction and Location of the model
	/*!
	Loads the North Correction, Longitude, Latitude, Time Zone,
	City Name, Country Name and Current Date

	@author German Molina
	@return success
	*/
	bool loadModelInfo();

	//! Transforms a SUCameraRef into a View
	/*!
	Assigns the given name to the View, and retrieves the FOV, View Point,
	View Direction, etc. from the SUCameraRef

	@author German Molina
	@param[in] viewName The name of the view to create
	@param[in] suCamera The SUCameraRef object to parse
	@param[out] view The View object to fill
	@return success
	*/
	bool SUCameraToView(std::string * viewName, SUCameraRef suCamera, View * view);

	//! Transforms a SUSceneRef object into a View object
	/*!
	Similar to SKPReader::SUCameraToView(), but the name is obtained
	directly from the SUSceneRef object name

	@author German Molina
	@param[in] suView The SUSceneRef object to be read
	@param[out] view The View object to be filled
	@return success
	*/
	bool SUViewToView(SUSceneRef suView, View * view);

	//! Loads all the SUSceneRef objects into the GroundhogModel object
	/*!
	Loads all the SUSceneRef in the suModel, including the 'current one'
	(i.e. that view that was not saved, but that is the last used in the model)

	Every SUSceneRef will lead to a View with the corresponding name, and the
	'Current View' will be stored as 'view'

	@author German Molina
	@return success
	*/
	bool loadViews();

	//! Loads the information in the SULayerRef objects into the model
	/*!
	Reads the layers, and loads them into the GroundhogModel. All the
	resulting Layer will be empty after this is done. After this function,
	the SKPReader::loadLayersContent() has to be called.

	@author German Molina
	@return success
	@note After this function, the SKPReader::loadLayersContent() has to be called.
	*/
	bool loadLayers();

	//! Retrieves a SUComponentDefinitionRef name
	/*!
	@author German Molina
	@param[in] definition The SUComponentDefinitionRef object to retrieve the name from
	@param[out] name The object to fill with the name
	@return success
	*/
	bool getSUComponentDefinitionName(SUComponentDefinitionRef definition, std::string * name);

	//! Creates a ComponentInstance from a SUComponentInstanceRef and adds it to the destination vector
	/*!
	Creates a ComponentInstance from a SUComponentInstanceRef and adds it to the destination vector.
	The destination is a vector within a ComponentDefinition or a Layer; and the GroundhogModel provided
	should contain the ComponentDefinition that contains the parent (checked by name).

	@author German Molina
	@param[out] dest The ComponentInstance vector to which the instance will be added
	@param[in] suComponentInstance The SUComponentInstanceRef object to add
	@return success
	*/
	bool addComponentInstanceToVector(std::vector <ComponentInstance * > * dest, SUComponentInstanceRef suComponentInstance);

	
	//! Retrieves all the SUFaceRef objects in entities and loads them into a Face vector
	/*!
	@author German Molina
	@param[out] dest The Face vector where all the SUFaceRef in entities will be bulked
	@param[in] entities the SUEntitiesRef object containing the SUFacesRef
	@return success
	*/
	bool bulkFacesIntoVector(std::vector <Otype * > * dest, SUEntitiesRef  entities);

	//! Transforms a SUComponentDefinitionRef into a ComponentDefinition and adds it to a GroundhogModel
	/*!
	@author German Molina
	@param[in] definition The SUComponentDefinitionRef to transform and import to model
	@return success
	*/
	bool loadComponentDefinition(SUComponentDefinitionRef definition);

	//! Loads all the SUComponentDefinitionRef in the suModel into a GroundhogModel
	/*!
	@author German Molina
	@return success
	*/
	bool loadComponentDefinitions();

    //! Loads all the SUGroupDefinitions in the model
    /*!
    @author German Molina
    @return success
    */
    bool loadGroupDefinitions();

    //! Loads the contents of the SUComponentDefinitions in the model
    /*!
    @author German Molina
    @return success
    */
    bool fillComponentDefinitions();

    //! Loads the contents of the SUGroupDefinitions in the model
    /*!
    @author German Molina
    @return success
    */
    bool fillGroupDefinitions();

	//! Loads the contents of the SULayerRef in the suModel
	/*!
	Starts by importing all the SUFaceRef objects (as Face) and continues
	by loading all the SUComponentInstanceRef (as ComponentInstance)

	@author German Molina
	@return success
	*/
	bool loadLayersContent();

	//! Transforms a SUFaceRef into a Polygon3D
	/*!
	Creates a polygon with the same vertex (Point3D) and Loop as the
	SUFaceRef object, except that it cleans them (see Loop::clean()). 
	It also fills the area of the face.

	The SketchUp API is in inches, so everything is transformed into
	meters and squared meters.

	@author German Molina
	@param[in] face The SUFaceRef to transform
	@param[out] polygon The Polygon3D object to fill
	@return success
	*/
	bool SUFaceToPolygon3D(SUFaceRef face, Polygon3D * polygon);

	//! Transforms an SULoopRef into a Loop
	/*!
	The SketchUp API is in inches, so everything is transformed into meters.

	@author German Molina
	@param[in] suLoop The SULoopRef to transform
	@param[out] loop The Loop object to fill
	@return success
	*/
	bool SULoopToLoop(SULoopRef suLoop, Loop * loop);

	//! Retrieves the name of the SUFaceRef object
	/*!
	@author German Molina
	@param[in] face The SUFaceRef to retrieve the name from
	@param[out] name The name to be filled
	@return success
	*/
	bool getSUFaceName(SUFaceRef face, std::string * name);

	//! Retrieves the Layer Name from a SUFaceRef object
	/*!
	@author German Molina
	@param[in] face The SUFaceRef to retrieve the Layer Name from
	@param[out] name The name to be filled
	@return success
	*/
	bool getSUFaceLayerName(SUFaceRef face, std::string * name);

	//! Retrieves the name of the SUDrawingElementRef object
	/*!
	@author German Molina
	@param[in] element The SUDrawingElementRef to retrieve the name from
	@param[out] name The name to be filled
	@return success
	*/
	bool getSUDrawingElementLayerName(SUDrawingElementRef element, std::string * name);

	//! Retrieves the name of the SUEntityRef object
	/*!
	@author German Molina
	@param[in] entity The SUEntityRef to retrieve the name from
	@param[out] name The name to be filled
	@return success
	*/
	bool getSUEntityName(SUEntityRef entity, std::string * name);

	//! Retrieves all the SUComponentInstanceRef objects in entities and loads them into a ComponentInstance vector
	/*!
	Retrieves all the SUComponentInstanceRef objects in entities and loads them into a
	ComponentInstance vector. The GroundhogModel inputed should contain the required
	ComponentDefinitions (which are checked by name).

	@author German Molina
	@param[out] dest The ComponentInstance vector where all the SUComponentInstanceRef in entities will be bulked
	@param[in] entities The SUEntitiesRef object containing the SUComponentInstanceRef
	@return success
	*/
	bool bulkComponentInstancesIntoVector(std::vector <ComponentInstance * > * dest, SUEntitiesRef  entities);

	//! Fills the location (rotation, translation and scale) of a ComponentInstance based on a suInstance
	/*!
	@author German Molina
	@param[out] instance The ComponentInstance to fill with location
	@param[in] suInstance The SUComponentInstanceRef object from which the location is obtained
	@return success
	*/
	bool fillComponentInstanceLocation(ComponentInstance * instance, SUComponentInstanceRef suInstance);

	//! Retrieves the label of a SUFaceRef
	/*!
	Will return TRUE if the face does have a label; false if not.
	
	@author German Molina
	@param[in] face The SUFaceRef to retrieve the label from
	@param[out] name The object to put the name into
	@return false if the face does not have a label, true otherwise
	*/
	bool getSUFaceLabel(SUFaceRef face, std::string * name);

	//! Retrieves the label of a SUEntityRef
	/*!
	Will return TRUE if the entity does have a label; false if not.

	@author German Molina
	@param[in] entity The SUEntityRef to retrieve the label from
	@param[out] label The object to put the name into
	@return false if the entity does not have a label, true otherwise
	@todo Change all labels to numbers.
	*/
	bool getSUEntityLabel(SUEntityRef entity, std::string * label);

	//! Adds a workplane face to a model.
	/*!
	Gets the name of the face and adds it to the model. The model
	automatically creates a workplane if it does not already exist.

	@author German Molina
	@param[in] face The face to treat as a workplane	
	@return success
	*/
	bool addWorkplaneToModel(SUFaceRef face);

	//! Adds a Window to the model.
	/*!
	Gets the name (and Window Group name) of the face and adds it to 
	the model. The model will create a new Window Group if needed.

	@author German Molina
	@param[in] face The face to treat as a workplane
	@return success
	*/
	bool addWindowToModel(SUFaceRef face);

	//! Retrieves an entity ID
	/*!
	@author German Molina
	@param entity The entity whose ID will be retrieved
	@return the ID
	*/
	int32_t getEntityID(SUEntityRef entity);

	//! Retrieves a SUTypedValue from an SUEntity Groundhog's dictionary
	/*!
	Will return SU_ERROR_NONE or SU_ERROR_NO_DATA, depending on the 
	availability of such value.

	@author German Molina
	@return Was any value or not
	@param[in] entity The SUEntityRef
	@param[in] key The key to retrieve from the dictionary
	@param[out] value The SUTypedValue retrieved
	*/
	bool getValueFromEntityGHDictionary(SUEntityRef entity, const char * key, SUTypedValueRef * value);

	//! Transforms a SUStringRef into an ASCII std::string
	/*!
	@author German Molina
	@param[in] suString The original SUSTringRef
	@param[out] string The resulting std::string
	@param[in] fix Should we fixString()?
	@return success
	*/
	bool SUStringtoString(SUStringRef suString, std::string * string, bool fix);

	//! Retrieves a std::string from a SUTypedValue object
	/*!
	@author German Molina
	@param[in] suValue The SUTypedValueRef object
	@param[out] value The returned string
	@param[in] fix Should we fix the string?
	@return success
	*/
	bool getFromSUTypedValue(SUTypedValueRef suValue, std::string * value, bool fix);

	//! Adds a Material to the Groundhogmodel
	/*!
	Will add the material unless another material with the same name exists
	@author German Molina
	@param[in] material The material to parse and add
	@return The pointer to the added material
	*/
	Material * addMaterialToModel(SUMaterialRef material);

	//! Retrieves a String value from a SUEntityRef
	/*!
	@author German Molina
	@param[in] entity The entity to retrieve the value from
	@param[out] value The value
	@param[in] fix Fix the value?
	@return success
	*/
	bool getGHValueFromEntity(SUEntityRef entity, std::string * value, bool fix);

	//! Guesses a material from its SketchUp properties.
	/*!
	@author German Molina
	@param[in] material the SUMaterialRef to guess
	@param[out] j The JSON that represents the guessed material
	@return success
	@note it will warn the user, since guessing materials is definetly not recommended
	*/
	bool guessMaterial(SUMaterialRef material, json * j);

	//! Retrieves the SUFaceRef material
	/*!
	Will prioritize the front RAD material. If it does not exist
	or it is not a RAD material, the back material will be tested. 
	If still no material, the material will be guessed. 

	If no material at all is available, the default material
	will be assigned.
	
	@author German Molina
	@param[in] face The SUFaceRef to retrieve the material from
	@param[out] mat The retrieved SUMaterialRef
	@return true if a material was returned, false if not.
	*/
	bool getFaceMaterial(SUFaceRef face, SUMaterialRef * mat);


	//! Transforms a SUFaceRef into a Face
	/*!
	The Material of the face will be added to the model

	@author German Molina
	@param[in] suFace The SUFaceRef to transform
	@return The pointer to the new Face
	*/
	Face * SUFaceToFace(SUFaceRef suFace);

	//! Retrieves the name of a SUMaterialRef object
	/*!
	@author German Molina
	@return success
	@param[in] material SUMaterialRef to retrieve the name from
	@param[out] name The name of the material
	*/
	bool getSUMaterialName(SUMaterialRef material, std::string * name);

	//! Adds a photosensor to the Model
	/*!
	@author German Molina
	@param[in] definition The SUComponentDefinitionRef representing the Photosensor
	*/
	bool addPhotosensorsToModel(SUComponentDefinitionRef definition);

	//! Loads the weather in the model
	/*!
	@author German Molina
	*/
	bool loadWeather();

	//! Retrieves a value from the SUModelRef dictionary
	/*!
	@author German Molina
	@param[in] key The key to check
	@param[out] value The value retrieved
	*/
	bool getValueFromModelGHDictionary(const char * key, SUTypedValueRef * value);

    //! Loads a SUComponentInstanceRef in the corresponding layer
    /*!
    @author German Molina
    @param[in] instance
    @return success
    */
    bool loadInstance(SUComponentInstanceRef instance);

    
};

#endif
