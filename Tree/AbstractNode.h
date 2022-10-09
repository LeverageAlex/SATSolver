//
// Created by Alex on 08.10.2022.
//

#ifndef SATSOLVER_ABSTRACTNODE_H
#define SATSOLVER_ABSTRACTNODE_H

class AbstractNode {
private:
    bool isValue {false};
    bool evaluationValue {false};
public:

    bool getType() const {
        return isValue;
    }
    AbstractNode(bool concrete = false) : isValue{concrete} {

    }
    bool getEvaluationValue() {
        return evaluationValue;
    }
    void setEvaluationValue(bool evalValue) {
        evaluationValue = evalValue;
    }
};

#endif //SATSOLVER_ABSTRACTNODE_H
