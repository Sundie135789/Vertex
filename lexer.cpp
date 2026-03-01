#include "lexer.hpp"
#include "token.hpp"
#include <iostream>
std::vector<Token> lex_line(std::string line){
  int position = 0;
  std::vector<Token> tokens;
  while(position < line.size()){
    std::cout << "Character: " << position << "\n";
    position++;
  }
  return tokens;
}
