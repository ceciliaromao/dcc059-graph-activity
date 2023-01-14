#include "Graph.h"
#include "random_n.h"
#include <thread>
#define INT 99999
#define timer_cout std::cout

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
bool Graph::depthFirstSearch(int initialId, int targetId)
{
    //Confere casos triviais: não possuir o nó incial ou target / ou eles serem iguais
    if(getNode(initialId) == nullptr || getNode(targetId) == nullptr)
        return false; 
    else if(initialId == targetId)
        return true;
    
    stack<int> pilha;
    pilha.push(initialId);
    stack<int> auxPilha; 

    verified.clear();

    int current;
    //atualiza o nó a ser analizado e tira ele da pilha
    while(!pilha.empty())
    {
        current = pilha.top();
        pilha.pop();
        if(!verified[current])
        {
            // cout<<current<<endl;

            verified[current] = true; 
            if(current == targetId) return true;
            
            //percorre todos as arestas do nó analisado
            for(Edge *i = getNode(current)->getFirstEdge(); i!= nullptr; i = i->getNextEdge())
            {
                if(!verified[i->getTargetId()])
                {
                    // adiciona todos os nós "inéditos" na pilha Auxiliar
                    auxPilha.push(i->getTargetId());
                }
            }

            //coloca nós na pilha auxiliar na ordem correta
            while(!auxPilha.empty())
            {
                pilha.push(auxPilha.top());
                auxPilha.pop();
            }
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
    int nodes = this->order-1;

    bool *visited = new bool[this->order];
    int count = 1;

    // call DFS from node 1 to all nodes to check if they are reachable
    for (int i = 1; i < nodes; i++){
        visited[i] = depthFirstSearch(1, i+1);
        if (visited[i])
            count++;
    }

        
    // check if every node is reachable
    if (count == nodes) return true;
    else return false;
}

float** Graph::floydWarshall(){
    Node *node = getNode(1);

    if(node == nullptr)
    {
        cout<<"Node not found"<<endl;
        return nullptr;
    }
    
    int nodes = order-1;
    float **dist = new float*[nodes];
    
    // Initialize the distance matrix with INT
    for (int i = 0; i < nodes; i++){
        dist[i] = new float[nodes];
        for (int j = 0; j < nodes; j++)
            dist[i][j] = INT;
    }
    
    // Initialize the distance matrix with the weight of the edges
    
    for (int i = 0; i < nodes; i++) { 
        for (Edge *j = getNode(i+1)->getFirstEdge(); j != nullptr; j = j->getNextEdge()) {
            dist[i][j->getTargetId()-1] = j->getWeight();
        }
        dist[i][i] = 0;
    }

    // Calculate the shortest path
    for (int k = 0; k < nodes; k++) {
        for (int i = 0; i < nodes; i++) {
            for (int j = 0; j < nodes; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j] && dist[i][k] != INT && dist[k][j] != INT)
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    return dist;
}
// check if the graph has an eulerian circuit (closed trail -> no repeated edges)
bool Graph::hasCircuit(){

    // if the graph is not connected, it can't have an eulerian circuit
    if (!this->connectedGraph())
        return false;

    // if the graph is directed, it has an eulerian circuit if and only if every vertex has equal in degree 
    // and out degree, and all of its vertices with nonzero degree belong to a single strongly connected component
    
    if (this->directed)
    {
        Node * node = this->first_node;
        while (node != nullptr)
        {
            if (node->getInDegree() != node->getOutDegree())
                return false;
            node = node->getNextNode();
        }
        return true;
    }

    // if the graph is undirected and has nodes with odd degree, it can't have an eulerian circuit
    Node * node = this->first_node;
    while (node != nullptr)
    {
        if (node->in_degree % 2 != 0)
            return false;
        node = node->getNextNode();
    }

    return true;
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


//Escreve o grafo no arquivo dot
void Graph::writeDotFile(string file_name)
{
    ofstream output_file(file_name, ios::out | ios::trunc);
    verified.clear();
    if(!output_file.is_open())
    {
        cout<<"Arquivo não aberto"<<endl; 
    }
    if (this->first_node != nullptr)
    {
        string edge_symbol;
        if(this->directed)
        {
            edge_symbol = "->";
            output_file<<"digraph{"<<endl;
        }
        else    
        {
            edge_symbol = "--";
            output_file<<"strict graph{"<<endl;
        }
        
        for (Node *aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            for(Edge *i = aux->first_edge; i != nullptr; i = i->getNextEdge())
            {
                if(aux->getId() == i->getTargetId())
                    continue; 
                if(!verified[i->getTargetId()])
                    output_file<<aux->id <<edge_symbol<<i->getTargetId() << endl;
            }
            verified[aux->getId()] = true;
        }
        output_file<<"}";
    }
    else
    {
        cout<<"Grafo vazio"<<endl;
    }
    output_file.close();
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
    // if(min == max) return min; 

    // std::this_thread::sleep_for(25ms);
    // srand(std::chrono::system_clock::to_time_t(
    //                        std::chrono::system_clock::now()));
    std::random_device rd;
    std::mt19937_64 e{rd()}; 
    std::uniform_int_distribution<> dist{min, max};

    // unsigned int randomNumber = rand() % (max - min + 1) + min;
    unsigned int randomNumber = dist(e);
    return randomNumber;
}

double numb()
{
    // if(min == max) return min; 

    // std::this_thread::sleep_for(25ms);
    // srand(std::chrono::system_clock::to_time_t(
    //                        std::chrono::system_clock::now()));
    std::random_device rd;
    std::mt19937 e{rd()}; 
    
    std::uniform_real_distribution<double> dist{0.0, 1.0};

    // unsigned int randomNumber = rand() % (max - min + 1) + min;
    double randomNumber = dist(e);
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
        // if(avgWeights.at(i).first == 0)
        //     probabilities->at(i) = 1.0/alphas.size();

    }

    q.clear();
}

// void Graph::updateProbabilities(vector<double>*probabilities, vector<double>alphas, int bestWeight, vector<pair<double,int>>avgWeights) {
    
//     for (int i = 0; i < alphas.size(); i++) {
//             double p = avgWeights[i].first ;
//             if (bestWeight > 0) {
//                 p = bestWeight/p;
//             } else {
//                 p = 1.0 / alphas.size();
//             }
//             probabilities->at(i) = p;
//             cout << "probabilities: " << probabilities->at(i) << endl;
//         }  
// }

double Graph::chooseAlpha(vector<double>* probabilities, vector<double> alphas) {
    // choose alpha according to probabilities
    double alpha;
    double highest = 0;
   
    for (int i = 0; i < probabilities->size(); i++) {
        if (probabilities->at(i) > highest) {
            highest = probabilities->at(i);
            alpha = alphas[i];
            
        }
        // cout << "alpha: " << alphas[i] << " prob: " << probabilities->at(i) << endl;
    }

    // discrete_distribution<> d(probabilities->begin(), probabilities->end());
    // random_device rd;
    // mt19937 gen(rd());
    // int index = d(gen);
    // cout << d.probabilities()[3] << endl;
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
            // cout << "alpha: " << alphas[j] << " avg: " << avgWeights.at(j).first << endl;
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

    double alpha = alphas.at(rNode(0, alphas.size()-1));
    cout << alpha << endl;

    bool runOnce = false;
    map<double,bool>run_alphas;

    for(auto i: alphas){
        run_alphas.insert(make_pair(i,false));
    }
    run_alphas[alpha] = true;

    while (i <= numIter) {

        if (i % block_size == 0 && runOnce) {
            // update probabilities
            updateProbabilities(&probabilities, alphas, bestWeight, avgWeights);
            // alpha = alphas.at(rNode(0, alphas.size()-1));
        }

        // initialize solution set
        for(int i = 1; i < this->order; i++){
            in_solution.insert(make_pair(i,false));
        }

        // candidate list is ordered according to biggest ratio between the weights sum of the
        // non-dominated neighbors and the weight of the node
        vector<int> candidateList = heuristic2(this,in_solution);
        sort(candidateList.begin(), candidateList.end(), greater<int>());

        
        int c = 0;
        while (!candidateList.empty()) {
            // exactly like the GRASP algorithm, but choosing between a set of possible alphas
            // k = rNode(0, trunc((1-alpha)* (float)candidateList.size()));
            // cout << c<<"k: " << k << endl;
            // c++;
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
        if(runOnce)
            alpha = chooseAlpha(&probabilities, alphas);
        else
            alpha = alphas.at(rNode(0, alphas.size()-1));
        run_alphas[alpha] = true;

        // if current solution is better than best solution
        if ((i == 1 || currentWeight < bestWeight) && !auxSolutionSet.empty()) {
            bestSolutionSet.clear();
            bestSolutionSet.swap(auxSolutionSet);
            bestWeight = currentWeight;
        }

        int checker = 0;

        for(auto i: run_alphas){
            if(i.second)
                checker++;
        }

        if(checker == alphas.size()-1){
            runOnce = true;
        }

        i++;
        
        currentWeight = 0;
        auxSolutionSet.clear();
        in_solution.clear();
        candidateList.clear();
    }

    return bestSolutionSet;
}


/*
    Here is an example of how you might implement the Reactive GRASP procedure in C++, using the rule proposed in [78]:

    #include <vector>
    #include <random>
    #include <numeric>
    #include <iostream>

    std::vector<double> RCL_values = {0.1, 0.2, 0.3, 0.4, 0.5}; // list of possible RCL values
    std::vector<double> RCL_probabilities(RCL_values.size(), 1.0/RCL_values.size()); // initial probabilities for each RCL value
    double incumbent_solution = 0; // the current best solution found
    double average_solution = 0; // the average value of all solutions found

    std::random_device rd; // used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // standard mersenne_twister_engine seeded with rd()
    std::discrete_distribution<> RCL_distribution(RCL_probabilities.begin(), RCL_probabilities.end()); // generates random RCL value based on probabilities

    void updateProbabilities(double new_solution) {
        average_solution = (average_solution * (RCL_values.size() - 1) + new_solution) / RCL_values.size(); // update average_solution
        if (new_solution > incumbent_solution) {
            incumbent_solution = new_solution; // update incumbent_solution
        }

        for (int i = 0; i < RCL_values.size(); i++) {
            RCL_probabilities[i] = RCL_probabilities[i] * (1 - RCL_values[i]) + (RCL_values[i] * (incumbent_solution - average_solution));
        }
        RCL_distribution = std::discrete_distribution<>(RCL_probabilities.begin(), RCL_probabilities.end()); //update the discrete distribution
    }

    void grasp_iteration() {
        double RCL = RCL_values[RCL_distribution(gen)]; // select RCL value based on probabilities
        // construction phase of GRASP using RCL value
        double new_solution = ...; // code to find new solution
        updateProbabilities(new_solution);
        // local search phase of GRASP using new_solution
    }

    int main() {
        for (int i = 0; i < 100; i++) {
            grasp_iteration();
        }
        return 0;
    }

    In this example, the RCL_values list stores the possible values for the RCL parameter, the RCL_probabilities list stores the probabilities of selecting each RCL value, incumbent_solution stores the current best solution found and average_solution stores the average value of all solutions found. The std::random_device, std::mt19937, and std::discrete_distribution are used to generate a random RCL value based on the probabilities.

    The updateProbabilities function is called after each iteration to update the average_solution, incumbent_solution, and RCL_probabilities list. This function

    #include <iostream>
    #include <vector>
    #include <set>
    #include <map>
    #include <algorithm>
    #include <random>

    using namespace std;

    int rNode(int min, int max) {
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<int> dist(min, max);
        return dist(mt);
    }

    double numb() {
        random_device rd;
        mt19937 mt(rd());
        uniform_real_distribution<double> dist(0, 1);
        return dist(mt);
    }

    void updateProbabilities(vector<double>* probabilities, const vector<double>& alphas, int bestWeight, vector<pair<double,int>>& avgWeights) {
        for (int i = 0; i < alphas.size(); i++) {
            double p = avgWeights[i].first / avgWeights[i].second;
            if (bestWeight > 0) {
                p = p / bestWeight;
            }
            (*probabilities)[i] = p;
        }
    }

    double chooseAlpha(vector<double>* probabilities, const vector<double>& alphas) {
        std::discrete_distribution<> d(probabilities->begin(), probabilities->end());
        int index = d(std::mt19937{std::random_device{}()});
        return alphas[index];
    }

    void updateAvgWeights(vector<pair<double,int>>& avgWeights, const vector<double>& alphas, double alpha, int currentWeight) {
        int index = -1;
        for (int i = 0; i < alphas.size(); i++) {
            if (alphas[i] == alpha) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            avgWeights[index].first += currentWeight;
            avgWeights[index].second++;
        }
    }

    int main() {
        // list of possible alphas
        vector<double> alphas{0.1, 0.2, 0.3, 0.4, 0.5};

        // list of probabilities
        vector<double> probabilities(alphas.size(), 1.0/alphas.size());

        // average of weights for each alpha
        vector<pair<double,int>> avgWeights;

        // set for the best solution
        set<pair<int,int>> bestSolutionSet;

        // set for the current solution
        set<pair<int,int>> auxSolutionSet;

        // map to verify if node is in solution
        map<int,bool> in_solution;

        int numIter = 100;
        int block_size = 10;
        int i=1, currentWeight = 0, bestWeight = INT_MAX;
        unsigned int k;

        // initialize vector of average weights with 0
        for (int j = 0; j < alphas.size(); j++) {
            avgWeights.push_back(make_pair(0,0));
        }

    double alpha = alphas.at(rNode(0, alphas.size()-1));
    cout << alpha
*/