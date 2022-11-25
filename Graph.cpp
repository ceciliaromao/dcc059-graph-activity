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
#include <limits.h>

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
    if(getNode(id) == nullptr)
    {
        insertNode(id);
    }
    getNode(id)->insertEdge(target_id, weight);
    if(!directed)
    {
        getNode(target_id)->insertEdge(id, weight);
    }
    this->number_edges++;
}

void Graph::removeEdge(int id, int target_id)
{
    this->number_edges -= getNode(id)->removeEdge(target_id, directed, getNode(target_id));
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

//Function that verifies if there is a path between two nodes
bool Graph::depthFirstSearch(int initialId, int targetId){
    Node* aux = getNode(initialId);

    if(aux == nullptr)
    {
        return false; 
    }
    if(initialId == targetId)
        return true; 
    verified[initialId] = true; 

    for(Edge *i = aux->getFirstEdge(); i != nullptr; i = i->getNextEdge())
    {
        //Line for debug

        cout<<i->getTargetId()<<endl;

        if(!verified[i->getTargetId()])
        {
            if(i->getTargetId() == targetId)
            {
                return true; 
            }


            if(depthFirstSearch(i->getTargetId(), targetId))
                return true;

            return depthFirstSearch(i->getTargetId(), targetId);

        }
    }

    return false; 
}

//? Essa função começa a procurar a partir de onde?
void Graph::breadthFirstSearch(ofstream &output_file){
    list<int> queue;
    int ak = this->getFirstNode()->getId();
    verified[ak] = true;
    queue.push_back(ak);

    verified.clear();
    
    Node* aux;
    while(!queue.empty())
    {
        ak = queue.front();
        queue.pop_front();

        aux = this->getNode(ak);
        for(Edge *i = aux->getFirstEdge(); i != nullptr; i = i->getNextEdge())
        {
            //output_file<<i->getTargetId()<<endl;
            //Não sei exatamente como escrever esse dado no arquivo ainda, eis o cout:
            cout<<i->getTargetId()<<endl;
            if (!verified[i->getTargetId()])
            {
                verified[i->getTargetId()] = true;
                queue.push_back(i->getTargetId());
            }
        }
    }
    //delete aux; //?pq quando eu deixo esse delete, o código dá problema de segmentação?

}


Graph *Graph::getComplement(){

    if (this->first_node == nullptr)
    {
        return nullptr;
    }
    
    //checking if the graph is completed
    int check_edges = (this->order*(this->order-1))/2;

    if(check_edges == this->number_edges){
        cout<< "The graph is completed" << endl;
        return nullptr;
    }


    Graph * complement = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);

    // complement->insertNode(this->first_node->id);
    
    Node * node = this->first_node;

    while (node !=nullptr)
    {
        complement->insertNode(node->id);
        for(Node *i = this->first_node; i!=nullptr; i = i->next_node){
            if(node->hasEdgeBetween(i->id)==nullptr && (i->id != node->id)){
                complement->insertNode(i->id);
                complement->insertEdge(node->id,i->id,0);
            }
        }
        node = node->next_node;
    }

    return complement;
}

    

//A function that returns a subjacent of a directed graph, which is a graph which the arcs have opposite directions to the original graph
Graph* Graph::getSubjacent(){

    if (this->directed == false)
    {
       return nullptr;
    }
    
    Graph* subjacent = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);

    subjacent->first_node = this->first_node;

    Node * next_node = subjacent->first_node;

    while(next_node != nullptr){

        next_node->in_degree = 0;

        next_node->out_degree = 0;
        
        next_node = next_node->getNextNode();
    }

    subjacent->directed = false;
    
    return subjacent;
}

bool Graph::connectedGraph(){
    
}



bool Graph::hasCircuit(){
    
}



float** Graph::floydMarshall(){
    
}

int mindist(float dist[], bool visited[], int order){
    int min = INT_MAX, ind;
    for(int i = 0; i < order; i++){
        if(dist[i] <= min && !visited[i]){
            min = dist[i];
            ind = i;
        }
    }
    return ind;
};

float* Graph::dijkstra(int id){

    //vetor de vertices visitados
    bool *visited = new bool[order];
    float *distance = new float[order];

    for (int i = 0; i < order; i++)
    {
        visited[i] = false;
        distance[i] = INT_MAX;
    }

    distance[id] = 0;

    for (int i = 0; i < order; i++)
    {
        int min = mindist(distance, visited, order);
        visited[min] = true;   
        for (Edge *aux = getNode(min)->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge())
        {
            if (!visited[aux->getTargetId()] && distance[min] != INT_MAX && distance[min] + aux->getWeight() < distance[aux->getTargetId()])
            {
                distance[aux->getTargetId()] = distance[min] + aux->getWeight();
            }
        }
    }
    return distance;
}