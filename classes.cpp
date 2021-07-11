#include "classes.h"

Date::Date(int year, int month, int day) : year(year), month(month), day(day)
{}

int Date::ToDays()
{
	return ((this->year*365.25)+(this->month*30.436875)+(this->day));
}