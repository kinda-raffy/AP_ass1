#include "NodeList.h"
#include <iostream>


NodeList::NodeList() {
   this->length = 0;
   this->nodes = new Node*[20];
}

NodeList::~NodeList() { this->clear(); }

NodeList::NodeList(NodeList& other) :
   length(other.length)
{
    // Array size constants.
    // Default length of the array.
    const int defaultNodeListLength = 20;
    // Factor of 10 required to get the defaultNodeListLength as a product. Ensures a very tight fit.
    const int defaultArrayFactor = 2;
    // Digit columns to remove (0.1 removes the one's column).
    const float subtractColumnFactor = 0.1f;
   // Factor to add to the length of the array to ensure that it is not too small.
    const int safeGuardFactor = 2;

    // Calculate required length of copy array.
    // Assigned length must be a factor of the defaultNodeListLength.
    int len = ((int(other.length * subtractColumnFactor) + safeGuardFactor) / defaultArrayFactor) * defaultNodeListLength;

    // If len is 0, then switch len to 1
    if (len == 0) {
        len = 1;
    } else if ((len % 20 == 10) > 0) {  // If len is a multiple of 10 and not 20, then add 10 to it.
        len += 10;
    }

    this->nodes = new Node*[len];
    // Deep copy of nodes array.
    for (int i = 0; i < this->length; i++) {
       this->nodes[i] = new Node(other.nodes[i]->getRow(),
                                 other.nodes[i]->getCol(),
                                 other.nodes[i]->getDistanceToS());
    }
}

/*
 * Get the length of the NodeList.
 * @return int - The length of the NodeList.
 */
int NodeList::getLength(){
   return this->length;
}

/*
 * Get the ith Node in the NodeList.
 * @param node* - Node at ith position.
 */
NodePtr NodeList::get(int i){
    // Check if index is within bounds.
   if (i > this->length || i < 0) {
       throw std::out_of_range("Given NodeList index is out of bounds.");
   }
   return nodes[i];
}

/*
 * Increase the size of the NodeList.
 */
void NodeList::resizeNodeList() {
   // Resize the array.
   Node **newNodes = new Node *[this->length + DEFAULT_ARRAY_LEN];
   for (int i = 0; i < this->length; i++) {
      // Transfer node pointers in this->nodes into newNodes array.
      newNodes[i] = this->nodes[i];
   }
   // Delete old array.
   delete[] this->nodes;
   // Assign new array to old array.
   this->nodes = newNodes;
}

/*
 * Add a Node to the end of the NodeList.
 * @param node* - Node to be added.
 */
void NodeList::addBack(NodePtr newNode) {
   // Check if array needs to be resized.
   if ((this->length % 20 == 0) && (this->length != 0)) { this->resizeNodeList(); }
   // Increase count.
   (this->length)++;
   // Add a COPY node element to the BACK of the nodelist
   nodes[this->length - 1] = new Node(newNode->getRow(), newNode->getCol(), newNode->getDistanceToS());
}

/*
 * Checks whether a node is within the list.
 * @param bool - Whether the node exists or not.
 */
bool NodeList::containsNode(NodePtr node){
    bool found = false;
    for (int i = 0; (i < this->length && !found && this->length != 0); i++) {
        if (node->getRow() == nodes[i]->getRow() && node->getCol() == nodes[i]->getCol()) {
            found = true;
        }
    } return found;
}

/*
 * Checks whether a node with row, col exists in the NodeList.
 * @param int row - Row of the node.
 * @param int col - Column of the node.
 * @return bool - Whether the node with row, col exists or not.
 */
bool NodeList::containsCdr(int row, int col){
    bool found = false;
    for (int i = 0; (i < this->length && !found && this->length != 0); i++) {
        if (row == nodes[i]->getRow() && col == nodes[i]->getCol()) {
            found = true;
        }
    } return found;
}

/*
 * Get a node with a given row, col.
 * @param int row - Row of the node.
 * @param int col - Column of the node.
 * @return node* - Node with row, col.
 */
NodePtr NodeList::getCdr(int row, int col){
    NodePtr node = nullptr;
    for (int i = 0; (i < this->length && this->length != 0); i++) {
        if (row == nodes[i]->getRow() && col == nodes[i]->getCol()) {
            node = nodes[i];
        }
    } return node;
}

/*
 * Delete NodeList.
 */
void NodeList::clear(){
   // Delete node in nodes array.
   const int len = this->length;
   for (int i = 0; i < len; i++) {
      delete this->nodes[i];
       (this->length)--;
    }
   // Delete node array.
   delete[] this->nodes;
   this->nodes = nullptr;
}


