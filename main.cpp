/* ---- CODE DESCRIPTION----
 * Approach taken:
 *    In this assignment, I have attempted to implement M1, M2, M3 and M4.
 *    1. This was done via dynamically loading in an environment of any size via readEnvStdin().
 *       In resizing the environment, I have used the updateEnv() function which allocates more memory to the env
 *       while simultaneously ensuring memory is properly handled and deleted.
 *    2. Next a PathPlanner object is created. The PathPlanner class is responsible for finding nodes that contribute
 *       to path creation.
 *    3. The getReachableNodes() method is called next. Implementation at this stage is simple and corresponds to the
 *       pseudocode in M2.
 *    4. The getPath() method is then called. Despite its similarities to getReachableNodes(), getPath() begins from the
 *       goal in the provided closed list. It then backtracks to the start node via a series of steps including
 *       possiblePathNode validation. Briefly, if a given node in the closed list is considered valid, it is added to a
 *       NodeList which is then returned when the start node is reached.
 *    5. The path is then printed to stdout via printPath(). To print the path, the solution NodeList is iterated through;
 *       and a representative char ('<', '^', for ex.) is placed at its appropriate location in the environment. The
 *       2D array is then printed to stdout.
 *    6. Finally, memory is appropriately deleted with their respective pointer set to nullptr (in case the code is
 *       expanded on in the future, this will prevent illegal read and writes).
 *    7. The program is then terminated.
 *
 * Issues faced:
 *   Throughout this assignment, many issues were encountered. As the spec sheet emphasised on "briefly", I will be
 *   going into details with two notable issues and will be listing the rest.
 *
 *   The first challenge was memory management. In particular, memory leaks that I did not realise existed.
 *   The leaks occurred due to a variable iterator being decreased in size when a node was deleted from the node list;
 *   resulting in half the list not being deleted. Now this might sound quite obvious but this was really hard to find.
 *   As a matter of fact, if I had not used Valgrind MemCheck, I would have not noticed this leak at all. The issue
 *   however, was quickly resolved by using a const variable. The second issue I faced was printing the environment.
 *   This seemed obvious to me at first: iterate through the environment, if a node exist at the current location, add
 *   the direction char to the buffer, else add the env char. The buffer is then cleared at the end of the row, thus
 *   printing the row. However, this only works if the (S)tart node is of left to the (G)oal. As the loop iterates left
 *   to right, the nodes on the left is always detected before the nodes on the right. This resulted in the directional
 *   checks (to determine which direction the arrows should face) to point in the opposite direction, as it always saw a
 *   valid node in other directions before seeing the next node it is supposed to point at. This problem was solved by
 *   iterating through the solution list and following the path to ensure every node is printed in the correct direction.
 *   My current solution works for all cases as it keeps track of the last node's direction, preventing it from
 *   backtracking and printing in the opposite direction.
 *
 *   Other issues I faced were:
 *   1. The program was not able to handle the case where the start and goal were next to each other (no path needs to
 *   be drawn). This was solved by checking for the start node and preventing it from being added to the solution list.
 *   2. The program was making invalid reads when checking for END_OF_ARRAY (as the detected by Valgrind). This was
 *   solved by allocating additional memory to the environment and ensuring that the last element is always set to
 *   END_OF_ARRAY.
 */


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Types.h"
#include "Node.h"
#include "NodeList.h"
#include "PathPlanner.h"

// Helper test functions
void testNode();
void testNodeList();

// Make a dynamic 2D Environment.
Env makeEnv(int rows, int cols);
// Delete a dynamic 2D Environment.
void deleteEnv(Env env, int rows, int cols);

// Read an environment from standard input.
Env readEnvStdin();

// Print out an Environment to standard output with path.
// To be implemented for Milestone 3
void printPath(Env env, NodeList* solution);

// Print reachable nodes from a node to standard output. FOR TESTING.
void printReachablePositions(Env env, NodeList* reachablePositions);


int main(int argc, char** argv) {
   // Load Environment from standard input.
   Env env = readEnvStdin();

   // Find number of rows in env.
   int envRows = 0;
   while(env[envRows] != nullptr) { envRows++; }
   // Find number of cols in env.
   int envCols = 0;
   while(env[0][envCols] != '\0') { envCols++; }

   // Solve using forwardSearch.
   auto* pathPlanner = new PathPlanner(env, envRows, envCols);
   NodeList* reachablePositions = nullptr;
   reachablePositions = pathPlanner->getReachableNodes();

   // Get the path.
   NodeList* solution = pathPlanner->getPath();

   // Print the path.
   printPath(env, solution);

   // Clean up.
   delete pathPlanner;
   pathPlanner = nullptr;
   delete reachablePositions;
   reachablePositions = nullptr;
   delete solution;
   solution = nullptr;
   deleteEnv(env, envRows, envCols);
}

/*
 * Updates the new env with elements from the old env.
 * @param oldEnv The old env.
 * @param newEnv The new env.
 * @param oldRows The number of rows in the old env.
 * @param oldCols The number of cols in the old env.
 * @return The new updated env.
 */
Env updateEnv(Env oldEnv, Env newEnv, int oldRow, int oldCol) {
   if (oldEnv != nullptr && newEnv != nullptr) {
      // Copy the previous env into newEnv.
      for (int i = 0; i < oldRow; i++) {
         for (int j = 0; j < oldCol; j++) {
            newEnv[i][j] = oldEnv[i][j];
         }
      }
      deleteEnv(oldEnv, oldRow, oldCol);
   } else {
      std::cout << "Error: updateEnv() - nullptr passed in." << std::endl;
   } return newEnv;
}

/*
 * Read and load in an environment from standard input.
 * @return The loaded environment.
 */
Env readEnvStdin(){
   std::string line = "";
   int row = 0;
   int col = 0;
   // Declare environment.
   Env env = nullptr;
   // For every line received from stdin, increase allocated memory by one row.
   while(std::getline(std::cin, line)){
      // Grab the number of columns required.
      col = line.length();
      // If not first row, updateEnv; else create new env.
      if (row != 0) {
         // Update env to have one more row.
         env = updateEnv(env, makeEnv(row + 1, col + END_OF_ROW), row, col + END_OF_ROW);
      } else {
         // Create the first env.
         env = makeEnv(row + 1, col + END_OF_ROW);
      }

      // For every character in the line, set the corresponding element in the env.
      if (env != nullptr) {
         for (int i = 0; i < col; i++) {
            env[row][i] = line[i];
         }
         // Add null terminator to the end of the line.
         env[row][col] = '\0';
      } row++;
    } return env;
}

/*
 * Make a 2D environment dynamically.
 * @param rows The number of rows in the environment.
 * @param cols The number of cols in the environment.
 * @return The created environment.
 */
Env makeEnv(const int rows, const int cols) {
   Env env = nullptr;
   if (rows >= 0 && cols >= 0) {
      env = new char*[rows + END_OF_ARRAY] {};
      for (int i = 0; i != rows; ++i) {
         env[i] = new char[cols + END_OF_ARRAY] {};
      }
      // Indicate the end of the array via nullptr.
      env[rows] = nullptr;
   }
   return env;
}

/*
 * Delete a 2D Environment dynamically.
 * @param env - the environment to be deleted.
 * @param rows - the number of rows in the environment.
 * @param cols - the number of columns in the environment.
 */
void deleteEnv(Env env, int rows, int cols) {
   if (rows >= 0 && cols + END_OF_ARRAY >= 0) {
      for (int i = 0; i != rows; ++i) {
         delete[] env[i];
      }
      // Delete nullptr.
      delete env[rows];
      // Delete the array.
      delete[] env;
      env = nullptr;
   }
}

/*
 * Print the path to standard output.
 * @param env - the environment to be printed.
 * @param path - the path to be printed over the env.
 */
void printPath(Env env, NodeList* solution) {
   // If solution list is greater than 1 node, a path is required to be printed.
   if (solution->getLength() > 1) {
      // Iterate through the solution and print out the path.
      std::string previousDirection = "";
      // Solution list is reversed.
      for (int i = solution->getLength() - 1; i >= 0; i--) {
         NodePtr p = solution->get(i);
         // Replace element with arrow if the next node is up, right, down or left.
         if (solution->containsCdr(p->getRow() - 1, p->getCol()) && previousDirection != "down") {
            env[p->getRow()][p->getCol()] = '^';
            previousDirection = "up";
         } else if (solution->containsCdr(p->getRow(), p->getCol() + 1) && previousDirection != "left") {
            env[p->getRow()][p->getCol()] = '>';
            previousDirection = "right";
         } else if (solution->containsCdr(p->getRow() + 1, p->getCol()) && previousDirection != "up") {
            env[p->getRow()][p->getCol()] = 'v';
            previousDirection = "down";
         } else if (solution->containsCdr(p->getRow(), p->getCol() - 1) && previousDirection != "right") {
            env[p->getRow()][p->getCol()] = '<';
            previousDirection = "left";
         }
      }
   }

   // Print out every row in the environment as a c-style string.
   int i = 0;
   while (env[i] != nullptr) {
      std::cout << env[i++] << std::endl;
   }
}

/*
 * LEFT IN FOR MARKING PURPOSES.
 * Print all reachable nodes  to standard output.
 * @param env - the environment to be printed.
 * @param path - the path to be printed over the env.
 */
void printReachablePositions(Env env, NodeList* reachablePositions){
   // Find number of rows in env.
   int envRows = 0;
   while(env[envRows] != nullptr) { envRows++; }
   // Find number of cols in env.
   int envCols = 0;
   while(env[0][envCols] != '\0') { envCols++; }
    // Layer reachablePositions over the environment.
    for (int i = 0; i < envRows; i++) {
        for (int j = 0; j < envCols; j++) {
            if (reachablePositions->containsCdr(i, j)) {
                std::cout << "X";
            } else {
                std::cout << env[i][j];
            }
        }
        std::cout << std::endl;
    }
}

/*
 * LEFT IN FOR MARKING PURPOSES.
 * Test node.
 */
void testNode() {
    std::cout << "TESTING Node" << std::endl;

    // Make a Node and print out the contents
    Node* node = new Node(1, 1, 2);
    std::cout << node->getRow() << ",";
    std::cout << node->getCol() << ",";
    std::cout << node->getDistanceToS() << std::endl;
    delete node;
    node = nullptr;

    // Change Node and print again
    node = new Node(4, 2, 3);
    std::cout << node->getRow() << ",";
    std::cout << node->getCol() << ",";
    std::cout << node->getDistanceToS() << std::endl;
    delete node;
    node = nullptr;
}

/*
 * LEFT IN FOR MARKING PURPOSES.
 * Test node list.
 */
void testNodeList() {
    std::cout << "TESTING NodeList" << std::endl;

    // Make a simple NodeList, should be empty size
    NodeList* nodeList = new NodeList();
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Add a Node to the NodeList, print size
    Node* b1 = new Node(1, 1, 1);
    nodeList->addBack(b1);
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Add second Nodetest
    Node* b2 = new Node(0, 0, 1);
    nodeList->addBack(b2);
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Test Get-ith - should be 0,0,1
    Node* getB = nodeList->get(1);
    std::cout << getB->getRow() << ",";
    std::cout << getB->getCol() << ",";
    std::cout << getB->getDistanceToS() << std::endl;

    // Print out the NodeList
    std::cout << "PRINTING OUT A NODELIST IS AN EXERCISE FOR YOU TO DO" << std::endl;
}

/*
 * LEFT IN FOR MARKING PURPOSES.
 * readEnvStdin for M3.
 * @param Environment to be loaded.
 */
//void readEnvStdin(Env env){
//    for (int i = 0; i<=ENV_DIM - 1; i++) {
//        for (int j = 0; j<=ENV_DIM - 1; j++) {
//            std::cin >> env[i][j];
//        }
//    }
//}