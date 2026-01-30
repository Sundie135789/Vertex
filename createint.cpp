#include "headers/createint.hpp"
#include "headers/findvariable.hpp"
#include "headers/variable.hpp"
#include <stdexcept>
void createInt(std::vector<std::string> tokens, int lineNo, std::string output, std::vector<variable> variables) {
    if (tokens.size() != 4) {
        output += "Error at line " + std::to_string(lineNo) +
                  " : Invalid syntax in INT declaration\n";
        return;
    }

    if (tokens[2] != "=") {
        output += "Error at line " + std::to_string(lineNo) +
                  " : Invalid syntax in INT declaration\n";
        return;
    }

    if (tokens[3] == "") {
        output += "Error at line " + std::to_string(lineNo) +
                  " : Invalid value in INT declaration\n";
        return;
    }

    int val;

    // Upper part: variable-to-variable assignment
    // Lower part: variable-to-int-literal assignment
    try {
        // Remove trailing character (probably ;)
        tokens[3].pop_back();

        for (int i = 0; i < tokens[3].size(); i++) {
            if (!std::isdigit(tokens[3][i])) {

                // Non-number found → check if it's a variable
                variable* found = findVariable(tokens[3], variables);

                if (found != nullptr) {
                    if (found->type != "int") {
                        output += "Error at line " + std::to_string(lineNo) +
                                  " : Cannot set int variable to string variable\n";
                    }
                    return;
                } else {
                    output += "Error at line " + std::to_string(lineNo) +
                              " : No variable found by the name of : \"" +
                              tokens[3] + "\"\n";
                    return;
                }
            }
        }

        val = std::stoi(tokens[3]);
    }
    catch (const std::out_of_range&) {
        output += "Error at line " + std::to_string(lineNo) +
                  " : Value surpassed limit of 2^32 in INT declaration "
                  "(See github documentation for details)\n";
        return;
    }

    // int a = abc;
    variable var("int", val, "", tokens[1]);
    variables.push_back(var);
};

