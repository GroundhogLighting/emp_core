#include "./io.h"
#include "./date.h"

#include <ctime>

Date::Date(int m, int d, int h, int min) {
	month = m;
	day = d;
	hour = h;
	minute = min;
}


Date::Date(int64_t t) {
	tm * time = gmtime(&t);
	month = (time->tm_mon) + 1;
	day = time->tm_mday;
	hour = time->tm_hour;
	minute = time->tm_min;
	delete time;

}

Date::~Date() {
	DEBUG_MSG("Destroying date");
}

void Date::setMonth(int m) {
	month = m;
}
void Date::setDay(int d) {
	day = d;
}
void Date::setHour(int h) {
	hour = h;
}
void Date::setMinute(int min) {
	minute = min;
}

int Date::getMonth() {
	return month;
}
int Date::getDay() {
	return day;
}
int Date::getHour() {
	return hour;
}

int Date::getMinute() {
	return minute;
}