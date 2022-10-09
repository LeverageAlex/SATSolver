//
// Created by Alex on 08.10.2022.
//

#ifndef SATSOLVER_NODE_H
#define SATSOLVER_NODE_H


#include "AbstractNode.h"

class Node : public AbstractNode {
private:
    AbstractNode left;
    AbstractNode right;

public:
    Node(AbstractNode left);
    AbstractNode getLeft() const;
    AbstractNode getRight() const;
    void setRight(AbstractNode right);
};


#endif //SATSOLVER_NODE_H
