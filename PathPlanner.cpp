#include "PathPlanner.h"
#include <iostream>


PathPlanner::PathPlanner(Env env, int rows, int cols){
   std::copy(&env[0][0], &env[0][0]+rows*cols,&this->env[0][0]);
   this->envRows = rows;
   this->envCols = cols;

   // Find and set Starting coords.
   PathPlanner::Coordinates start = this->getStartingPosition();
   this->initialPosition(start.Row, start.Col);

   // Find and set Goal coords.
   PathPlanner::Coordinates goal = this->getGoalPosition();
   this->goalPosition(goal.Row, goal.Col);
}

PathPlanner::~PathPlanner()= default;


void PathPlanner::initialPosition(int row, int col){
    this->startRow = row;
    this->startCol = col;
}

void PathPlanner::goalPosition(int row, int col){
    this->goalRow = row;
    this->goalCol = col;
}

PathPlanner::Coordinates PathPlanner::getStartingPosition(){
   int row;
   int col;
   bool found = false;
    for (int i=0; i<this->envRows && !found; i++) {
        for (int j = 0; j < this->envCols && !found; j++) {
            if (this->env[i][j] == 'S') {
                row = i;
                col = j;
                found = true;
                // TODO: Optimize.
            }
        }
    } return Coordinates {row, col};
}

PathPlanner::Coordinates PathPlanner::getGoalPosition() {
   int row;
   int col;
   bool found = false;
   for (int i = 0; i < this->envRows && !found; i++) {
      for (int j = 0; j < this->envCols && !found; j++) {
         if (this->env[i][j] == 'G') {
            row = i;
            col = j;
            found = true;
         }
      }
   } return Coordinates {row, col};
}

void PathPlanner::setClosedList(NodeList* cList){
   this->closedList = new NodeList(*cList);
}

NodeList* PathPlanner::getReachableNodes() {
   NodeList openList;
   Node start = Node(this->startRow, this->startCol, 0);
   openList.addBack(&start);
   Node *p = &start;

   NodeList closedList;

   bool empty = false;
   while (!empty) {
      //Initialize reachable array of size four.
      int reachable[4] = {0};
      // Check if nodes surrounding p is reachable.
      if (p->getRow() > 0 && this->env[p->getRow() - 1][p->getCol()] != '=') { reachable[0] = 1; } // Up
      if (p->getCol() < this->envCols - 1 && this->env[p->getRow()][p->getCol() + 1] != '=') { reachable[1] = 1; } // Right
      if (p->getRow() < this->envRows - 1 && this->env[p->getRow() + 1][p->getCol()] != '=') { reachable[2] = 1; } // Down
      if (p->getCol() > 0 && this->env[p->getRow()][p->getCol() - 1] != '=') { reachable[3] = 1; } // Left

      // Iterate through reachable array.
      for (int i = 0; i < 4; i++) {
         if (reachable[i] == 1) {
            int qRow = 0;
            int qCol = 0;
            // Set q attributes.
            if (i == 0) {
               qRow = p->getRow() - 1;
               qCol = p->getCol();
            } else if (i == 1) {
               qRow = p->getRow();
               qCol = p->getCol() + 1;
            } else if (i == 2) {
               qRow = p->getRow() + 1;
               qCol = p->getCol();
            } else if (i == 3) {
               qRow = p->getRow();
               qCol = p->getCol() - 1;
            }

            // Add q to open list if it is not in the closed list.
            Node q = Node(qRow, qCol, p->getDistanceToS() + 1);
            if (!closedList.containsNode(&q)) {
               openList.addBack(&q);
            }
         }
      }
      // Add p to closed list.
      closedList.addBack(p);
      // Select a node from the open-list that is NOT in the closed-list.
      bool pRefreshed = false;
      for (int i = 0; (i < openList.getLength() && !pRefreshed); i++) {
         Node *q = openList.get(i);
         if (!closedList.containsNode(q)) {
            p = q;
            pRefreshed = true;
         }
      }

      // If p is null, then the open-list is empty.
      if (!pRefreshed && closedList.getLength() > 0) {
         empty = true;
      }
   }

   // Set closed list.
   this->setClosedList(&closedList);
   // Return a copy of the closedList.
   return new NodeList(closedList);
}


NodeList* PathPlanner::getPath(){
   // Create path list.
   NodeList path;
   // Get the goal node.
   NodePtr p = this->closedList->getCdr(this->goalRow, this->goalCol);
   // Add the goal node to the path list.
   path.addBack(p);

   bool empty = false;
   while (!empty) {
      // Initialize exist array of size four.
      int exist[4] = {0};
      
      // Check if surrounding nodes is in the closed list.
      if (p->getRow() > 0 && this->closedList->containsCdr(p->getRow() - 1, p->getCol())) { exist[0] = 1; }  // Up
      if (p->getCol() < this->envCols - 1 && this->closedList->containsCdr(p->getRow(), p->getCol() + 1)) { exist[1] = 1; }  // Right
      if (p->getRow() < this->envRows - 1 && this->closedList->containsCdr(p->getRow() + 1, p->getCol())) { exist[2] = 1; }  // Down
      if (p->getCol() > 0 && this->closedList->containsCdr(p->getRow(), p->getCol() - 1)) { exist[3] = 1; }  // Left

      // Initialize valid nodes array.
      NodePtr validNode[4] = {nullptr, nullptr, nullptr, nullptr};

      // Iterate through exist array.
      for (int i = 0; i < 4; i++) {
         if (exist[i] == 1) {
            int qRow = 0;
            int qCol = 0;
            // Set q attributes.
            if (i == 0) {
               qRow = p->getRow() - 1;
               qCol = p->getCol();
            } else if (i == 1) {
               qRow = p->getRow();
               qCol = p->getCol() + 1;
            } else if (i == 2) {
               qRow = p->getRow() + 1;
               qCol = p->getCol();
            } else if (i == 3) {
               qRow = p->getRow();
               qCol = p->getCol() - 1;
            }
            // Get node pointer of q.
            NodePtr q = this->closedList->getCdr(qRow, qCol);
            // Add q to valid nodes array.
            validNode[i] = q;
            }
         }
         // Iterate through valid nodes array.
         for (int i = 0; i < 4; i++) {
            if ((validNode[i] != nullptr)
                && (validNode[i]->getDistanceToS() == p->getDistanceToS() - 1)
                && (!path.containsNode(validNode[i]))) {
               // Add to path list.
               path.addBack(validNode[i]);
               // Set p to valid node.
               p = validNode[i];
            }
         }
         // If p is one away from start node, then the path is complete.
         if (p->getDistanceToS() == 1) {
            empty = true;
         }
   } return new NodeList(path);
}
