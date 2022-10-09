#include <iostream>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <valarray>
#include "Tree/AbstractNode.h"
#include "Tree/ValueNode.h"
#include "Tree/Node.h"

using std::string, std::map;

string unformattedFormula;
std::set<string> partialFormulas;
std::map<string, ValueNode*> evalLeaves;
AbstractNode root;


void getInput();
std::set<string> generatePartialFormulas(string orgFormula);
void printSet(std::set<string> &setToPrint);
void generateEvalLeaves(std::set<string> &partialFormulas, std::map<string, ValueNode*> &evalTree);
void buildTree(AbstractNode &root, std::map<string, ValueNode*> &evalLeaves, string unformattedFormula);
bool evaluateExpression(std::map<string, ValueNode*> &evalLeaves, string unformattedFormula);
bool generateAndTest(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula);

int main() {
    getInput();
    partialFormulas = generatePartialFormulas(unformattedFormula);

    generateEvalLeaves(partialFormulas, evalLeaves);
    
    generateAndTest(evalLeaves, unformattedFormula);



    return 0;
}

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

    return partialFormulas;
}

void generateEvalLeaves(std::set<string> &partialFormulas, std::map<string, ValueNode*> &evalTree) {
    for (const string &part: partialFormulas) {
        evalTree.insert(std::pair<string,ValueNode*> (part, new ValueNode()));
    }
}


void buildTree(AbstractNode &root, std::map<string, ValueNode*> &evalLeaves, string unformattedFormula) {
    AbstractNode* currentNodeHolder {nullptr};
    string space_delimiter = " ";
    size_t pos = 0;

    while ((pos = unformattedFormula.find(space_delimiter)) != string::npos) {
        if(currentNodeHolder == nullptr) {
            currentNodeHolder = new Node(  evalLeaves.at(unformattedFormula.substr(0, pos)) );
            unformattedFormula.erase(0, pos + space_delimiter.length());

        }
        else {



        }

    }


}

/**
 * Generates all possible assignments
 * @param evalLeaves
 * @param unformattedFormula
 */
bool generateAndTest(std::map<string, ValueNode*> &evalLeaves, string &unformattedFormula) {
    int upperBound = int (std::pow( 2, evalLeaves.size()));
    int index;
    for (int i = 0; i < upperBound; ++i) {
        int j = i;
        index = evalLeaves.size()-1;

        //Put int to binary values
        for (auto node: evalLeaves) {
            int evaluation = int (std::pow(2, index));
            int frac = j / evaluation;
            if(frac > 0) {
                node.second->setEvaluationValue(true);
                j -= evaluation;
            }
            else {
                node.second->setEvaluationValue(false);
            }
            --index;
        }
        //Assignment generated
        //Now test values
        if(evaluateExpression(evalLeaves, unformattedFormula)) {
            std::cout << "Found valid Assignment with number: " << std::to_string(i) << std::endl;
            return true;
        }
    }
    std::cout << "There was no valid assignment found!" << std::endl;
    return false;

}



bool* eval(const string &op, bool val1, bool val2) {
    if (op == "&") return new bool {val1 && val2};
    if (op == "|") return new bool {val1 || val2};
    throw std::invalid_argument( "Invalid operator" );
}

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
         //       vals.pop();
                *val2 = !*val2;
                delete op;
             //   vals.push(val2);


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
        std::cout << "Evaluation Value is " << x << std::endl;
        vals.pop();

        delete result;
        return x;







}







void printSet(std::set<string> &setToPrint) {
    for (const string &part: partialFormulas) {
        std::cout << part << std::endl;
    }
}




void getInput() {
    std::cout << "Enter your SAT formula (use & for and, | for or): " << std::endl;
    std::getline(std::cin, unformattedFormula);
}
