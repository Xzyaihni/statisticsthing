#ifndef CLASSES_H
#define CLASSES_H

class Date
{
	public:
		int year;
		int month;
		int day;
		Date(int, int, int);
		int ToDays();
};

#endif