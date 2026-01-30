#pragma once
#include <string>

class variable {
public:
  int intValue;
  std::string strValue;
  std::string type, name;
  variable(std::string type, int iVal, std::string strVal, std::string name);
};
