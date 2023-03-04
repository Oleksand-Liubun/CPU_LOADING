#include "CPU_info.h"

CPU_info::CPU_info()
{
	this->_id = 0;
	this->_name = "";
	this->_percent = 0;
}

CPU_info::CPU_info(int id, std::string name, float percent)
{
	this->_id = id;
	this->_name = name;
	this->_percent = percent;
}

CPU_info::CPU_info(const CPU_info &obj) {
	this->_id = obj._id;
	this->_name = obj._name;
	this->_percent = obj._percent;
}

CPU_info &CPU_info::operator=(CPU_info &obj)
{
	this->_id = obj._id;
	this->_name = obj._name;
	this->_percent = obj._percent;
	return obj;
}

std::ostream& operator<<(std::ostream& os, const CPU_info &obj)
{
	os << "Id: " << obj._id << " "
		<< "name: " << obj._name << " "
		<< "percent: " << obj._percent << " ";
	return os;
}

bool operator<(const CPU_info& p1, const CPU_info& p2) { return p1._name < p2._name; }
bool operator>(const CPU_info& p1, const CPU_info& p2) { return p1._name > p2._name; }
bool operator>=(const CPU_info& p1, const CPU_info& p2) { return p1._name >= p2._name; }
bool operator<=(const CPU_info& p1, const CPU_info& p2) { return p1._name <= p2._name; }
bool operator==(const CPU_info& p1, const CPU_info& p2) { return p1._id == p2._id; }
bool operator!=(const CPU_info& p1, const CPU_info& p2) { return p1._name != p2._name; }

CPU_info::~CPU_info(){}