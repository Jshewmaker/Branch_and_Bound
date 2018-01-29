/*
Joshua Shewmaker
Program 4
 
Write a program that will take in a number of stop along a route with their coordinates. Each
 stop will have a set of pokemon at them. You want to be able to visit all the stops needed too
 collect all pokemon, with no duplicates. The program will find the most efficent route and it will 
 do this in a respectable amount of time
 
*/

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

struct PokeStop {
    int north_South = 0;
    int east_West = 0;
    int bound = 0;
    vector<string> pokemonAtStop;
    
    
    PokeStop(int x, int y){
        north_South = y;
        east_West = x;
    }
    
};

struct Node {
    vector<int> path;
    int weight = 0;
    int bound;
    
    bool operator<(const Node& other) const
    {
        return bound > other.bound; }
    
    
};
/*
 Pre: assumes 2 nodes have been created and passes both coords of both nodes in
 Post: Calculates the difference from x1 to x2 and y1 to y2. If the value is negative
 it makes it positive.
*/
int calculate_Weight(int parentEast, int parentNorth, int childEast, int childNorth){
    
    int weight = 0, eastWestDiff = 0, northSouthDiff = 0;
    
    eastWestDiff = parentEast - childEast;
    northSouthDiff = parentNorth - childNorth;
    weight = eastWestDiff + northSouthDiff;
    if(weight < 0)
        return weight * -1;
    else
        return eastWestDiff + northSouthDiff;
}
/*
 Pre: Number of pokestops has been determined. Blank matrix is passed in
 Post: fills matrix with the weight of each edge for every node
 */
void create_Distance_To_PokeStop_Matrix(int matrixSize,vector<PokeStop> & pokeVector, vector<vector<int>> & distanceMatrix){
    
    
    for (int i = 0; i < matrixSize; i++)
        distanceMatrix[i].resize(matrixSize);

    for(int i = 0; i < distanceMatrix.size(); i++){
        for(int j = 0; j < distanceMatrix[i].size(); j++){
            if( i == j)
                distanceMatrix[i][j] = 0;
            else
                distanceMatrix[i][j] = calculate_Weight(pokeVector[i].east_West, pokeVector[i].north_South, pokeVector[j].east_West, pokeVector[j].north_South);
        }
    }
    
    //Prints out matrix for testing
   /* for (int i = 0; i < distanceMatrix.size(); i++) {
        for (int j = 0; j < distanceMatrix[i].size(); j++) {
        cout << distanceMatrix[i][j] << " ";
        }
        cout << endl;
    }
    */
    
    
}
/*
 Pre: Pokestops have been created and filled with coords and pokemon present
 Post: prints out poke present at each stop
 */
void print_PokeMap(vector<PokeStop> & pokeVector){
    for(int i = 0; i < pokeVector.size(); i++){
        for(int j = 0; j < pokeVector[i].pokemonAtStop.size(); j++)
            cout << pokeVector[i].pokemonAtStop[j] << " ";
        cout << endl;
    }
}
/*
 Pre: Matrix has been filled in with weights
 Post: returns lowest weight value from each row
 */
int lowValueOfRow(vector<vector<int>> & distanceMatrix, int lastNodeVisted){
    int rowLow = INT_MAX;
    
    for(int i = 0; i < distanceMatrix[lastNodeVisted].size(); i++){
        if(rowLow > distanceMatrix[lastNodeVisted][i] && distanceMatrix[lastNodeVisted][i] != 0)
            rowLow = distanceMatrix[lastNodeVisted][i];
        
    }
    
    return rowLow;
}
/*
 Pre: at least 2 nodes have been created. Matrix has been filled with weights
 Post: sets weight, bound and path for each new node pushed into priority queue
 */
void evaluate(Node &parentNode, int lastNodeVisted,int j, vector<vector<int>> & distanceMatrix, Node &newNode){
    
    int rowLow = INT_MAX;
    
    rowLow = lowValueOfRow(distanceMatrix, lastNodeVisted);
    newNode.weight = distanceMatrix[lastNodeVisted][j] + parentNode.weight;
    newNode.bound = parentNode.bound + (distanceMatrix[lastNodeVisted][j] - rowLow);
    newNode.path.push_back(j);
    
}

//*******************************************************************************
int main() {
    
    int numberOfPokeStops;
    int coordX;
    int coordY;
    int min = 0, tempMin = INT_MAX;  //Used for finding lower bound
    vector<PokeStop> pokeVector;    //Holds all inputed values in vector
    string pokemon;
    //priority_queue<int, vector<int>, greater<int>> pq;
    priority_queue<Node> Pokequeue;
    Node firstNode, tempNode;
    bool done = true;
    int lastNodeVisted;

    //fills all nodes with input
    cin >> numberOfPokeStops;
    numberOfPokeStops++;
    
    //creates matrix
    vector<vector<int>> distanceMatrix(numberOfPokeStops);
    pokeVector.push_back(PokeStop(0, 0));
    for(int i = 1; i < numberOfPokeStops; i++){
        cin >> coordX >> coordY;
        pokeVector.push_back(PokeStop(coordX, coordY));
        cin >> pokemon;
        pokeVector[i].pokemonAtStop.push_back(pokemon);
    }
    
    create_Distance_To_PokeStop_Matrix(numberOfPokeStops, pokeVector, distanceMatrix);
   
    //creates starting lower bound
    for(int i = 0; i < distanceMatrix.size(); i++){
        for(int j = 0; j < distanceMatrix[i].size(); j++){
            if(tempMin > distanceMatrix[i][j] && distanceMatrix[i][j] != 0){
                tempMin = distanceMatrix[i][j];
            }

        }
        min += tempMin;
        tempMin = INT_MAX;
    }
    
    firstNode.bound = min;
    firstNode.path.push_back(0);
    firstNode.weight = 0;
    Pokequeue.push(firstNode);
    while(done){
        Node newNode;
        tempNode = Pokequeue.top();
        
        
        if(tempNode.path.size() == numberOfPokeStops){
            //done = false;
           tempNode.bound = Pokequeue.top().bound + (distanceMatrix[tempNode.path.back()][0] - lowValueOfRow(distanceMatrix, Pokequeue.top().path.back()));
            break;
        }
        Pokequeue.pop();
        lastNodeVisted = tempNode.path.back();
        for(int i = 0; i < numberOfPokeStops; i++){
            //checks if we already went to that stop
            if(find(tempNode.path.begin(), tempNode.path.end(), i) != tempNode.path.end()){
                continue;
            }
            else{
                newNode.path = tempNode.path;
                newNode.weight = tempNode.weight;
                evaluate(tempNode, lastNodeVisted,i, distanceMatrix, newNode);
                Pokequeue.push(newNode);
            }
            
        }
    }
    /* SHOWS PATH FOR BUG TESTING
    for(int i = 0; i < tempNode.path.size(); i++){
        cout << tempNode.path[i] << " " << endl;
    }
   cout << endl;*/
    cout << tempNode.bound << endl;
   // print_PokeMap(pokeVector);
    
    return 0;
}
/*
4
5 9 Eeveeeeeee
1 1 Flareon
1 8 Jolteon
2 8 Umbreon
 
 
5
5 9 Eeveeeeeee
10 10 Flareon
1 1 Flareon
1 8 Jolteon
2 8 Umbreon
*/
