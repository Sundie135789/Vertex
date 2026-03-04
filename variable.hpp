#pragma once
#include <string>
class variable{
  public:
	variable(std::string name, std::string stringValue);
	variable(std::string name, int intValue);
	std::string name;
	std::string type;
	std::string stringValue;
	int intValue;
};
