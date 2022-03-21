
#include "PathPlanner.h"

#include <iostream>



PathPlanner::PathPlanner(Env env, int rows, int cols){
   std::copy(&env[0][0], &env[0][0]+rows*cols,&this->env[0][0]);
   this->rows = rows;
   this->cols = cols;

   // Find Starting coords.
   PathPlanner::startingCoord coord = this->getStartingPosition();
   this->initialPosition(coord.startingRow, coord.startingCol);

   // Find Reachable Positions.
   this->getReachableNodes()

}

PathPlanner::~PathPlanner(){
   std::cout << "PathPlanner Destroyed.";
}


void PathPlanner::initialPosition(int row, int col){
    this->startingRow = row;
    this->startingCol = col;
}

PathPlanner::startingCoord PathPlanner::getStartingPosition(){
    int row;
    int col;
    for (int i=0; i<this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            if (this->env[i][j] == 'S') {
                row = i;
                col = j;
                // TODO: Optimize.
            }
        }
    }
    return startingCoord {row, col};
}

NodeList* PathPlanner::getReachableNodes(){
    NodeList openList;
    Node start = Node(this->startingRow, this->startingCol, 0);
    openList.addBack(&start);

    NodeList closedList;
    Node* p = &start;



}

NodeList* PathPlanner::getPath(){
   
}
