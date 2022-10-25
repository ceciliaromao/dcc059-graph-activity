#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <map>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}

bool Graph::getDirected()
{

    return this->directed;
}

bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

bool Graph::getWeightedNode()
{

    return this->weighted_node;
}

Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    Node *next;
    Node *aux = nullptr;

    if(this->getFirstNode() == nullptr) {
        this->first_node = new Node(id);
        this->last_node = this->getFirstNode();
    } else {
      if (!this->searchNode(id)) {
            Node *node = new Node(id);
            node->setNextNode(nullptr);

            this->last_node->setNextNode(node);
            this->last_node = node;
        
            /* next = this->first_node;

            while (next != nullptr)
            {
                aux = next;
                next = next->getNextNode();
            }

            aux->setNextNode(node) */;
        }      
    }
}

void Graph::insertEdge(int id, int target_id, float weight)
{
<<<<<<< HEAD
    
=======
    if(getNode(id) == nullptr)
    {
        insertNode(id);
    }
    getNode(id)->insertEdge(target_id, weight);
    this->number_edges++;
>>>>>>> feat-graph-insere-aresta]
}

void Graph::removeEdge(int id, int target_id)
{
    if(getNode(id)!= nullptr)
        getNode(id)->removeEdge(id, directed, getNode(target_id));
}

void Graph::removeNode(int id){ 
    
}

bool Graph::searchNode(int id)
{
    if (this->first_node != nullptr)
    {
        for (Node *aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            if (aux->getId() == id) return true;
        }
    }

    return false;
}

Node *Graph::getNode(int id)
{
    if (this->first_node != nullptr)
    {
        for (Node *aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            if (aux->getId() == id) return aux;
        }
    }

    return nullptr;
    
}

//NECESSÁRIO TESTAR!!!!!!!!!!!!!!
//Function that verifies if there is a path between two nodes
bool Graph::depthFirstSearch(int initialId, int targetId){
    Node* aux = getNode(initialId);

    if(aux == nullptr)
        return false; 
    if(initialId == targetId)
        return true; 

    map<int,bool> verified; 
    verified[initialId] = true; 

    for(Edge *i = aux->getFirstEdge(); i != aux->getLastEdge(); i = i->getNextEdge())
    {
        if(!verified[i->getTargetId()])
        {
            if(i->getTargetId() == targetId)
            {
                return true; 
            }
            return depthFirstSearch(i->getTargetId(), targetId);
        }
    }

    return false; 
}


void Graph::breadthFirstSearch(ofstream &output_file){
    
}


Graph *Graph::getComplement(){
    
}

    

//A function that returns a subjacent of a directed graph, which is a graph which the arcs have opposite directions to the original graph
Graph* Graph::getSubjacent(){
    
}

bool Graph::connectedGraph(){
    
}



bool Graph::hasCircuit(){
    
}



float** Graph::floydMarshall(){
    
}

   

float* Graph::dijkstra(int id){
    
}