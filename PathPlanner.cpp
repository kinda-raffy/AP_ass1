#include "PathPlanner.h"
#include <iostream>


PathPlanner::PathPlanner(Env env, int rows, int cols) {
   this->env = env;
   this->envRows = rows;
   this->envCols = cols;
   // Find and set Starting coords.
   NodePtr initialNode = this->getInitialPosition();
   this->initialPosition(initialNode->getRow(), initialNode->getCol());
   delete initialNode;
   initialNode = nullptr;
   // Find and set Goal coords.
   NodePtr goalNode = this->getGoalPosition();
   this->goalPosition(goalNode->getRow(), goalNode->getCol());
   delete goalNode;
   goalNode = nullptr;
}

PathPlanner::~PathPlanner() {
   delete this->closedList;
   this->closedList = nullptr;
}

/*
 * Finds the initial position of the robot.
 * @return NodePtr - pointer to the initial position.
 */
NodePtr PathPlanner::getInitialPosition() {
   NodePtr start = nullptr;
   bool found = false;
   for (int i = 0; i < this->envRows && !found; i++) {
      for (int j = 0; j < this->envCols && !found; j++) {
         if (this->env[i][j] == SYMBOL_START) {
            start = new Node(i, j, 0);
            found = true;
         }
      }
   }
   if (!found) { throw std::runtime_error("No start location (S) found within the given .env file."); }
   return start;
}

/*
 * Sets initial position of the robot.
 * @param int row - row of the initial position.
 * @param int col - column of the initial position.
 */
void PathPlanner::initialPosition(int row, int col) {
   this->startRow = row;
   this->startCol = col;
}

/*
 * Finds the goal position of the map.
 * @return NodePtr - pointer to the goal position.
 */
NodePtr PathPlanner::getGoalPosition() {
   NodePtr goalNode = nullptr;
   bool found = false;
   for (int i = 0; i < this->envRows && !found; i++) {
      for (int j = 0; j < this->envCols && !found; j++) {
         if (this->env[i][j] == SYMBOL_GOAL) {
            goalNode = new Node(i, j, -1);
            found = true;
         }
      }
   }
   if (!found) { throw std::runtime_error("No endpoint (G) found within the .env file."); }
   return goalNode;
}

/*
 * Sets goal position of the map.
 * @param int row - row of the goal position.
 * @param int col - column of the goal position.
 */
void PathPlanner::goalPosition(int row, int col) {
   this->goalRow = row;
   this->goalCol = col;
}

/*
 * Finds all reachable positions in the map.
 * @return NodeList* - Deep copy of the cList.
 */
NodeList* PathPlanner::getReachableNodes() {
   // Create openList.
   auto* oList = new NodeList();
   // Create the start node.
   Node start = Node(this->startRow, this->startCol, 0);
   // Add start node to openList.
   oList->addBack(&start);
   // Create a pointer to the start node called p.
   NodePtr p = &start;
   // Create the closedList .
   auto* cList = new NodeList();

   // While openList is not empty (aka. p is not refreshed).
   bool empty = false;
   while (!empty) {
      // Initialize reachable array of size four.
      int reachable[4] = {0};
      // Check if nodes surrounding p is reachable.
      // Up.
      if (p->getRow() > 0 && this->env[p->getRow() - 1][p->getCol()] != SYMBOL_WALL) { reachable[0] = 1; }
      // Right.
      if (p->getCol() < this->envCols - 1 && this->env[p->getRow()][p->getCol() + 1] != SYMBOL_WALL) {
         reachable[1] = 1;
      }
      // Down.
      if (p->getRow() < this->envRows - 1 && this->env[p->getRow() + 1][p->getCol()] != SYMBOL_WALL) {
         reachable[2] = 1;
      }
      // Left.
      if (p->getCol() > 0 && this->env[p->getRow()][p->getCol() - 1] != SYMBOL_WALL) { reachable[3] = 1; }

      // Iterate through reachable array.
      for (int i = 0; i < 4; i++) {
         // If the side is reachable.
         if (reachable[i] == 1) {
            // Initialize q attributes.
            int qRow = 0;
            int qCol = 0;
            // Set q attributes depending on the side.
            if (i == 0) {
               // Up
               qRow = p->getRow() - 1;
               qCol = p->getCol();
            } else if (i == 1) {
               // Right
               qRow = p->getRow();
               qCol = p->getCol() + 1;
            } else if (i == 2) {
               // Down
               qRow = p->getRow() + 1;
               qCol = p->getCol();
            } else if (i == 3) {
               // Left
               qRow = p->getRow();
               qCol = p->getCol() - 1;
            }
            // Add q to open list if it is not in the closed list.
            Node q = Node(qRow, qCol, p->getDistanceToS() + 1);
            if (!cList->containsNode(&q)) {
               oList->addBack(&q);
            }
         }
      }
      // Add p to the closed list.
      cList->addBack(p);
      // Select a node from the open-list that is not in the closed-list.
      bool pRefreshed = false;
      for (int i = 0; (i < oList->getLength() && !pRefreshed); i++) {
         Node *q = oList->get(i);
         if (!cList->containsNode(q)) {
            p = q;
            pRefreshed = true;
         }
      }
      // If p has not been refreshed, then the open-list is empty; and the while loop is broken.
      if (!pRefreshed) { empty = true; }
   }
   // Bound the closed list to the object.
   this->closedList = new NodeList(*cList);
   // Memory clean up.
   delete oList;
   oList = nullptr;
   delete cList;
   cList = nullptr;
   // Return a deep copy of the cList.
   return new NodeList(*this->closedList);
}

/*
 * Validates node if they fit path requirements.
 * @return bool - Node fits path requirements or not.
 */
bool PathPlanner::validPathNode(NodePtr i, NodePtr p, NodeList* pathList) {
   return (i != nullptr) && (i->getDistanceToS() == p->getDistanceToS() - 1) && (!pathList->containsNode(i)) &&
           i->getDistanceToS() != 0;
}

/*
 * Finds the path from the start to the goal.
 * @return NodeList* - Deep copy of the pathList.
 */
NodeList* PathPlanner::getPath() {
   // Create the pathList list.
   auto* pathList = new NodeList();
   NodePtr p = nullptr;
   // Grab a pointer to the goal node.
   if (this->closedList != nullptr) {
      p = this->closedList->getCdr(this->goalRow, this->goalCol);
   } else {
      throw std::runtime_error("Closed list is NULL. Run getReachableNodes() before calling getPath().");
   }
   // Add the goal node to the pathList list.
   pathList->addBack(p);

   // Iterate until p is one node away from the start node.
   bool empty = false;
   while (!empty) {
      // Initialize existNodes array of size four.
      int existNodes[4] = {0};
      // Check if surrounding nodes is in the closed list.
      // Up
      if (p->getRow() > 0 && this->closedList->containsCdr(p->getRow() - 1, p->getCol())) { existNodes[0] = 1; }
      // Right
      if (p->getCol() < this->envCols - 1 && this->closedList->containsCdr(p->getRow(), p->getCol() + 1)) {
         existNodes[1] = 1;
      }
      // Down
      if (p->getRow() < this->envRows - 1 && this->closedList->containsCdr(p->getRow() + 1, p->getCol())) {
         existNodes[2] = 1;
      }
      // Left
      if (p->getCol() > 0 && this->closedList->containsCdr(p->getRow(), p->getCol() - 1)) { existNodes[3] = 1; }

      // Initialize possiblePathNode array.
      NodePtr possiblePathNode[4] = {nullptr, nullptr, nullptr, nullptr};
      // Iterate over the existNodes array.
      for (int i = 0; i < 4; i++) {
         // If surrounding node exists; then add it to the possiblePathNode array.
         if (existNodes[i] == 1) {
            // Initialize q attributes.
            int qRow = 0;
            int qCol = 0;
            // Set q attributes.
            if (i == 0) {
               // Up
               qRow = p->getRow() - 1;
               qCol = p->getCol();
            } else if (i == 1) {
               // Right
               qRow = p->getRow();
               qCol = p->getCol() + 1;
            } else if (i == 2) {
               // Down
               qRow = p->getRow() + 1;
               qCol = p->getCol();
            } else if (i == 3) {
               // Left
               qRow = p->getRow();
               qCol = p->getCol() - 1;
            }
            // Get node pointer of q.
            NodePtr q = this->closedList->getCdr(qRow, qCol);
            // Add q to possiblePathNode array.
            possiblePathNode[i] = q;
            }
         }
         // Iterate through possiblePathNode array.
         for (auto & i : possiblePathNode) {
            if (this->validPathNode(i, p, pathList)) {
               // Add to pathList list.
               pathList->addBack(i);
               // Set p to valid node.
               p = i;
            }
         }
         // If p is one away from start node, or is the start node; then the pathList is complete.
         if (p->getDistanceToS() <= 1) {
            empty = true;
         }
   }
   auto* returnNodeList = new NodeList(*pathList);
   delete pathList;
   pathList = nullptr;
   return returnNodeList;
}
