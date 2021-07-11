#include <fstream>
#include <vector>
#include <iostream>
#include <map>
#include <regex>
#include "readdata.h"
#include "classes.h"

DataValues::DataValues(std::string name, Date date, double value) : name(name), date(date), value(value)
{}

int readfile(std::string path, std::vector<DataValues>& data)
{
	std::string readString;
	Date lastDate(0,0,0);
	
	std::ifstream datafile(path);
	if(datafile.is_open())
	{
		while(datafile.good())
		{
			getline(datafile, readString);
			std::smatch match;
			std::regex rgx("[0-9\\.]+");
			if(!std::regex_match(readString, std::regex("#.+")))
			{
				if(std::regex_match(readString, std::regex("[0-9]+/[0-9]+/[0-9]+")))
				{
					std::smatch datematch;
					std::regex datergx("[0-9]+");
					int nums[3];
					std::string dateString = readString;
					for(int i = 0; i < 3; i++)
					{
						if (std::regex_search(dateString, datematch, datergx))
						{
							nums[i] = std::stod(datematch[0]);
							dateString.erase(0,datematch.length(0)+1);
						} else
						{
							std::cout << "\n\n\nINVALID DATE FORMAT\n\n\n";
							break;
						}
					}
					Date date(nums[0],nums[1],nums[2]);
					lastDate = date;
				} else if (std::regex_search(readString, match, rgx))
				{
					std::string name = readString;
					name.erase(readString.find(match[0]),match.length(0));
					DataValues datatemp(name, lastDate, std::stod(match[0]));
					data.push_back(datatemp);	
				}
			}
		}
		datafile.close();
	}
	
	return 0;
}