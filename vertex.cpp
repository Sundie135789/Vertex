#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "headers/createint.hpp"
#include "headers/variable.hpp"

std::vector<std::string> keywords = {"int", "string", "print"};
std::vector<variable> variables;
std::string output = "";
std::string assembly;
int lineNo = 1;
//TODO add semicolon check, work on game engine. 
const std::string *findKeyword(std::vector<std::string> &vec,
                               std::string &target) {
  auto it = std::find(vec.begin(), vec.end(), target);
  if (it != vec.end())
    return &(*it);
  else
    return nullptr;
}
std::vector<std::string> tokenize(std::string line) {
  // efficient syntax tokenizer
  // example string: int a = 10;
  std::vector<std::string> tokens;
  std::string tokenBuffer;
  for (int i = 0; i < line.length(); i++) {
    if (std::isalpha(line.at(i))) {
      tokenBuffer += line.at(i);
      if (findKeyword(keywords, tokenBuffer) != nullptr) {
        tokens.push_back(tokenBuffer);
        tokenBuffer = "";

        // std::cout << tokens[i] << std::endl;
        continue;
      }
    } else if (std::isdigit(line.at(i))) {
      tokenBuffer += line.at(i);
    } else if (line.at(i) == ' ') {

      if (tokenBuffer != "")
        tokens.push_back(tokenBuffer);
      if (tokenBuffer != "")
        tokenBuffer = "";
    } else if (line.at(i) == '+' || line.at(i) == '-' || line.at(i) == '*' ||
               line.at(i) == '/' || line.at(i) == '=') {
      tokenBuffer += line.at(i);
      tokens.push_back(tokenBuffer);
      tokenBuffer = "";
    }
  }
  tokens.push_back(tokenBuffer);
  tokenBuffer = "";
  return tokens;
}
void error(std::string str, int lineNo) {
  output += "Error at line " + std::to_string(lineNo) + str;
  return;
}
void interpret(std::string line, int lineNo) {
  std::vector<std::string> tokens = tokenize(line);
  for(int i=0; i<tokens.size();i++){
    std::cout << tokens.at(i) << std::endl;
  
  }
  return;
  if (tokens[tokens.size() - 1].back() != ';') {
    output += "Error at line " + std::to_string(lineNo) +
              " : Expected \';\' at the end of line\n";
    return;
  }
  if (tokens[0] == "int") {
    createInt(tokens, lineNo,output, variables);
  }else{
    output += "Error at line " + std::to_string(lineNo) + " : Invalid start of expression\n";
  }
  lineNo++;
}

int main(int argc, char *argv[]) {
  bool repl = false;
  if (argc == 1 || argc == 0)
    repl = true;
  if (repl) {
    std::string line;
    while (true) {
      std::cout << "> ";
      std::getline(std::cin, line);
      if (line == "ret") {
        break;
      }
      interpret(line, lineNo);
    }
    return 0;
  } else {
    std::ifstream file(argv[1]);
    if (!file)
      std::cout << "Output: Error: System faced an error while opening file"
                << std::endl;
    return 1;
    if (std::filesystem::path(argv[1]).extension() != ".vx")
      std::cout << "Output: Error: File extension must be \".vx\"" << std::endl;
    return 1;
    std::string line;
    lineNo = 1;
    while (getline(file, line)) {
      if (line.empty() && std::any_of(line.begin(),line.end(),[](char c){return c != ' ';})) continue;;
      interpret(line, lineNo);
      lineNo++;
      output += "Error: File extension must be \".vx\"";
    }
  }
  if (output == "") output = "No Output";
  std::cout << "Output: " << output << std::endl;
  return 0;
}
