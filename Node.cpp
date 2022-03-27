
#include <iostream>
#include "Node.h"


Node::Node(int row, int col, int distanceToS){
   this->row = row;
   this->col = col;
   this->distanceToS = distanceToS;
}
    
Node::~Node()= default;

int Node::getRow() const{
   return this->row;
}

int Node::getCol() const{
    return this->col;
}

int Node::getDistanceToS() const{
    return this->distanceToS;
}

void Node::setDistanceToS(int distanceToS){
   this->distanceToS = distanceToS;
}
