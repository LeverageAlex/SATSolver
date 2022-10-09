//
// Created by Alex on 08.10.2022.
//

#include "Node.h"



AbstractNode Node::getRight() const {
    return this->right;
}
AbstractNode Node::getLeft() const {
    return this->left;
}

Node::Node(AbstractNode left) : AbstractNode(false), left{left}{

}

void Node::setRight(AbstractNode right) {
    this->right = right;
}


