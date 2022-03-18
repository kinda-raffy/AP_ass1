
#include "PathPlanner.h"

#include <iostream>


PathPlanner::PathPlanner(Env env, int rows, int cols){
   std::copy(&env[0][0], &env[0][0]+rows*cols,&this->env[0][0]);
   this->rows = rows;
   this->cols = cols;

   this->findInitialPosition();
}

PathPlanner::~PathPlanner(){
   std::cout << "PathPlanner Destroyed.";
}

void PathPlanner::initialPosition(int row, int col){

}

void PathPlanner::findInitialPosition() {
    for (int i=0; i<this->rows; i++) {
        for (int j=0; j<this->cols; j++) {
            if (this->env[i][j] == 'S') {
                this->startingCoordinates[0] = i;
                this->startingCoordinates[1] = j;

                std::cout << "Starting position found at: " <<
                this->startingCoordinates[0] << ", " <<
                this->startingCoordinates[1] << std::endl;

            }
        }
    }

}

NodeList* PathPlanner::getReachableNodes(){

}

NodeList* PathPlanner::getPath(){
   
}
