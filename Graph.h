/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include "random_n.h"
#include <fstream>
#include <stack>
#include <list>
#include <map>
#include <set>
#include "Edge.h"
#include <iostream>
#include <queue>
#include <cstdlib>
#include <float.h>
#include <iomanip>
#include <vector>
#include <limits.h>
#include <random>
#include <algorithm>
#include<chrono>

using namespace std;

class Graph{

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;
        map<int, bool> verified; 

    public:
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        //Destructor
        ~Graph();
        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();
        //Other methods
        void insertNode(int id);
        void insertEdge(int id, int target_id, float weight);
        void removeEdge(int id, int target_id);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);
        set<pair<int,int>> GreedyConstructive();
        set<pair<int,int>> GreedyRandomizedAdaptive(double alpha, int iterations);
        set<pair<int,int>> GreedyRandomizedReactive(vector<double> alphas, int numIter, int block_size);
        void updateProbabilities(vector<double>&probabilities, vector<double>alphas, int bestWeight, vector<pair<double,int>>avgWeights);
        double chooseAlpha(vector<double>& probabilities, vector<double>alphas);
        void updateAvgWeights(vector<pair<double,int>>& avgWeights, vector<double> alphas, double alpha, int currentWeight);

};

#endif // GRAPH_H_INCLUDED