
// This hw assignment
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <stack>
#include "node.hpp"
using namespace std;

// Global output file
ofstream outputFile;

void read_file_func(ifstream& file, State& st);
void iddfs(State initial, State goal);
void bfs_dfs(State initial, State goal, int flag);
void aStar(State initial, State goal);

//Prints the Path found in screen and output file
void printSolution(Node* node){
    int whileCount = 0;
    while(node != NULL){
        //Print Path on the Console
        printf("L[%d,%d,%d]  -  [%d,%d,%d]R\n", node->currentState.leftB.tot_chick, node->currentState.leftB.tot_wolf, node->currentState.leftB.boat, node->currentState.rightB.tot_chick, node->currentState.rightB.tot_wolf, node->currentState.rightB.boat);
        //Print Path in the OutputFile
        outputFile << "L[" << node->currentState.leftB.tot_chick << "," <<  node->currentState.leftB.tot_wolf << "," << node->currentState.leftB.boat << "]  -  [" << node->currentState.rightB.tot_chick << "," << node->currentState.rightB.tot_wolf << "," << node->currentState.rightB.boat << "]R" << endl;

        //Move to Parent Node
        node = node->parent;
        whileCount++;
    }
    cout << "---START PATH---" << endl;
    cout << "Number of Solution Path Nodes: " << whileCount << endl;
    outputFile << "---START PATH---" << endl;
    outputFile << "Number of Solution Path Nodes: " << whileCount << endl;
}


//Read file and parse data in to structure
void read_file_func(ifstream& file, State& st) {
    char trash, boatL, boatR;
    file >> st.leftB.tot_chick >> trash >> st.leftB.tot_wolf >> trash >> boatL;
    file >> st.rightB.tot_chick >> trash >> st.rightB.tot_wolf >> trash >> boatR;
    if (boatL == '0') {
        st.leftB.boat = false;
        st.rightB.boat = true;
    }
    else {
        st.leftB.boat = true;
        st.rightB.boat = false;
    }
}

//Checks if initial state and goal state are equal
bool stateChecker(State initial, State goal) {
    if (initial.leftB.tot_chick == goal.leftB.tot_chick && initial.rightB.tot_chick == goal.rightB.tot_chick
        && initial.leftB.tot_wolf == goal.leftB.tot_wolf && initial.rightB.tot_wolf == goal.rightB.tot_wolf
        && initial.rightB.boat == goal.rightB.boat && initial.leftB.boat == goal.leftB.boat) {
        return true;
    }
    else {
        return false;
    }
}

//Heuristic Function
//Estime cost of the cheapest path from node n to goal node
int heuristicAstar(State initial, State goal) {
    int x = 0, y = 0, z = 0, totalCost = 0;
    x = abs(initial.leftB.boat - goal.leftB.boat) + x;
    x = abs(initial.rightB.boat - goal.rightB.boat) + x;
    y = abs(initial.leftB.tot_wolf - goal.leftB.tot_wolf) + y;
    y = abs(initial.leftB.tot_chick - goal.leftB.tot_chick) + y;
    z = abs(initial.rightB.tot_wolf - goal.rightB.tot_wolf) + z;
    z = abs(initial.rightB.tot_chick - goal.rightB.tot_chick) + z;
    totalCost = y * z + x;

    return (totalCost/2);
}

//Main
int main(int argc, char* argv[])
{
    //Check for correct input
    //If not correct print error message
    if (argc != 5) {
        cout << "Wrong input. Please Try Again." << endl;
        cout << "Code example: <initial state file> <goal state file> <mode> <output file>" << endl;
    }
    else {
        //Commandline Arguments
        ifstream initialStateFile(argv[1]);
        ifstream goalStateFile(argv[2]);
        outputFile.open(argv[4]);

        //Initilize States (Initial/Goal)
        State initialState;
        State goalState;

        string line;
        //Read the opened file
        if (initialStateFile.is_open() && goalStateFile.is_open())
        {
            read_file_func(initialStateFile, initialState);
            read_file_func(goalStateFile, goalState);
            initialStateFile.close();
            goalStateFile.close();

        }
        else {
            //Print Error Message on the Screen
            cout << "An ERROR has occured, provided files are inccorect!" << endl;
            return 1;
        }

        //Call the specified mode from argv[3]
        if (strcmp(argv[3], "bfs") == 0) {
            cout << "Breadth-First Search" << endl;
            outputFile << "Breadth-First Search" << endl;
            bfs_dfs(initialState, goalState, 1);
        }
        else if (strcmp(argv[3], "dfs") == 0) {
            cout << "Depth-First Search" << endl;
            outputFile << "Depth-First Search" << endl;
            bfs_dfs(initialState, goalState, 2);
        }
        else if (strcmp(argv[3], "iddfs") == 0) {
            cout << "Iterative Deepening Depth-first search" << endl;
            outputFile << "Iterative Deepening Depth-first search" << endl;
            iddfs(initialState, goalState);
        }
        else if (strcmp(argv[3], "astar") == 0) {
            cout << "A-Star Search" << endl;
            outputFile << "A-Star Search" << endl;
            aStar(initialState, goalState);
        }

    }
    //Close Output File
    outputFile.close();
    return 0;
}

//===============================NODE CREATION - START================================================

//Creates new nodes for bfs state
void childNodes(Node*& parent, Node*& newChild, State newState, vector<Node*>& visited, queue<Node*>& qList) {
    newChild = new Node();
    newChild->currentState = newState;
    newChild->parent = parent;
    visited.push_back(newChild);
    qList.push(newChild);
}
//Creates new nodes for dfs and iddfs algorithms
void dfs_childNodes(Node*& parent, Node*& newChild, State newState, vector<Node*>& visited, stack<Node*>& vPath, bool iddfs) {
    newChild = new Node();
    newChild->currentState = newState;
    newChild->parent = parent;
    if (iddfs == true) {
        newChild->depth = parent->depth + 1;
    }
    visited.push_back(newChild);
    vPath.push(newChild);
}

//Creates new nodes for bfs state
void astar_childNode(Node*& parent, Node*& newChild, State newState,State goalState ,vector<Node*>& visited, priority_queue<Node*>& qList) {
    newChild = new Node();
    newChild->currentState = newState;
    newChild->parent = parent;

    Node* newNode = new Node();
    newNode->priority=heuristicAstar(newState, goalState);
    newNode = newChild;
    visited.push_back(newChild);
    qList.push(newNode);
}

//===============================NODE CREATION - END================================================


////==============================VALIDATORS - START================================================

// Iterete thrue the visited list
//Source: https://www.geeksforgeeks.org/iterators-c-stl/
bool visitedChecker(State state, vector<Node*> list) {
    vector<Node*>::iterator ptr;

    // Search vector elements using begin() and end()
    for (ptr = list.begin(); ptr != list.end(); ptr++) {
        if (stateChecker((*ptr)->currentState, state)) {
            return true;
        }
    }
    return false;
}

//Action Checker
//Checks if there is enough animal to move
//Checks If there are chickens present on a bank,
//Check if there are more wolves than chickens
//If more wolve than the wolves will eat the chickens.
bool actionChecker(vector<Node*> list, State state, State* newNode, int numChick, int numWolv) {
    //Check if the animal count more chicken than wolves
    if ((((int)state.rightB.tot_chick >= (int)state.rightB.tot_wolf) || ((int)state.rightB.tot_chick == 0))
        && (((int)state.leftB.tot_chick >= (int)state.leftB.tot_wolf) || (int)state.leftB.tot_chick == 0)) {
        //Check if it is visited
        if (state.rightB.boat == true) {
            //Check if enough animals exist to move
            if (state.rightB.tot_chick >= numChick) {
                newNode->rightB.tot_chick = state.rightB.tot_chick - numChick;
                newNode->leftB.tot_chick = state.leftB.tot_chick + numChick;
            }
            else {
                return false;
            }
            //Check if enough animals exist to move
            if (state.rightB.tot_wolf >= numWolv) {
                newNode->rightB.tot_wolf = state.rightB.tot_wolf - numWolv;
                newNode->leftB.tot_wolf = state.leftB.tot_wolf + numWolv;
            }
            else {
                //cout << "Wrong action" << endl;
                return false;
            }
            //check if changes were valid
            if ((((int)newNode->rightB.tot_chick >= (int)newNode->rightB.tot_wolf) || ((int)newNode->rightB.tot_chick == 0))
                && (((int)newNode->leftB.tot_chick >= (int)newNode->leftB.tot_wolf) || (int)newNode->leftB.tot_chick == 0)) {

                //Update the boat location
                newNode->leftB.boat = true;
                newNode->rightB.boat = false;
                return true;
            }
            else
                return false;
        }
        else {
            //Check if enough animals exist to move
            if (state.leftB.tot_chick >= numChick) {
                newNode->leftB.tot_chick = state.leftB.tot_chick - numChick;
                newNode->rightB.tot_chick = state.rightB.tot_chick + numChick;
            }
            else {
                return false;
            }
            //Check if enough animals exist to move
            if (state.leftB.tot_wolf >= numWolv) {
                newNode->leftB.tot_wolf = state.leftB.tot_wolf - numWolv;
                newNode->rightB.tot_wolf = state.rightB.tot_wolf + numWolv;
            }
            else {
                return false;
            }
            //check if changes were valid
            if ((((int)newNode->rightB.tot_chick >= (int)newNode->rightB.tot_wolf) || ((int)newNode->rightB.tot_chick == 0))
                && (((int)newNode->leftB.tot_chick >= (int)newNode->leftB.tot_wolf) || (int)newNode->leftB.tot_chick == 0)) {

                //Update the boat location
                newNode->leftB.boat = false;
                newNode->rightB.boat = true;
                return true;
            }
            else
                return false;
        }
    }
    return false;

}
////==============================VALIDATORS - END================================================


// Breadth-First Search and Depth-First Search Algorithm
void bfs_dfs(State initial, State goal, int flag) {
    //Create First Node and Initize
    Node* node = new Node();
    node->currentState = initial;
    node->parent = NULL;
    vector <Node*> vPath;
    vPath.push_back(node);

    Node* newNode;
    int count = 1;

    //Flag 1: BFS, Flag 2: DFS
    switch (flag)
    {
        case 1: {// code to be executed if flag = 1;
            //Create queue to save expanded values
            queue <Node*> qList;
            //Add Node to the queue
            qList.push(node);

            cout << "---END PATH---" << endl;
            outputFile << "---END PATH---" << endl;
            //Do while queue is not empty
            while (!qList.empty()) {

                node = qList.front();
                qList.pop();

                //Check if Goal state and Initial State is the same
                if (stateChecker((node->currentState), goal)) {
                    printSolution(node);
                    break;
                }
                //Increment the expand count
                count += 1;

                State newState;
                //Put one chicken in the boat (1,0)
                //Put two chickens in the boat (2,0)
                //Put one wolf in the boat (0,1)
                //Put one wolf and one chicken in the boat (1,1)
                //Put two wolves in the boat (0,2)
                int chicken[5] = { 1,2,0,1,0 };
                int wolve[5] = { 0,0,1,1,2 };
                for (int i = 0; i < 5; i++) {
                    if (actionChecker(vPath, node->currentState, &newState, chicken[i], wolve[i])) {
                        if (!visitedChecker(newState, vPath)) {
                            childNodes(node, newNode, newState, vPath, qList);
                        }

                    }
                }
            }
            break;
        }
        case 2: { // code to be executed if flag = 2;
            //Create stack to save expanded values
            stack <Node*> sList;
            //Add Node to the stack
            sList.push(node);


            cout << "---END PATH---" << endl;
            outputFile << "---END PATH---" << endl;
            //Do while stack is not empty
            while (!sList.empty()) {
                node = sList.top();
                sList.pop();

                //Check if Goal state and Initial State is the same
                if (stateChecker((node->currentState), goal)) {
                    printSolution(node);
                    break;
                }
                //Increment the expand count
                count += 1;
                //Create new state to hold
                State newState;
                int chicken[5] = { 1,2,0,1,0 };
                int wolve[5] = { 0,0,1,1,2 };
                for (int i = 0; i < 5; i++) {
                    if (actionChecker(vPath, node->currentState, &newState, chicken[i], wolve[i])) {
                        if (!visitedChecker(newState, vPath)) {
                            dfs_childNodes(node, newNode, newState, vPath, sList, false);
                        }
                    }
                }
            }

            break;
        }

    }
    //Print nodes expanded

    cout << "Number Node Expanded: " << count << endl << endl;
    outputFile << "Number Node Expanded: " << count << endl << endl;
}

// Iterative-Deepening Depth First Search Algorithm
void iddfs(State initial, State goal) {
    int count = 1;
    bool flag = true;
    //Create stack to save expanded values
    stack <Node*> sList;
    vector <Node*> vPath;
    int dMax = 0, depthCurrent = 0;
    cout << "---END PATH---" << endl;
    outputFile << "---END PATH---" << endl;
    while (flag == true) {
        Node* node = new Node();
        node->currentState = initial;
        node->parent = NULL;
        node->depth = 0;

        //Add the Node in the stack
        sList.push(node);
        vPath.push_back(node);

        Node* newNode;
        //Do while stack is not empty
        while (!sList.empty()) {
            node = sList.top();
            sList.pop();

            if (depthCurrent < node->depth) {
                depthCurrent = node->depth;
            }

            //Check if Goal state and Initial State is the same
            if (stateChecker((node->currentState), goal)) {
                break;
            }
            //Increment the expand count
            count += 1;

            if (node->depth == dMax) {
                printf(""); // skip child node creation stuff
            }
            else {
                State newState;
                int chicken[5] = { 1,2,0,1,0 };
                int wolve[5] = { 0,0,1,1,2 };
                for (int i = 0; i < 5; i++) {
                    if (actionChecker(vPath, node->currentState, &newState, chicken[i], wolve[i])) {
                        if (!visitedChecker(newState, vPath)) {
                            dfs_childNodes(node, newNode, newState, vPath, sList, true);
                        }
                    }
                }
            }
        }
        //Check if Goal state and Initial State is the same
        if (stateChecker((node->currentState), goal)) {
            printSolution(node);
            break;
        }
        if (depthCurrent < dMax) {
            printf("No solution found for IDDFS\n");
            break;
        }
        else {
//            printf("No solution found yet!!\n");
            dMax++;
            depthCurrent = 0;
            while (!vPath.empty()) {
                vPath.pop_back();
            }
        }
    }

    //Print nodes expanded
    cout << "Number Node Expanded: " << count << endl;
    cout << "Max depth: " << dMax << endl << endl;

    //Write to output file
    outputFile << "Number Node Expanded: " << count << endl;
    outputFile << "Max depth: " << dMax << endl << endl;
}

//A-star search Algorithm
//https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
void aStar(State initial, State goal) {
    int count = 1;
    //Create the First Node
    Node* node = new Node();
    Node* tmp_node = new Node();
    node->currentState = initial;
    node->parent = NULL;
    node->depth = 0;

    tmp_node = node;
    tmp_node->priority = heuristicAstar(initial, goal);

    //Create a priority queue for A-Sart
    priority_queue<Node*> queue;
    vector <Node*> vPath;

    queue.push(tmp_node);
    vPath.push_back(node);

    Node *newNode;
    cout << "---END PATH---" << endl;
    outputFile << "---END PATH---" << endl;
    while(!queue.empty()){
        node = queue.top();
        queue.pop();

        //Check if Goal state and Initial State is the same
        if (stateChecker((node->currentState), goal)) {
            printSolution(node);
            break;

        }
        //Increment the expand count
        count += 1;
        State newState;
        int chicken[5] = {1,2,0,1,0};
        int wolve[5] = {0,0,1,1,2};
        for(int i = 0; i < 5; i++){
            if (actionChecker(vPath, node->currentState, &newState, chicken[i], wolve[i])) {
                if (!visitedChecker(newState, vPath)) {
                  astar_childNode(node, newNode, newState, goal, vPath, queue);
                }
            }
        }

    }

    //Print nodes expanded
    cout << "Number Node Expanded: " << count << endl << endl;
    outputFile << "Number Node Expanded: " << count << endl << endl;
}
