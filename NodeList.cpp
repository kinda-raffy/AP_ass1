
#include "NodeList.h"
#include <iostream>



NodeList::NodeList(){
   this->length = 0;
}


NodeList::~NodeList(){
   // Delete node in nodes array.
   for(int i = 0; i < this->length; i++){
      delete this->nodes[i];
   }
}

NodeList::NodeList(NodeList& other) :
   length(other.length)
{
    // Deep copy of nodes array.
    for(int i = 0; i < this->length; i++){
       this->nodes[i] = new Node(other.nodes[i]->getRow(),
                                 other.nodes[i]->getCol(),
                                 other.nodes[i]->getDistanceToS());
    }
}



int NodeList::getLength(){
   return this->length;
}


NodePtr NodeList::get(int i){
    // Check if index is within bounds.
   if (i > this->length || i < 0) {
       throw std::errc::argument_out_of_domain;  // FIXME: Convert to custom error.
   }
   return nodes[i];
}

void NodeList::addBack(NodePtr newNode){
    // Increase count.
   (this->length)++;
    // Add a COPY node element to the BACK of the nodelist
   nodes[this->length - 1] = new Node(newNode->getRow(), newNode->getCol(), newNode->getDistanceToS());

    // Append nodePtr to nodeList.
    // nodes[this->length - 1] = newNode;
}

void NodeList::removeBack(NodePtr remNode) {
    // If node is at the back.
    if (nodes[this->length] == remNode) {
        nodes[this->length] = nullptr;
        (this->length)--;
    }
}

bool NodeList::containsNode(NodePtr node){
    bool found = false;
    for (int i = 0; (i < this->length && !found && this->length != 0); i++) {
        if (node->getRow() == nodes[i]->getRow() && node->getCol() == nodes[i]->getCol()) {
            found = true;  // FIXME: Should Break as soon as it is found.
        }
    } return found;
}

bool NodeList::containsCdr(int row, int col){
    bool found = false;
    for (int i = 0; (i < this->length && !found && this->length != 0); i++) {
        if (row == nodes[i]->getRow() && col == nodes[i]->getCol()) {
            found = true;  // FIXME: Should Break as soon as it is found.
        }
    } return found;
}

NodePtr NodeList::getCdr(int row, int col){
    NodePtr node = nullptr;
    for (int i = 0; (i < this->length && this->length != 0); i++) {
        if (row == nodes[i]->getRow() && col == nodes[i]->getCol()) {
            node = nodes[i];
        }
    } return node;
}

void NodeList::clear(){
    // TODO: Update this to delete node on the heap.
    int arrayLen = this->length - 1;
    for (int i = 0; i <= arrayLen; i++) {
        nodes[i] = nullptr;
        (this->length)--;
    }
}


