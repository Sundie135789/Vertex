#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>
#include "variable.hpp"
#define INT_MAX  1 << 16
#define INT_OUT (1 << 16) + 1
//TODO: refine createInt using findVariable.
std::string output = "", error = "", fileName = "";
  int lineNo = 1;
bool hasError = false;
std::vector<variable> variables;
variable* findVariable(std::string name, std::string type){
  for(int i=0; i<variables.size();i++){
    if(name == variables.at(i).name && type == variables.at(i).type){
      return &variables.at(i);
    }
  }
  return nullptr;
}
void takeInput(std::vector<std::string> tokens){
  if(tokens.size() != 2){
    error += fileName + ": Error: Invalid syntax for input\n";
    hasError = true;
    return;
  }
  tokens[1].pop_back(); // input num;
  variable* intvar = findVariable(tokens[1], "int");
  variable* strvar = findVariable(tokens[1], "string");
  if(intvar == nullptr && strvar == nullptr){
    error += fileName + ": Error: Non-existent variable used in input\n";
    hasError = true;
    return;
  }
  if(intvar == nullptr){
    std::string temp;
    std::getline(std::cin, temp);
    strvar->stringValue = temp;
  }
  if(strvar == nullptr){
    std::string tempstr;
    std::getline(std::cin, tempstr);
    try{
      if(std::stoi(tempstr) > INT_MAX){
        error += fileName + ": Error: Integer variable set to a too large value\n";
        hasError = true;
        std::cout << error;
        exit(1);
      }
      if(std::stoi(tempstr) < 0){
        error += fileName + ": Error: Signed integer set to negative value\n";
        hasError = true;
        return;
      }
      intvar->intValue = std::stoi(tempstr);
    }catch(std::invalid_argument){
      error += fileName + ": Error: Invalid value entered for input of variable\n";
      hasError = true;
      return;
    }
  }
}
std::vector<std::string> lex_line(std::string line){
  std::vector<std::string> tokens;
  std::string token = "";
  // int length = 10;
  int position = 0;
  while(position < line.size()){
    std::cout << position << '\n';
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
    }else if(c == ' '){
      // string a = "hello";
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
        if(position < line.size() && std::isdigit(line[position+1])){
          position++;
          token += c;
        }
        else{
          tokens.push_back(token);
          token = "";
        }
      }
   	  token += c;
      position++;
      tokens.push_back(token);	 
      token = "";
      //string name = "abhay krishna is my best friend.";
    }else if(c == '"'){
      token += c;
      while(position < line.length() && line[position] != '"'){
        position++;

      }
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
void createInt(std::vector<std::string> tokens){
  int value = INT_OUT;
  /*if(tokens.size() <= 4){
    error += fileName + ": Error: invalid syntax for integer variable creation.\n";
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
    error += fileName + ": Error: Integer variable name must contain at least one letter\n";
    hasError = true;
    return;
  }

  // Handle creation + setting value.
  if(tokens.size() == 4){
    if(tokens[2] != "="){
      error += fileName + ": Error: Unknown operator '" + tokens[2] + "'\n";
      hasError = true;
      return;
    }
    tokens[3].pop_back();
    
    for(int i=0; i<tokens[3].size();i++){
      if(std::isdigit(tokens[3][i]) == false && tokens[3][i] != ';' ){
          variable* v = findVariable(tokens[3], "int");
          if(v != nullptr){
            value = v->intValue;
          }
          else{
            error += fileName + ": Error: Invalid value for integer variable\n";
            hasError = true;
            return;
          }
      }
    }
    // check if int variable value is above INT_MAX or below 0.
    if(std::stoi(tokens[3]) > INT_MAX){
     error += fileName + ": Error: Integer variable set to a too large value\n";
      hasError = true;
      return;
    }
    if(std::stoi(tokens[3]) < 0){
      error += fileName + ": Error: Signed integer variable set to negative value\n";
      hasError = true;
      return;
    }
    //if just creating variable, then the checks for that.
  }else if(tokens.size() != 2){
    for(std::string s : tokens){
      std::cout << s << '\n';
    }
    error += fileName + ": Error: Invalid syntax for integer declaration\n"; 
    hasError = true;
    return;
  }
  // Example Statement: int a = 10;
  //Before creating variable, check if such a variable already exists.
  
//  if(tokens.size() == 4)  tokens[3].pop_back();
  if(tokens.size() == 2) tokens[1].pop_back();
  if(value != INT_OUT) value = tokens.size() == 4 ? std::stoi(tokens[3]) : INT_OUT;
  for(int i=0; i<variables.size();i++){
    if(variables[i].type == "int" && variables[i].name == tokens[1]){
      error += fileName + ": Error: Redefinition of '" + variables[i].name + "'\n";
      hasError = true;
      return;
    }
  }
    
  variables.push_back(variable(tokens[1], value));

}
// tokens.size() == 4
// 10;
void print(std::vector<std::string> tokens){
    if(tokens.size() != 2){
      error += fileName + ": Error: Invalid syntax for print statement at line " + std::to_string(lineNo) + "\n";
      hasError = true;
      return;
    }
    // Check what we are printing
    //For strings, dont check negation of being digit, check if quotation mark is found at i = 0. if found then it is a string literal. else if it is -
    // Not a digit, then it is a integer literal.
    bool isLiteral = true;
    tokens[1].pop_back();
    for(int i=0; i<tokens[1].size();i++){
      if(!std::isdigit(tokens[1][i])){
        isLiteral = false;
        break;
      }
    }
    // print 32
    if(isLiteral){
      std::cout << tokens[1];
    }else{
      variable* v = findVariable(tokens[1], "int");
      if(v == nullptr){
        error += fileName + ": Error: Could not find variable at line " + std::to_string(lineNo) + " -> '" + tokens[1] + "'\n";
        hasError = true;
        return;
      }
      std::cout << v->intValue;
    }
  }
void createString(std::vector<std::string> tokens){
  std::string value = "";
  bool hasLetter = false;
  for(int i=0;i<tokens[1].size();i++){
    if(std::isalpha(tokens[1][i])){
      hasLetter = true;
      break;
    }
  }
  if(hasLetter == false){
    error += fileName + ": Error: String variable name must contain at least one letter\n";
    hasError = true;
    return;
  }
  if(tokens.size() == 4) {
    if(tokens[2] != "=") {
      error += fileName + ": Error: Unknown operator '" + tokens[2] + "'\n";
      hasError = true;
      return;
    }
    tokens[3].pop_back();
    //string name = "mudit" tokens[3] has been popped back, so semicolon gone.
    //Literal vs Variable checking is done outside loop, unlike int creation.
    //Inside-loop checking is too complex for my feeble brain.
    bool isLiteral = false;
    if(tokens[3][0] == '"' && tokens[3][tokens[3].size()-1] == '"'){
      isLiteral = true;
    }
    else if(tokens[3][tokens[3].size()-1] == '"' || tokens[3][0] == '"'){
      error += fileName + ": Missing terminating '\"' at line " + std::to_string(lineNo) + '\n';
      hasError = true;
      return;
    }
    if(isLiteral){
      //Handle string literal. string a = "mudit and arvind. best friends forever.".  string a = "hello"
      tokens[3].erase(0, 1);
      tokens[3].pop_back();
      value = tokens[3];
    }else{
      //Handle string variable. string a = str;
      
    }
    //if(tokens.size() == 2)tokens[1].pop_back();
    // check if it already exists
    variable* v = findVariable(tokens[1], "string");
    if(v != nullptr){
      error += fileName + ": Error redefinition of '" + v->name + "'\n";
      hasError = true;
      return;
    }
    variables.push_back(variable(tokens[1], "string"));
    }
}

void run_line(std::vector<std::string> tokens, int lineNo){
  if(tokens.empty()){
    return;
  }
  
  //std::cout << "\n\n" << tokens[tokens.size() - 1][tokens[tokens.size()-1].size()-1] << "\n\n";
  if(tokens[tokens.size() - 1][tokens[tokens.size()-1].size()-1] != ';'){
    error += fileName + ": Error: Expected ';' at line " + std::to_string(lineNo) + '\n';
    hasError = true;
  }
  if(tokens[0] == "int"){
    createInt(tokens);
  }else if(tokens[0] == "string"){
    createString(tokens);
  }else if(tokens[0] == "input"){
    takeInput(tokens);
  }else if(tokens[0] == "output"){
    print(tokens);
  }
  else{
    error += fileName + ": Error: Unknown token at line " + std::to_string(lineNo) + " -> '" + tokens[0] + "'\n";
    hasError = true;
  }
    /*for(int i=0; i<tokens.size();i++){
    assert(i < tokens.size());
    std::cout << "Token Number " << (i+1) << ": " << tokens[i] << '\n';
  }*/
}
int main(int argc, char** argv){
  if(argc == 1){
    std::cout << "Error: Expected filename argument.\n";
    return 1; 
  }
  if(argc > 2){
    std::cout << "Error: Please enter only one file\n";
    return 1;
  }
  fileName = argv[1];
  std::string text;
  std::ifstream file(fileName);
  if(!file) {
    std::cout << "Error: Cannot open '" + fileName + ";\n";
    return 1;
  }
  while(getline(file, text)){
    run_line(lex_line(text), lineNo);
    if(hasError)
    {
      output += error;
      error ="";
      hasError = false;
    }
    lineNo++;
  }
  std::cout << output << std::endl;
  file.close();
  return 0;
}
