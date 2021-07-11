#ifndef READDATA_H
#define READDATA_H

#include "classes.h"
#include <string>

class DataValues
{
	public:
		std::string name;
		Date date;
		double value;
		DataValues(std::string, Date, double);
};

int readfile(std::string, std::vector<DataValues>&);

#endif