#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>


//TODO: put a if and a boolean around the REPL, so that we can read files and allow ./vertex filename.vx
class variable{
  public:
    
    int intValue;
    std::string strValue;
    std::string type,name;
    variable(std::string type, int iVal, std::string strVal, std::string name) : strValue(strVal), intValue(iVal), name(name), type(type) {}
};i
//TODO code works, in createInt function, detect variable name along with literal. good night. have a nice republic day. Gn.
std::vector<std::string> keywords = {
  "int",
  "string",
  "print"
};
std::vector<variable> variables;
std::string output = "";
std::string assembly;
int lineNo = 1;
variable* findVariable(std::string name){
  for(int i =0 ;i<variables.size();i++){
    if(variables.at(i).name == name){
      return &variables.at(i);
    }
  }
  return nullptr;
}

void createInt(std::vector<std::string> tokens, int lineNo){
  if(tokens.size() != 4){
    output += "Error at line " + std::to_string(lineNo) + " : Invalid syntax in INT declaration\n";
    return;
  }
  if(tokens[2] != "="){
    output += "Error at line " + std::to_string(lineNo) + " : Invalid syntax in INT declaration\n";
    return;
  }
  if(tokens[3] == ""){
    output += "Error at line " + std::to_string(lineNo) + " : Invalid value in INT declaration\n";
    return;
  }
  int val;
  // [IMPORTANT] the upper part of this try block checks for variable to variable assigment.
  // [IMPORTANT] the lower part of this try block checks for variable to int literal assignment.
  try{

    // checking for non-numbers.
  tokens[3].pop_back();
  for(int i =0;i<tokens[3].size();i++){
    if (!std::isdigit(tokens[3].at(i))) {
      // if there is a non number found, look for variable.
      variable *found = findVariable(tokens[3]);
      if(found != nullptr){
        if(found->type != "int") output += "Error at line " + std::to_string(lineNo) + " : Cannot set int variable to string variable\n";return;
        // this means it is not int literal + is variable + is int variable.
          
      }else{
        // this means not int literal + no variable. throw error.
        output += "Error at line " + std::to_string(lineNo) + " : No variable found by the name of : \""+ tokens[3]+ "\"\n";
        return;
      }
    }
  }
  val = std::stoi(tokens[3]);
  }catch(const std::out_of_range& e){
    output += "Error at line " + std::to_string(lineNo) + " : Value surpassed limit of 2^32 in INT declaration (See github documentation for details)\n";
    return;
  }
  // int a = abc;
  variable var = variable("int",val,"",tokens[1]);
  variables.push_back(var);
}
const std::string* findKeyword(std::vector<std::string>& vec,std::string& target){
  auto it = std::find(vec.begin(),vec.end(),target);
  if (it != vec.end()) return &(*it);
  else return nullptr;
}
std::vector<std::string> tokenize(std::string line){
 // efficient syntax tokenizer
 // example string: int a = 10;
  std::vector<std::string> tokens;
  std::string tokenBuffer;
  for(int i=0; i<line.length();i++){
    if(std::isalpha(line.at(i))){
      tokenBuffer += line.at(i);
      if(findKeyword(keywords, tokenBuffer) != nullptr){
        tokens.push_back(tokenBuffer);
        tokenBuffer = "";
        
        // std::cout << tokens[i] << std::endl;
        continue;
      }
    }else if(std::isdigit(line.at(i))){
      tokenBuffer += line.at(i);
    }else if(line.at(i) == ' '){
      
      if (tokenBuffer != "")tokens.push_back(tokenBuffer);
      if(tokenBuffer != "") tokenBuffer = "";
    }else if(line.at(i) == '+' || line.at(i) == '-' || line.at(i) == '*' || line.at(i) == '/' || line.at(i) == '='){
      tokenBuffer += line.at(i);
      tokens.push_back(tokenBuffer);
      tokenBuffer = "";
    }
  }
  tokens.push_back(tokenBuffer);
  tokenBuffer = "";
  return tokens;
} 
void error(std::string str, int lineNo){
  output += "Error at line " + std::to_string(lineNo) + str;
  return;
}
void interpret(std::string line, int lineNo){
  std::vector<std::string> tokens = tokenize(line);
  for(int i=0;i<tokens.size();i++){
    std::cout << tokens.at(i) << std::endl;
  }
  if(tokens[tokens.size()-1].back() != ';'){
    output += "Error at line " + std::to_string(lineNo) + " : Expected \';\' at the end of line\n";
    return;
  }
  if(tokens[0] == "int"){
    createInt(tokens, lineNo);
  }
  lineNo++;
  
}

int main(int argc, char* argv[]){
  std::string line;
  while(true){ 
    std::cout << "> ";
    std::getline(std::cin, line);
    if(line == "ret"){
      break;
    }
    interpret(line, lineNo);
  }
  if(output == "") output = "No Output";
  std::cout << "Output: " << output;
  return 0;
}

