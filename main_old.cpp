/*#include <iostream>
#include <string>
#include <set>
#include <map>
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
void findEvaluationExpression(string toSplit);

int main() {
    getInput();
    partialFormulas = generatePartialFormulas(unformattedFormula);

    printSet(partialFormulas);
    generateEvalLeaves(partialFormulas, evalLeaves);

   // buildTree(root,  evalLeaves, unformattedFormula);

    findEvaluationExpression(unformattedFormula);

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

void findEvaluationExpression(string toSplit) {
    int depth = 0;
    int beginning = 0;
    int status = 0;
    int end = toSplit.length();
    for (size_t i = 0; i < toSplit.length(); i++) {
        if(toSplit.at(i) == '(') {
            ++depth;
            if(status == 0) {
                status = 1;
                beginning = i + 1;
            }
        }
        if(toSplit.at(i) == ')') {
            --depth;
            if(depth == 0) {
                end = i;
                std::cout << "First split " <<toSplit.substr(beginning, end - beginning) << std::endl;
                std::cout << "Second split " << toSplit.substr(end + 1, toSplit.length()-end) << std::endl;
            }
        }
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



bool eval(string op, bool val1, bool val2) {
    if (op == "&") return val1 && val2;
    if (op == "|") return val1 || val2;
    throw std::invalid_argument( "Invalid operator" );
}

void evaluateExpression(std::set<string> evalLeaves, ) {




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
*/