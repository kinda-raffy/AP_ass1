
#include <iostream>
#include "Node.h"


Node::Node(int row, int col, int distanceToS){
   this->row = row;
   this->col = col;
   this->distanceToS = distanceToS;
}
    
Node::~Node()= default;

/*
 * Returns the row of the node
 * @return row - the row of the node
 */
int Node::getRow() const{
   return this->row;
}

/*
 * Returns the column of the node
 * @return col - the column of the node
 */
int Node::getCol() const{
    return this->col;
}

/*
 * Returns the distance to the start node
 * @return distanceToS - the distance to the start node
 */
int Node::getDistanceToS() const{
    return this->distanceToS;
}

/*
 * Sets the distance to the start node
 * @param distanceToS - the distance to the start node
 */
void Node::setDistanceToS(int distanceToS){
   this->distanceToS = distanceToS;
}
