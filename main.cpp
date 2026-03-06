#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>
#include <cassert>
#include "variable.hpp"
//TODO: Refine error messages
#define INT_MAX  1 << 16;
#define INT_OUT (1 << 16) + 1;
std::string output = "";
std::vector<std::string> lex_line(std::string line){
  std::vector<std::string> tokens;
  std::string token = "";
  // int length = 10;
  int position = 0;
  while(position < line.size()){
    char c = line[position];
    //int 22=25;
    //position 4, token 
    if(std::isalpha(c)){
      token = "";
      while(position < line.size() && (std::isalnum(line[position]) || line[position] == '_')){
        token+= line[position];
        position++;
      }
      if(position < line.size() && line[position] == ';'){
        token+= ';';
        position ++;
      }
      tokens.push_back(token);
      token = "";
    }else if(std::isspace(c)){
      //if(token != "") tokens[position] = token;
      position++;
    }else if(std::isdigit(c)){
      token = "";
      while(position < line.size() && std::isdigit(line[position])){
        token += line[position];
        position ++;
      }
      if(position < line.size() && line[position] == ';'){
        token += ';';
        position++;
      }
      tokens.push_back(token);
    }else if(c == '=' || c == '+' || c == '-' || c == '*' || c == '/'){
      if(token != ""){
        tokens.push_back(token);
        token = "";
      }
   	  token += c;
      position++;
      tokens.push_back(token);	 
      token = "";
    }
    else{
      position++ ;
    }
  }
  std::cout << "\n\n\n";
  for(int i=0; i<tokens.size();i++){
    std::cout << tokens.at(i) << '\n';
  }
  std::cout << "\n\n\n";
  return tokens;
}
std::vector<variable> variables;
bool hasError = false;
std::string fileName;
std::string error;
void createInt(std::vector<std::string> tokens){
  /*if(tokens.size() <= 4){
    error += fileName + ": error: invalid syntax for integer variable creation.\n";
    hasError = true;
    return;
  }*/
  // Check if the variable name has atleast a single letter.
  bool hasLetter = false;
  for(int i=0;i<tokens[1].size();i++){
    if(std::isalpha(tokens[1][i])){
      hasLetter = true;
      break;
    }
  }
  if(hasLetter == false){
    error += fileName + ": error: integer variable name must have atleast one alphabetical letter\n";
    hasError = true;
    return;
  }

  // Handle creation + setting value.
  if(tokens.size() == 4){
    if(tokens[2] != "="){
      error += fileName + ": error: could not find operator -> " + tokens[2] + "\n";
      hasError = true;
      return;
    }
    for(int i=0; i<tokens[3].size();i++){
      if(std::isdigit(tokens[3][i]) == false && tokens[3][i] != ';'){
        error += fileName + ": error: cannot set integer variable's value to invalid literal\n";
        hasError = true;
        return;
      }
    }
    //if just creating variable, then the checks for that.
  }else if(tokens.size() != 2){
    for(std::string s : tokens){
      std::cout << s << '\n';
    }
    error += fileName + ": error: invalid syntax for creating integer variable\n"; 
    hasError = true;
    return;
  }
  // Example Statement: int a = 10;
  //Before creating variable, check if such a variable already exists.
  if(tokens.size() == 4)  tokens[3].pop_back();
  int value = tokens.size() == 4 ? std::stoi(tokens[3]) : INT_OUT;
  if(tokens.size() == 2) tokens[1].pop_back();
  for(int i=0; i<variables.size();i++){
    if(variables[i].type == "int" && variables[i].name == tokens[1]){
      error += fileName + ": error: redefinition of integer variable -> " + variables[i].name + '\n';
      hasError = true;
      return;
    }
  }
  variables.push_back(variable(tokens[1], value));

}
// tokens.size() == 4
// 10;
// 2
void run_line(std::vector<std::string> tokens, int lineNo){
  if(tokens.empty()){
    return;
  }

  //std::cout << "\n\n" << tokens[tokens.size() - 1][tokens[tokens.size()-1].size()-1] << "\n\n";
  if(tokens[tokens.size() - 1][tokens[tokens.size()-1].size()-1] != ';'){
    error += fileName + ": error: expected ';' in line " + std::to_string(lineNo) + '\n';
    hasError = true;
  }
  if(tokens[0] == "int"){
    createInt(tokens);
  }
  /*for(int i=0; i<tokens.size();i++){
    assert(i < tokens.size());
    std::cout << "Token Number " << (i+1) << ": " << tokens[i] << '\n';
  }*/
}
int main(int argc, char** argv){
  std::cout << "RUNNING";
  fileName = argv[1];
  std::string text;
  std::ifstream file(fileName);
  if(!file) std::cout << "Could not open test.v";
  int lineNo = 1;
  while(getline(file, text)){
    run_line(lex_line(text), lineNo);
    if(hasError)
    {
      output += error;
      hasError = false;
    }
    lineNo++;
  }
  std::cout << output << std::endl;
  file.close();
  return 0;
}
