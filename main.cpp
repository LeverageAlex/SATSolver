#include <iostream>
#include <string>
#include <set>
#include <map>
#include <stack>
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
void evaluateExpression(std::map<string, ValueNode*> &evalLeaves, string unformattedFormula);

int main() {
    getInput();
    partialFormulas = generatePartialFormulas(unformattedFormula);

    printSet(partialFormulas);
    generateEvalLeaves(partialFormulas, evalLeaves);

   // buildTree(root,  evalLeaves, unformattedFormula);

    evalLeaves.at("A")->setEvaluationValue(true);
    evaluateExpression(evalLeaves, unformattedFormula);




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



bool* eval(const string &op, bool val1, bool val2) {
    if (op == "&") return new bool {val1 && val2};
    if (op == "|") return new bool {val1 || val2};
    throw std::invalid_argument( "Invalid operator" );
}

void evaluateExpression(std::map<string, ValueNode*> &evalLeaves, string unformattedFormula) {

    std::map<string, int> precedence = std::map<string, int>();
    precedence.insert(std::pair<string, int>("(", 0));
    precedence.insert(std::pair<string, int>(")", 0));
    precedence.insert(std::pair<string, int>("|", 1));   // + and - have lower precedence than * and /
    precedence.insert(std::pair<string, int>("&", 2));

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
            string op = *ops.top();
            ops.pop();

            // but ignore left parentheses
            if (op == "(") {
                break;
            }

                // evaluate operator and two operands and push result onto value stack
            else {
                bool val2 = *vals.top();
                vals.pop();
                bool val1 = *vals.top();
                vals.pop();
                vals.push(eval(op, val1, val2));
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
            bool* val2 = vals.top();
            vals.pop();
            bool* val1 = vals.top();
            vals.pop();
            vals.push(eval(*op, *val1, *val2));
            delete val1;
            delete val2;
            delete op;

        }

        // last value on stack is value of expression
        std::cout << "Evaluation Value is " << *vals.top() << std::endl;







}









inline void setEvalValue(std::map<string, ValueNode*> &list, string &variable, bool evalValue) {
    list.at(variable)->setEvaluationValue(evalValue);
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
