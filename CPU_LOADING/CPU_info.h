#pragma once

#include "windows.h"
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
#include <string>

class CPU_info {
public:
	int _id;
	std::string _name;
	float _percent;

	CPU_info();
	CPU_info(int id, std::string name, float percent);
	CPU_info(const CPU_info &obj);
	CPU_info &operator=(CPU_info &obj);
	~CPU_info();

	friend std::ostream& operator<<(std::ostream& os, const CPU_info &obj);

	friend bool operator<(const CPU_info& p1, const CPU_info& p2);
	friend bool operator>(const CPU_info& p1, const CPU_info& p2);
	friend bool operator>=(const CPU_info& p1, const CPU_info& p2);
	friend bool operator<=(const CPU_info& p1, const CPU_info& p2);
	friend bool operator==(const CPU_info& p1, const CPU_info& p2);
	friend bool operator!=(const CPU_info& p1, const CPU_info& p2);
};

