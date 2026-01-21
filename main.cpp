//TODO line 75 create int.
#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <fstream>  
#include <array>
class variable{
  public:
    
    int intValue;
    std::string strValue;
    std::string type,name;
    variable(std::string type, int iVal, std::string strVal, std::string name) : strValue(strVal), intValue(iVal), name(name) {}
};
std::vector<variable> variables;
std::string output;
std::string assembly;
std::string runAsm(const std::string& asmFilename, const std::string& assemblyCode) {
    // 1️⃣ Write assembly to file
    std::ofstream out(asmFilename);
    if (!out.is_open()) {
        std::cerr << "Failed to open file: " << asmFilename << "\n";
        return "No Output";
    }
    out << assemblyCode;
    out.close();

    // 2️⃣ Assemble & link
    std::string objFile = "temp.o";
    std::string exeFile = "temp_exec";
    std::string assembleCmd = "gcc -c -nostdlib -o " + objFile + " " + asmFilename; // assemble AT&T syntax
    std::string linkCmd = "gcc -no-pie -o " + exeFile + " " + objFile;    // link to executable

    if (system(assembleCmd.c_str()) != 0) return "No Output";
    if (system(linkCmd.c_str()) != 0) return "No Output";

    // 3️⃣ Run executable and capture stdout
    std::array<char, 128> buffer;
    std::string output;
    FILE* pipe = popen(("./" + exeFile).c_str(), "r");
    if (!pipe) return "No Output";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
        output += buffer.data();
    pclose(pipe);

    if (output.empty()) return "No Output";
    return output;
}
void createInt(std::vector<std::string> tokens){
  if(tokens.size() != 4){
    output += "Error: Invalid syntax in INT declaration\n";
    return;
  }
  if(tokens[2] != "="){
    output += "Error: Invalid syntax in INT declaration\n";
    return;
  }
  if(tokens[3] == ""){
    output += "Error: Invalid value in INT declaration\n";
    return;
  }
  int val;
  try{
  tokens[3].pop_back();
  val = std::stoi(tokens[3]);
  }catch(const std::invalid_argument& e){
    output += "Error: Invalid value in INT declaration";
    return;
  }catch(const std::out_of_range& e){
    output += "Error: Value surpassed limit of 2^32 in INT declaration (See github documentation for details)";
    return;
  }
  variable var = variable("int",val,nullptr,tokens[1]);
  variables.push_back(var);
  output += "int " + var.name + " = " + std::to_string(var.intValue);
      }
std::vector<std::string> tokenize(std::string line){
  std::stringstream ss(line);
  std::string word;
  std::vector<std::string> tokens;
  while(ss >> word){
    tokens.push_back(word);
  }
  return tokens;
} 
void convert(std::string line){
  std::vector<std::string> tokens = tokenize(line);
  if(tokens[tokens.size()-1].back() != ';'){
    output += "Error: Expected \';\' at the end of line";
    return;
  }
  if(tokens[0] == "int"){
    createInt(tokens);
  }
}

int main(){
  std::string line;
  while(true){ 
    std::cout << "> ";
    std::getline(std::cin, line);
    if(line == "run"){
      break;
    }
    convert(line);
  }
  runAsm("",assembly);
  std::cout << "Output: " << output;
  return 0;
}

