#include "variable.hpp"
variable::variable(std::string name, std::string stringValue){
				this->name = name;
				this->stringValue = stringValue;
				this->type = "string";
}
variable::variable(std::string name,  int intValue){
				this->name = name;
				this->intValue = intValue;
				this->type = "int";
}
