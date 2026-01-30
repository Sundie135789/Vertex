#include "headers/variable.hpp"
#include <string>

variable::variable(std::string type,int iVal, std::string strVal, std::string name) : intValue(iVal),
  strValue(strVal), type(type), name(name) {}
