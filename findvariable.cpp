#include <string>
#include <vector>
#include "headers/variable.hpp"

variable *findVariable(std::string name, std::vector<variable> variables) {
  for (int i = 0; i < variables.size(); i++) {
    if (variables.at(i).name == name) {
      return &variables.at(i);
    }
  }
  return nullptr;
}

