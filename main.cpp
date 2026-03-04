#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>
#include <cassert>
#include "variable.hpp"
std::string output = "";
std::vector<std::string> lex_line(std::string line){
  std::vector<std::string> tokens;
  std::string token = "";
  int position = 0;
  while(position < line.size()){
    char c = line[position];
    //int a = 10;
    // position 4, token int
    if(std::isalpha(c)){
      while(position < line.size() && std::isalpha(line[position])){
        token+= line[position];
        position++;
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
      tokens.push_back(token);
    }else if(c == '=' || c == '+' || c == '-' || c == '*' || c == '/'){
   	  token += c;
      position++;
      tokens.push_back(token);	 
    }
    else if(c == ';'){ 
      token+=c;
      tokens.push_back(token);
      break;
    }
  }
  return tokens;
}
std::vector<variable> variables;
bool hasError = false;
std::string fileName;
std::string error;
void createInt(std::vector<std::string> tokens){
  if(tokens.size() <= 3){
    error += fileName + ": error: invalid syntax for integer variable creation.\n";
    hasError = true;
    return;
  }
  if(tokens[2] != "="){
    error += fileName + ": error: could not find operator -> " + tokens[2];
    hasError = true;
    return;
  }
  variable tempVar(tokens[1], std::stoi(tokens[3]));
  variables.push_back(tempVar);
  std::cout << "Created int variable with name : " << tokens[1]<<" and value : "<< tokens[3];
}
// tokens.size() == 4
// 10;
// 2
void run_line(std::vector<std::string> tokens, int lineNo){

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
