#include "./location.h"
#include "../../common/utilities/io.h"

Location::Location() {
	DEBUG_MSG("Creating Location");
	city = "not specified";
	country = "not specified";
	latitude = 0;
	longitude = 0;
	timeZone = 0;
}

Location::~Location() {
	DEBUG_MSG("Destroying location");
}

double Location::getLatitude() {
	return latitude;
}
void Location::setLatitude(double l) {
	latitude = l;
}
double Location::getLongitude() {
	return longitude;
}
void Location::setLongitude(double l) {
	longitude = l;
}
double Location::getTimezone() {
	return timeZone;
}
void Location::setTimezone(double t) {
	timeZone = t;
}
std::string Location::getCity() {
	return city;
}
void Location::setCity(std::string c) {
	city = c;
}
std::string Location::getCountry() {
	return country;
}
void Location::setCountry(std::string c) {
	country = c;
}