#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>
std::vector<std::string> lex_line(std::string line){
  std::vector<std::string> tokens;
  std::string token = "";
  int position = 0;
  while(position < line.size()){
    char c = line[position];
    //int a = 10;
    // position 1, token i
    if(std::isalpha(c)){
      while(position < line.size() && std::isalpha(line[position])){
        token+= line[position];
        position++;
      }
      tokens.push_back(token);
    }else if(std::isspace(c)){
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
      
      break;
    }
  }
  return tokens;
}
void run_line(std::vector<std::string> tokens){

  for(int i=0; i<tokens.size();i++){
    std::cout << "token number " << i << '\n';
  }
}
int main(){
  std::string text;
  std::ifstream file("test.v");
if(!file) std::cout << "Could not open test.v";
  std::string output = "";
  while(getline(file, text)){
    run_line(lex_line(text));
  }
  file.close();
}
