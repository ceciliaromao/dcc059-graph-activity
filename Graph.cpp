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
#include <vector>
#include <limits.h>

#define INT 99999

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

    if(getNode(target_id) == nullptr)
    {
        insertNode(target_id);
    }
    if(!getNode(id)->searchEdge(target_id)){
        if(directed)
        {
            getNode(id)->insertEdge(target_id, weight);
            getNode(id)->incrementOutDegree();
            getNode(target_id)->incrementInDegree();
            this->number_edges++;
        } else {

            getNode(id)->insertEdge(target_id, weight);
            if(!getNode(target_id)->searchEdge(id))
            {
                getNode(target_id)->insertEdge(id, weight);
            }   
            this->number_edges++;
        }
        
    }
    
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

//A function that returns the union of two graphs
Graph* Graph::getUnion(Graph* graph){

    //checks if the graphs are compatible
    if(this->getDirected() != graph->getDirected() || this->getWeightedEdge() != graph->getWeightedEdge() || this->getWeightedNode() != graph->getWeightedNode())
    {
        return nullptr;
    }
    Graph* unionGraph = new Graph(this->order ,this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());

    if(this->order == graph->order)
        Graph* unionGraph = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);
    else if(this->order > graph->order)
        Graph* unionGraph = new Graph(this->order + (this->order-graph->order), this->directed, this->weighted_edge, this->weighted_node);
    else if(this->order < graph->order)
        Graph* unionGraph = new Graph(graph->order + (graph->order-this->order), this->directed, this->weighted_edge , this->weighted_node);
    
    Node* sourceNode = this->first_node;

    //Insere nós e arestas do grafo fonte
    while(sourceNode != nullptr)
    {
        for(Edge* i = sourceNode->getFirstEdge(); i != nullptr; i = i->getNextEdge())
        {
            unionGraph->insertEdge(sourceNode->id, i->getTargetId(), i->getWeight());
        }
        sourceNode = sourceNode->next_node;
    }

    Node* unionNode = graph->first_node;

    // Insere nós e arestas do grafo de união
    while(unionNode != nullptr)
    {
        // for(Node* graph_1 = this->first_node; graph_1 !=nullptr; graph_1 = graph_1->next_node){
        //     for(Edge *i = unionNode->getFirstEdge(); i != nullptr; i = i->getNextEdge())
        //     {
        //        if(unionNode->id != graph_1->id)
        //             unionGraph->insertEdge(unionNode->id, i->getTargetId(), i->getWeight());
        //     }
        // }
        for(Edge *i = unionNode->getFirstEdge(); i != nullptr; i = i->getNextEdge())
        {
            unionGraph->insertEdge(unionNode->id, i->getTargetId(), i->getWeight());
        }
        unionNode = unionNode->next_node;
    }

    return unionGraph;
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
        Graph* complement= new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);
        return complement;
    }


    Graph * complement = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);

    // complement->insertNode(this->first_node->id);
    
    Node * node = this->first_node;

    while (node !=nullptr)
    {

        for(Node *i = this->first_node; i!=nullptr; i = i->next_node){
            if(!(node->searchEdge(i->id))){
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

    Node * next_node = this->first_node;
    subjacent->directed = false;
    
    while(next_node != nullptr){

        subjacent->insertNode(next_node->id);

        Node* aux = subjacent->getNode(next_node->id);

        aux->in_degree= 0;
        aux->out_degree = 0;

        if (next_node->first_edge != nullptr)
        {
            Edge * next_edge = next_node->first_edge;

            while (next_edge != nullptr)
            {
                subjacent->insertEdge(next_edge->getTargetId(), next_node->id, next_edge->getWeight());
                next_edge = next_edge->getNextEdge();
            }
        }
        
        next_node = next_node->getNextNode();
    }
    
    return subjacent;
}


bool Graph::connectedGraph(){
    
}

bool Graph::hasCircuit(){
    
}


float** Graph::floydWarshall(){
    Node *node = getNode(1);

    if(node == nullptr)
    {
        cout<<"Node not found"<<endl;
        return nullptr;
    }

    float **dist = new float*[order];

    // Initialize the distance matrix with INT
    for (int i = 0; i < order; i++){
        dist[i] = new float[order];
        for (int j = 0; j < order; j++)
            dist[i][j] = INT;
    }
    // Initialize the distance matrix with the weight of the edges
    for (int i = 0; i < order; i++) {
        for (Edge *j = getNode(i)->getFirstEdge(); j != nullptr; j = j->getNextEdge()) {
            dist[i][j->getTargetId()] = j->getWeight();
        }
    }

    // Calculate the shortest path
    for (int k = 0; k < order; k++) {
        for (int i = 0; i < order; i++) {
            for (int j = 0; j < order; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j] && dist[i][k] != INT && dist[k][j] != INT)
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
            dist[i][i] = 0;
        }
    }

    return dist;
}

float* Graph::dijkstra(int id){

    Node *node = getNode(id);

    if(node == nullptr)
    {
        cout<<"Node not found" <<endl;
        return nullptr;
    }

    //vetor de vertices visitados
    bool *visited = new bool[order];
    float *distance = new float[order];

    for (int i = 0; i < order; i++)
    {
        visited[i] = false;
        distance[i] = INT;
    }

    //distancia do nó fonte para ele mesmo é 0
    distance[id] = 0;
    
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> queue_;
    queue_.push(make_pair(distance[id], id));
    pair<float,int>pair_ = queue_.top();
    
    //primeira iteração seta o primeiro nó como visitado
    while(!queue_.empty()){

        pair<float,int>pair_aux = queue_.top();
        int u = pair_aux.second;
        queue_.pop();
        
        if(visited[u]) 
            continue; 
        else 
            visited[u] = true;
        
        Node* aux = getNode(u);
        // cout << aux->id   << endl;
        for(Edge *i = aux->getFirstEdge(); i != nullptr; i = i->getNextEdge()){
            float weight;
            
            !(this->weighted_edge) ? weight = 1 : weight = i->getWeight(); 

            int v = i->getTargetId();

            if((!visited[v]) && distance[v] > (distance[u] + weight)){
                distance[v] = distance[u] + weight;
                queue_.push(make_pair(distance[v], v));
            }
        }
        
    }

    delete [] visited;
    return distance;
}