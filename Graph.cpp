#include "Graph.h"
#include <thread>
#define INT 99999
#define timer_cout std::cout

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods for second part
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

        next_node->removeAllEdges(this->directed);
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

void Graph::setFirstNode(Node *node)
{
    this->first_node = node;
}

void Graph::setLastNode(Node *node)
{
    this->last_node = node;
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

    if (this->getFirstNode() == nullptr)
    {
        this->first_node = new Node(id);
        this->last_node = this->getFirstNode();
    }
    else
    {
        if (!this->searchNode(id))
        {
            Node *node = new Node(id);
            node->setNextNode(nullptr);

            this->last_node->setNextNode(node);
            this->last_node = node;
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
            //Não possui incremento de grau //TODO
            getNode(id)->insertEdge(target_id, weight);
            
            if(id == target_id)
                getNode(id)->in_degree+=2;
            else 
                getNode(id)->in_degree++;

            if(!getNode(target_id)->searchEdge(id))
            {
                getNode(target_id)->insertEdge(id, weight);

                getNode(target_id)->in_degree++;
            }   
            this->number_edges++;
        }
    }
    
}

void Graph::removeEdge(int id, int target_id)
{
    this->number_edges -= getNode(id)->removeEdge(target_id, directed, getNode(target_id));
}


bool Graph::searchNode(int id)
{
    if (this->first_node != nullptr)
    {
        for (Node *aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            if (aux->getId() == id)
                return true;
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
            if (aux->getId() == id)
                return aux;
        }
    }

    return nullptr;
}

//biggest ratio between the node's degree and it's weight
double lambda3(Graph* graph,int node_id,map <int,bool> &in_solution){
    Node * node = graph->getNode(node_id);
 
    return node->getInDegree()/node->getWeight();
}

// biggest ratio between the sum of the weights of the non-dominated neighbors and the node weight
double lambda4(Graph* graph, int node_id,map <int,bool> &in_solution){
    Node* node = graph->getNode(node_id);
    int neighbor_weight = 0;
    for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
        if(!in_solution[edge->getTargetId()])
            neighbor_weight += graph->getNode(edge->getTargetId())->getWeight();
    }
    return neighbor_weight/node->getWeight();
}

// biggest ratio between the product of the non-dominated neighbor count and sum the of weights with the node weight
double lambda5(Graph* graph,int node_id,map <int,bool> &in_solution){
    Node * node = graph->getNode(node_id);
    
    int neighbor_weight = 0;
    int non_dominated_neighbors = 0;
    for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
        if(!in_solution[edge->getTargetId()]){
            neighbor_weight += graph->getNode(edge->getTargetId())->getWeight();
            non_dominated_neighbors++;
        }    
    }

    return (non_dominated_neighbors*neighbor_weight)/node->getWeight();
}

// biggest sum of non-dominated neighbor weight
double lambda6(Graph* graph, int node_id,map <int,bool> &in_solution){
    Node* node = graph->getNode(node_id);
    int neighbor_weight = 0;
    for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
        if(!in_solution[edge->getTargetId()])
            neighbor_weight += graph->getNode(edge->getTargetId())->getWeight();
    }
    return neighbor_weight;
}



priority_queue<pair<double,int>> heuristic(Graph* graph, double (lambda)(Graph*,int,map<int,bool>&), map <int,bool> &in_solution){
    priority_queue<pair<double,int>>node_degrees;
    Node * node = graph->getFirstNode();
    double heuristic_value = 0;

    while(node!=nullptr){
        heuristic_value = lambda(graph,node->getId(),in_solution);

        node_degrees.push(make_pair(heuristic_value, node->getId()));
        
        node = node->getNextNode();
    }

    return node_degrees;
}

vector<int> heuristic2(Graph *graph,map <int,bool> &in_solution)
{
    priority_queue<pair<double, int>> aux = heuristic(graph,lambda4,in_solution);
    vector<int> vet;

    while(!aux.empty())
    {
        vet.push_back(aux.top().second);
        aux.pop();
    }

    return vet; 
}

unsigned int rNode(int min, int max)
{
    std::random_device rd;
    std::mt19937_64 e{rd()}; 
    std::uniform_int_distribution<> dist{min, max};

    unsigned int randomNumber = dist(e);
    return randomNumber;
}

//Greedy Constructive Algorithm
set<pair<int,int>> Graph::GreedyConstructive(){

    // set containing each node and its weight
    set<pair<int,int>> auxSolutionSet;
    
    // map to verify if node is in solution
    map<int,bool> in_solution;
    
    for(int i = 1; i < this->order; i++){
        in_solution.insert(make_pair(i,false));
    }
        
    // get first node 
    Node * node = this->getFirstNode();
    
    //max heap to get node with highest degree
    priority_queue<pair<double,int>> node_degrees = heuristic(this,lambda4,in_solution);

    bool viable = false;
    int check = 0;
    int heuristic_node = node_degrees.top().second;
    node_degrees.pop();

    while(!viable){

        for(Edge* edge =this->getNode(heuristic_node)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            if(!in_solution[edge->getTargetId()]){
                auxSolutionSet.insert(make_pair(heuristic_node,getNode(heuristic_node)->getWeight()));
                in_solution[heuristic_node] = true;
            }
            in_solution[edge->getTargetId()] = true;
        }   
        
        for(int i = 1; i < this->order; i++){
            if(in_solution[i]){
                check++;
            }
        }
        if(check == this->order-1){
            viable = true;
        } else {
            check = 0; 
            node_degrees = heuristic(this,lambda5,in_solution);
        }


        heuristic_node = node_degrees.top().second;
        
        node_degrees.pop();

    }

    return auxSolutionSet;
}

set<pair<int,int>> Graph::GreedyRandomizedAdaptive(double alpha, int numIter){
    //!Timer
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = chrono::high_resolution_clock::now();
    // set for the best solution
    set<pair<int,int>> bestSolutionSet;

    // set for the current solution
    set<pair<int,int>> auxSolutionSet;

    // map to verify if node is in solution
    map<int,bool> in_solution;

    int i = 1, currentWeight = 0, bestWeight;
    unsigned int k = 0;
    while (i <= numIter) {

        // candidate list is ordered according to biggest ratio between the weights sum of the
        // non-dominated neighbors and the weight of the node
        vector<int> candidateList = heuristic2(this,in_solution);
        
        // initialize solution set
        for(int i = 1; i < this->order; i++){
            in_solution.insert(make_pair(i,false));
        }

        while (!candidateList.empty()) {
            // get random node from candidate list
            k = rNode(0, trunc(alpha* (float)candidateList.size()));
            sort(candidateList.begin(), candidateList.end(), greater<int>());
            int randomNode = candidateList[k];

            // if node is not in solution yet 
            if (!in_solution[randomNode]) {
                // add node to solution
                auxSolutionSet.insert(make_pair(randomNode,getNode(randomNode)->getWeight()));
                in_solution[randomNode] = true;
                currentWeight += getNode(randomNode)->getWeight();
            }

            // remove node from candidate list
            candidateList.erase(candidateList.begin()+k);

            // remove dominated nodes from candidate list
            for (int j = 0; j < candidateList.size(); j++) {
                Node * candidate = this->getNode(candidateList[j]);
                if (candidate->searchEdge(randomNode)) {
                    candidateList.erase(candidateList.begin()+j);
                }
            }
        }

        // if current solution is better than best solution
        if ((i == 1 || currentWeight < bestWeight) && !auxSolutionSet.empty()) {
            bestSolutionSet.clear();
            bestSolutionSet.swap(auxSolutionSet);
            bestWeight = currentWeight;  //?soma-se 1 aqui?? 
        }

        i++;
        currentWeight = 0;
        auxSolutionSet.clear();
        in_solution.clear();
    }

    end = chrono::high_resolution_clock::now();
    int elapsed_seconds = chrono::duration_cast<chrono::milliseconds>(end-start).count();

    return bestSolutionSet;
}

void Graph::updateProbabilities(vector<double>*probabilities, vector<double>alphas, int bestWeight, vector<pair<double,int>>avgWeights) {
    
    // according to section 3.1 Reactive GRASP on Handbook of Metaheuristics (check if it's correct)

    vector <double> q;
    double sum = 0, qi;
    
    for (int i = 0; i < alphas.size(); i++) {
        if (avgWeights.at(i).first != 0)
            qi = bestWeight/(double)avgWeights.at(i).first;
        else
            qi = 1.0/alphas.size();
        q.push_back(qi);
        sum += qi;
    }

    for (int i = 0; i < alphas.size(); i++) {
        if (q.at(i) != 0)
            probabilities->at(i) = (q.at(i) / sum);
    }

    q.clear();
}


double Graph::chooseAlpha(vector<double>* probabilities, vector<double> alphas) {
    // choose alpha according to probabilities
    double alpha;
    double highest = 0;
   
    for (int i = 0; i < probabilities->size(); i++) {
        if (probabilities->at(i) > highest) {
            highest = probabilities->at(i);
            alpha = alphas[i];
            
        }
    }

    return alpha;
}

void Graph::updateAvgWeights(vector<pair<double,int>>& avgWeights, vector<double> alphas, double alpha, int currentWeight) {
    // update average weights when alphas[j] == alpha
    if (currentWeight == 0) return;

    for (int j = 0; j < alphas.size(); j++) {
        if (alphas[j] == alpha) {
            avgWeights.at(j).second++;

            int qtd = avgWeights.at(j).second;
            double avg = avgWeights.at(j).first;

            avgWeights.at(j).first = (avg*(qtd-1) + currentWeight)/qtd;
            break;
        }
    }
}

set<pair<int,int>> Graph::GreedyRandomizedReactive(vector<double> alphas, int numIter, int block_size){
    // list of probabilities
    vector<double> probabilities(alphas.size(), 1.0/alphas.size());

    // average of weights for each alpha
    vector<pair<double,int>> avgWeights(alphas.size(), make_pair(0,0));

    // set for the best solution
    set<pair<int,int>> bestSolutionSet;

    // set for the current solution
    set<pair<int,int>> auxSolutionSet;

    // map to verify if node is in solution
    map<int,bool> in_solution;

    int i=1, currentWeight = 0, bestWeight = INT;
    unsigned int k;

    double alpha;

    while (i <= numIter) {
        
        // find first n solutions using each one of the alphas
        if (i <= alphas.size())
            alpha = alphas[i-1];
        else {
            alpha = chooseAlpha(&probabilities, alphas);
        }

        if (i % block_size == 0) {
            // update probabilities
            updateProbabilities(&probabilities, alphas, bestWeight, avgWeights);
        }

        // initialize solution set
        for(int i = 1; i < this->order; i++){
            in_solution.insert(make_pair(i,false));
        }

        // candidate list is ordered according to biggest ratio between the weights sum of the
        // non-dominated neighbors and the weight of the node
        vector<int> candidateList = heuristic2(this,in_solution);
        sort(candidateList.begin(), candidateList.end(), greater<int>());

        while (!candidateList.empty()) {
            // exactly like the GRASP algorithm, but choosing between a set of possible alphas
            k = rNode(0, trunc((1-alpha)* (float)candidateList.size()));
            int randomNode = candidateList[k];

            // if node is not in solution yet 
            if (!in_solution[randomNode]) {
                // add node to solution
                auxSolutionSet.insert(make_pair(randomNode,getNode(randomNode)->getWeight()));
                in_solution[randomNode] = true;
                currentWeight += getNode(randomNode)->getWeight();
            }

            // remove node from candidate list
            candidateList.erase(candidateList.begin()+k);

            // remove dominated nodes from candidate list
            for (int j = 0; j < candidateList.size(); j++) {
                Node * candidate = this->getNode(candidateList[j]);
                if (candidate->searchEdge(randomNode)) {
                    candidateList.erase(candidateList.begin()+j);
                }
            }
        }
        
        updateAvgWeights(avgWeights, alphas, alpha, currentWeight);

        // if current solution is better than best solution
        if ((i == 1 || currentWeight < bestWeight) && !auxSolutionSet.empty()) {
            bestSolutionSet.clear();
            bestSolutionSet.swap(auxSolutionSet);
            bestWeight = currentWeight;
        }

        i++;
        
        currentWeight = 0;
        auxSolutionSet.clear();
        in_solution.clear();
        candidateList.clear();
    }

    return bestSolutionSet;
}