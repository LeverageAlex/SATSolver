#include <iostream>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <valarray>
#include "Tree/ValueNode.h"

using std::string, std::map;

string unformattedFormula;
std::set<string> partialFormulas;
std::map<string, ValueNode*> evalLeaves;


void getInput(string &unformattedFormula);
std::set<string> generatePartialFormulas(string orgFormula);
void printSet(std::set<string> &setToPrint);
void generateEvalLeaves(std::set<string> &partialFormulas, std::map<string, ValueNode*> &evalTree);
bool evaluateExpression(std::map<string, ValueNode*> &evalLeaves, string unformattedFormula);
bool generateAndTest(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula, bool printSteps = false);
void printCurrentAssignment(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula, bool solved);
bool isSolveAble(string &formula);
inline void printAssignmentTableHead(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula);
inline void printAssignementTableLine(std::map<string, ValueNode*> &evalLeaves, bool solved);
void printTruthTable(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula);

int main() {
    //setup
    getInput(unformattedFormula);

    //solve
    bool solved = isSolveAble(unformattedFormula);
    if (solved)
    printCurrentAssignment(evalLeaves, unformattedFormula, true);

    return 0;
}



/**
 * Filters every literal in orgFormula
 * @param orgFormula
 * @return
 */
std::set<string> generatePartialFormulas(string orgFormula) {
    string space_delimiter = " ";
    std::set<string> partialFormulas;
    size_t pos = 0;
    while ((pos = orgFormula.find(space_delimiter)) != string::npos) {
        partialFormulas.insert(orgFormula.substr(0, pos));
        orgFormula.erase(0, pos + space_delimiter.length());
    }
    if(!orgFormula.empty()) {
        partialFormulas.insert(orgFormula);
    }
    if( partialFormulas.find("&") != partialFormulas.end()) {
        partialFormulas.erase(partialFormulas.find("&"));
    }

    if( partialFormulas.find("|") != partialFormulas.end()) {
        partialFormulas.erase(partialFormulas.find("|"));
    }

    if( partialFormulas.find("(") != partialFormulas.end()) {
        partialFormulas.erase(partialFormulas.find("("));
    }

    if( partialFormulas.find(")") != partialFormulas.end()) {
        partialFormulas.erase(partialFormulas.find(")"));
    }

    if( partialFormulas.find("-") != partialFormulas.end()) {
        partialFormulas.erase(partialFormulas.find("-"));
    }

    return partialFormulas;
}

/**
 * Generates for each literal in set a corresponding node for evaluation
 * @param partialFormulas
 * @param evalTree
 */
void generateEvalLeaves(std::set<string> &partialFormulas, std::map<string, ValueNode*> &evalTree) {
    for (const string &part: partialFormulas) {
        evalTree.insert(std::pair<string,ValueNode*> (part, new ValueNode()));
    }
}




/**
 * Generates all possible assignments until valid assignment found. If there is none returns false.
 * If parameter printSteps is set to true (default false) it will always return true, but print every testable assignment to stdout
 * @param evalLeaves
 * @param unformattedFormula
 * @param printSteps
 */
bool generateAndTest(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula, bool printSteps) {
    int upperBound = int (std::pow( 2, evalLeaves.size()));
    int index;
    for (int i = 0; i < upperBound; ++i) {
        int j = i;
        index = evalLeaves.size()-1;
        int evaluation = int (std::pow(2, index));
        //Put int to binary values
        for (auto node: evalLeaves) {

            int frac = j / evaluation;
            if(frac > 0) {
                node.second->setEvaluationValue(true);
                j -= evaluation;
            }
            else {
                node.second->setEvaluationValue(false);
            }
            //same as --index, but more efficient than recalculation power
           evaluation /= 2;
        }
        //Assignment generated
        //Now test values
        if(evaluateExpression(evalLeaves, unformattedFormula)) {
            //std::cout << "Found valid Assignment with number: " << std::to_string(i) << std::endl;
            if(!printSteps)
            return true;
            else printAssignementTableLine(evalLeaves, true);
        }
        else if(printSteps) printAssignementTableLine(evalLeaves, false);
    }
    std::cout << "There was no valid assignment found!" << std::endl;
    return false;

}


/**
 * Evaluates the passed expression
 * @param op
 * @param val1
 * @param val2
 * @return bool
 */
bool* eval(const string &op, bool val1, bool val2) {
    if (op == "&") return new bool {val1 && val2};
    if (op == "|") return new bool {val1 || val2};
    throw std::invalid_argument( "Invalid operator" );
}

/**
 * Parses the unformatted expression in an proper manner to solve the equation in right order (brackets bind stronger
 * than &, & binds stronger than |) and evalutes it with the assignment in evalLeaves
 * @param evalLeaves
 * @param unformattedFormula
 * @return
 */
bool evaluateExpression(std::map<string, ValueNode*> &evalLeaves, string unformattedFormula) {

    std::map<string, int> precedence = std::map<string, int>();
    precedence.insert(std::pair<string, int>("(", 0));
    precedence.insert(std::pair<string, int>(")", 0));
    precedence.insert(std::pair<string, int>("|", 1));   // + and - have lower precedence than * and /
    precedence.insert(std::pair<string, int>("&", 2));
    precedence.insert(std::pair<string, int>("-", 3));

    std::stack<string*> ops  = std::stack<string*>();
    std::stack<bool*> vals = std::stack<bool*>();

    string space_delimiter = " ";
    size_t pos = 0;


    while ((pos = unformattedFormula.find(space_delimiter)) != string::npos || unformattedFormula.length() > 0) {
        string s = unformattedFormula.substr(0, pos);


        //Token is a value
        if (precedence.find(s) == precedence.end()) {
            bool* cpy = new bool {evalLeaves.at(s)->getEvaluationValue()};
            vals.push(cpy);

            if(pos != string::npos) {
                unformattedFormula.erase(0, pos + space_delimiter.length());
            }
            else {
                unformattedFormula = "";
            }
            continue;
        }


        // token is an operator
        while (true) {

            // the last condition ensures that the operator with higher precedence is evaluated first
            if (ops.empty() || s == "(" || (precedence.at(s) > precedence.at(*ops.top()))) {
                string* cpy = new string {s};
                ops.push(cpy);
                break;
            }

            // evaluate expression
            string* op = ops.top();
            ops.pop();

            // but ignore left parentheses
            if (*op == "(") {
                delete op;
                break;
            }
            else if(*op == "-") {
                bool* val2 = vals.top();
                *val2 = !*val2;
                delete op;


            }

                // evaluate operator and two operands and push result onto value stack
            else {
                bool* val2 = vals.top();
                vals.pop();
                bool* val1 = vals.top();
                vals.pop();
                vals.push(eval(*op, *val1, *val2));
                delete val1;
                delete val2;
                delete op;
            }
        }
        if(pos != string::npos) {
            unformattedFormula.erase(0, pos + space_delimiter.length());
        }
        else {
            unformattedFormula = "";
        }
    }



        // finished parsing string - evaluate operator and operands remaining on two stacks
        while (!ops.empty()) {
            string* op = ops.top();
            ops.pop();
            bool *val2 = vals.top();
            if(*op == "-") {
                *val2 = !*val2;
                delete op;
            }
            else {
                vals.pop();
                bool *val1 = vals.top();
                vals.pop();
                vals.push(eval(*op, *val1, *val2));
                delete val1;
                delete val2;
                delete op;
            }
        }

        // last value on stack is value of expression
        bool* result = vals.top();
        bool x = {*result};
        vals.pop();

        delete result;
        return x;

}

/**
 * Prints current assignment in evalLeaves
 * @param evalLeaves
 * @param unformattedFormula
 * @param solved
 */
void printCurrentAssignment(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula, bool solved) {
    printAssignmentTableHead(evalLeaves, unformattedFormula);
    printAssignementTableLine(evalLeaves, solved);
}


/**
 * Prints only the head of a truth table
 * @param evalLeaves
 * @param unformattedFormula
 */
inline void printAssignmentTableHead(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula) {
    for (auto leaf: evalLeaves) {
        std::cout << leaf.first << "   ";
    }
    std::cout << unformattedFormula << std::endl;
}

/**
 * Prints the current assignment without literal headers
 * @param evalLeaves
 * @param solved
 */
inline void printAssignementTableLine(std::map<string, ValueNode*> &evalLeaves, bool solved) {
    for (auto leaf: evalLeaves) {
        std::cout << std::to_string(leaf.second->getEvaluationValue()) << "   ";
        for (int i = 0; i < leaf.first.length()-1; ++i) {
            std::cout << " ";
        }
    }
    if(solved) std::cout << "1" << std::endl;
    else std::cout << "0" << std::endl;
}

/**
 * Prints the all possible assignments for unformattedFormula with its corresponding evaluation values
 * @param evalLeaves
 * @param unformattedFormula
 */
inline void printTruthTable(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula) {
    printAssignmentTableHead(evalLeaves, unformattedFormula);
    generateAndTest(evalLeaves, unformattedFormula, true);
}




/**
 * Debug method for printing all unique literals in passed formula
 * @param setToPrint
 */
void printSet(std::set<string> &setToPrint) {
    for (const string &part: partialFormulas) {
        std::cout << part << std::endl;
    }
}



/**
 * Reads from stdin the formula to evaluate
 */
void getInput(string &unformattedFormula) {
    std::cout << "Enter your SAT formula (use & for and, | for or): " << std::endl;
    std::getline(std::cin, unformattedFormula);
}

/**
 * Tries to solve passed formula
 * @param formula
 * @return whether formula is solveable
 */
bool isSolveAble(string &formula) {
    //setup
    partialFormulas = generatePartialFormulas(unformattedFormula);
    generateEvalLeaves(partialFormulas, evalLeaves);

    //solve formula
    return generateAndTest(evalLeaves, unformattedFormula);
}