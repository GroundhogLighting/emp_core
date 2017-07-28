#pragma once

#include <vector>


#include "./src/material.h"
#include "./src/layer.h"
#include "./src/componentdefinition.h"
#include "./src/view.h"
#include "../common/utilities/date.h"
#include "./src/location.h"

//! The main class, that contains all the Layer, Workplane and much more

/*!
The GroundhogModel contains all the information representing Geometry, Materials,
Components, Weather, Luminaires, and more (see its private members for more info).
*/

class GroundhogModel {

private:
	//Objectives // **
	std::vector <Layer *> layers; //!< Containins all the geometry that represents physical objects
	//Workplanes // **
	//Photosensors // **
	//std::vector <Material *> materials;
	std::vector <View *> views; //!< Contains all the views that are saved in the model
	std::vector <ComponentDefinition *> definitions; //!< Contains all the Component Definitions in the model
	//WindowGroups
	//Weather
	//Luminaires
	Location * location; //!< The location (i.e. longitude, latitude, timezone, etc.)

	Date * date; //!< The current date
	//Options
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

	//! Adds a face to a certain layer
	/*!
	Will return false (and inform a fatal error) if the layer 
	was not found.

	@author German Molina
	@param[in] layerName The name of the layer
	@param[in] face The face to add
	@return success
	*/
	bool addFaceToLayer(std::string * layerName, Face * face);

	//! Adds a ComponentInstance to a certain layer
	/*!
	Will return false (and inform a fatal error) if the layer
	was not found.

	@author German Molina
	@param[in] layerName The name of the layer
	@param[in] instance The ComponentInstance to add
	@return success
	*/
	bool addComponentInstanceToLayer(std::string * layerName, ComponentInstance * instance);

	//! Adds a Face to a vector
	/*!
	@author German Molina
	@param[in] faces The vector of Face
	@param[in] face The Face to add
	*/
	void addFaceToVector(std::vector <Face *> * faces, Face * face);

	//! Adds a ComponentInstance to a vector
	/*!
	@author German Molina
	@param[in] instances The vector of CompoentInstance
	@param[in] instance The instance to add
	*/
	void addComponentInstanceToVector(std::vector <ComponentInstance *> * instances, ComponentInstance * instance);

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
	@param[in] componentDefinition The reference to the ComponentDefinition to add
	@todo Check if a ComponentDefinition with the same name already exists
	*/
	void addComponentDefinition(ComponentDefinition * componentDefinition);

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
	@return the reference to the Layer
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

	//! Sets the Latitude of the model Location
	/*!
	@author German Molina
	@param[in] l the latitude
	*/
	void setLatitude(double l);

	//! Sets the Longitude of the model Location
	/*!
	@author German Molina
	@param[in] l the longitude
	*/
	void setLongitude(double l);

	//! Sets the Time Zone of the model Location
	/*!
	@author German Molina
	@param[in] t the time zone
	*/
	void setTimeZone(double t);

	//! Sets the City Name of the model Location
	/*!
	@author German Molina
	@param[in] c the name of the city
	*/
	void setCity(std::string c);

	//! Retrieves the City Name of the model Location
	/*!
	@author German Molina
	@return The name of the city
	*/
	std::string getCity();

	//! Sets the Country Name of the model Location
	/*!
	@author German Molina
	@param[in] c the name of the country
	*/
	void setCountry(std::string c);

	//! Retrieves the Country Name of the model Location
	/*!
	@author German Molina
	@return The name of the Country
	*/
	std::string getCountry();

	//! Sets the Month of the model Date
	/*!
	@author German Molina
	@param[in] m The month
	*/
	void setMonth(int m);

	//! Sets the Day of the model Date
	/*!
	@author German Molina
	@param[in] d The day
	*/
	void setDay(int d);

	//! Sets the Hour of the model Date
	/*!
	@author German Molina
	@param[in] h The hour
	*/
	void setHour(int h);

	//! Sets the Minute of the model Date
	/*!
	@author German Molina
	@param[in] min The minute
	*/
	void setMinute(int min);

	//! Retrieves the Latitude of the model Location
	/*!
	@author German Molina
	@return The latitude
	*/
	double getLatitude();

	//! Retrieves the Longitude of the model Location
	/*!
	@author German Molina
	@return The longitude
	*/
	double getLongitude();

	//! Retrieves the Time Zone of the model Location
	/*!
	@author German Molina
	@return The time zone
	*/
	double getTimeZone();

	//! Retrieves the Month of the model Date
	/*!
	@author German Molina
	@return The month
	*/
	int getMonth();

	//! Retrieves the Day of the model Date
	/*!
	@author German Molina
	@return The day
	*/
	int getDay();

	//! Retrieves the Hour of the model Date
	/*!
	@author German Molina
	@return The hour
	*/
	int getHour();

	//! Retrieves the Minute of the model Date
	/*!
	@author German Molina
	@return The minute
	*/
	int getMinute();

};