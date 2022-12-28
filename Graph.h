/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <map>
#include <vector>

using namespace std;

struct pertTask{
    int id;
    int a;
    int b;
}; 

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
        // Setters
        void setFirstNode(Node* node);
        void setLastNode(Node* node);
        //Other methods
        void insertNode(int id);
        void insertEdge(int id, int target_id, float weight);
        void removeEdge(int id, int target_id);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);
        bool depthFirstSearch(int initialId,int targetId);
        bool depthFirstSearch(int initialId, int targetId, ofstream &output_file); 
        void breadthFirstSearch(ofstream& output_file);
        Graph* getComplement();
        Graph* getSubjacent();
        Graph* getUnion(Graph* graph);
        Graph* getIntersection(Graph *graph);
        bool hasCircuit();
        bool connectedGraph();
        float** floydWarshall();
        float* dijkstra(int id);
        void writeDotFile(string file_name);
        void pert(string path_out);


    private:
        int isIn(vector<pertTask> sol, int id);
        //Auxiliar methods

};

#endif // GRAPH_H_INCLUDED