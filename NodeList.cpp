
#include "NodeList.h"
#include <iostream>



NodeList::NodeList(){
   this->length = 0;
}


NodeList::~NodeList(){
   std::cout << "\n" << "NodeList Destroyed" << "\n";
}

NodeList::NodeList(NodeList& other){
    // TODO
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
    // Append nodePtr to nodeList.
    nodes[this->length - 1] = newNode;
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
    for (int i = 0; i <= this->length; i++) {
        if (node == nodes[i]) {
            found = true;  // FIXME: Should Break as soon as it is found.
        }
    } return found;
}

void NodeList::clear(){
    int arrayLen = this->length - 1;
    for (int i = 0; i <= arrayLen; i++) {
        nodes[i] = nullptr;
        (this->length)--;
    }
}


