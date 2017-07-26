#pragma once

class Date{
private:
    int month;
    int day;
    int hour;
    int minute;

public:
	Date(int month, int day, int hour, int min);
	Date(int64_t t);

	~Date();

	void setMonth(int m);
	void setDay(int d);
	void setHour(int h);
	void setMinute(int min);

	int getMonth();
	int getDay();
	int getHour();
	int getMinute();
};