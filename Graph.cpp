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

            /* next = this->first_node;

            while (next != nullptr)
            {
                aux = next;
                next = next->getNextNode();
            }

            aux->setNextNode(node) */
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
    if(!this->searchNode(id))
    {
        cout << "Node not found" << endl;
        return;
    }

    Node *current_node = this->getFirstNode();
    Node *previous_node = nullptr;

    while (current_node->getId() != id)
    {
        previous_node = current_node;
        current_node = current_node->getNextNode();
    }

    if (previous_node == nullptr)
        this->setFirstNode(current_node->getNextNode());
    else
        previous_node->setNextNode(current_node->getNextNode());

    if (current_node->getNextNode() == nullptr)
        this->setLastNode(previous_node);
        
    current_node->removeAllEdges(this->directed);

    // delete current_node;
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

bool Graph::depthFirstSearch(int initialId, int targetId, ofstream &output_file)
{
    //Confere casos triviais: não possuir o nó incial ou target / ou eles serem iguais
    if(getNode(initialId) == nullptr || getNode(targetId) == nullptr)
        return false; 
    else if(initialId == targetId)
        return true;
    
    stack<int> pilha;
    pilha.push(initialId);
    stack<int> auxPilha; 
    string edge_symbol; 

    verified.clear();

    //Escrita em arquivo dot
    if(output_file.is_open())
    {
        //escrevendo o grafo em dot
        if(this->directed)
        {
            output_file<<"digraph{"<<endl;
            edge_symbol = "->";
        }
        else
        {
            output_file<<"strict graph{"<<endl;
            edge_symbol = "--";
        }
    }else
    {
        //Mensagem de aviso: não para o código
        cout<<"Depth First Search: Arquivo de saída não aberto"<<endl;
    }

int current;
    //enquanto a pilha não estiver vazia
    while(!pilha.empty())
    {
        current = pilha.top();
        pilha.pop();
        //atualiza o nó a ser analizado e tira ele da pilha
        if(!verified[current])
        {
            output_file<<current; //saida no arquivo
            
            verified[current] = true; 
            if(current == targetId) 
            {
                output_file<<" }"<<endl; //saida no arquivo
                return true;
            }
            if(!pilha.empty())
                output_file<<edge_symbol; //saida no arquivo
            
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
    output_file.seekp(-sizeof(edge_symbol.c_str()), ios::end);
    output_file<<"           }"<<endl; //saida no arquivo
    return false;
}

//? Essa função começa a procurar a partir de onde?
void Graph::breadthFirstSearch(ofstream &output_file){
    list<int> queue;
    int ak = this->getFirstNode()->getId();
    verified[ak] = true;
    queue.push_back(ak);
    string edge_symbol;

    verified.clear();

    if(output_file.is_open())
    {
        //escrevendo o grafo em dot
        if(this->directed)
        {
            output_file<<"digraph{"<<endl;
            edge_symbol = "->";
        }
        else
        {
            output_file<<"strict graph{"<<endl;
            edge_symbol = "--";
        }
    }else
    {
        //Mensagem de aviso: não para o código
        cout<<"Breadth First Search: Arquivo de saída não aberto"<<endl;
    }

    Node* aux;
    while(!queue.empty())
    {
        ak = queue.front();
        queue.pop_front();

        aux = this->getNode(ak);
        for(Edge *i = aux->getFirstEdge(); i != nullptr; i = i->getNextEdge())
        {
            if(aux->getId() == i->getTargetId())
                continue;

            //Desenha TODAS as arestas do grafo
            output_file<<aux->getId()<<edge_symbol<<i->getTargetId();
            //cout<<i->getTargetId()<<endl;
            if (!verified[i->getTargetId()])
            {
                output_file<<"[color=\"red\"]"<<endl;
                verified[i->getTargetId()] = true;
                queue.push_back(i->getTargetId());
            }else
                output_file<<endl;
        }
    }
    output_file<<"}"<<endl;
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

//A function that returns the intersection of two graphs
Graph* Graph::getIntersection (Graph* graph){

    //checks if the graphs are compatible
    if(this->getDirected() != graph->getDirected() || this->getWeightedEdge() != graph->getWeightedEdge() || this->getWeightedNode() != graph->getWeightedNode())
    {
        return nullptr;
    }

    Graph *intersectionGraph;
    if(this->order < graph->order){
        intersectionGraph = new Graph(this->order, this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
        for(Node *i = this->first_node; i!=nullptr; i = i->next_node){
            int nodeId = i->getId();
            Node *graphNode = graph->getNode(nodeId);
            if(graphNode == nullptr) continue;

            for(Edge *j = i->getFirstEdge(); j != nullptr; j = j->getNextEdge())
            {
                if(graphNode->searchEdge(j->getTargetId())){
                    intersectionGraph->insertEdge(nodeId, j->getTargetId(), 0);
                }
            }
        }
    } else {
        intersectionGraph = new Graph(graph->order, graph->getDirected(), graph->getWeightedEdge(), graph->getWeightedNode());
        for(Node *i = graph->first_node; i!=nullptr; i = i->next_node){
            int nodeId = i->getId();
            Node *thisNode = this->getNode(nodeId);
            if(thisNode == nullptr) continue;

            for(Edge *j = i->getFirstEdge(); j != nullptr; j = j->getNextEdge())
            {
                if(thisNode->searchEdge(j->getTargetId())){
                    intersectionGraph->insertEdge(nodeId, j->getTargetId(), 0);
                }
            }
        }
    }

    return intersectionGraph;
}

//A function that checks if a graph is a subgraph of another graph
bool Graph::isSubgraph(Graph* graph){
    
    for(Node *i = this->first_node; i!=nullptr; i = i->next_node){
        int nodeId = i->getId();
        Node *graphNode = graph->getNode(nodeId);

        if(graphNode == nullptr) continue;

        for(Edge *j = i->getFirstEdge(); j != nullptr; j = j->getNextEdge())
        {
            if(!graphNode->searchEdge(j->getTargetId())){
                return false;
            }
        }
    }

    return true;
}

//A function that returns the difference of two graphs
Graph * Graph::getDifference(Graph* graph){

    //checks if the graphs are compatible
    if(this->getDirected() != graph->getDirected() || this->getWeightedEdge() != graph->getWeightedEdge() || this->getWeightedNode() != graph->getWeightedNode())
        return nullptr;

    cout << "Os grafos são compatíveis." << endl;

    //checks if the graphs have the same order
    if(this->getOrder() != graph->getOrder()) {
        cout << "Os grafos não têm a mesma ordem." << endl;
        return nullptr;
    }

    // checks if one graph is subgraph of the other
    if (this->isSubgraph(graph))
    {
        cout << "Um grafo é subgrafo do outro." << endl;
        return new Graph(0, this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    }

    int graphOrder = graph->getOrder();

    Graph *differenceGraph = new Graph(graphOrder, this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    cout << "Instancia um grafo de ordem " << graphOrder << endl;

    // runs through the nodes of the graph
    cout << "Percorre os nós desse grafo" << endl << endl;
    for(Node *i = this->getFirstNode(); i!=nullptr; i = i->getNextNode()){
        int nodeId = i->getId();
        Node *graphNode = graph->getNode(nodeId);
        cout << "Instancia o nó " << nodeId << " desse grafo" << endl;
        cout << "Instancia o nó " << graph->getNode(nodeId) << " do outro grafo" << endl;
        
        if(graphNode == nullptr){
            differenceGraph->insertNode(nodeId);
            if(this->getWeightedNode())
                differenceGraph->getNode(nodeId)->setWeight(i->getWeight());
            cout << "O nó " << nodeId << " não existe no outro grafo. Insere no diferença" << endl;
            if (differenceGraph->searchNode(nodeId))
                cout << "entrou no grafo" << endl;
            continue;
        }

        cout << "Percorre as arestas do nó " << nodeId << endl;
        for(Edge *j = i->getFirstEdge(); j != nullptr; j = j->getNextEdge())
        {
            cout << "Aresta de " << nodeId << " para " << j->getTargetId() << endl;
            if(!graphNode->searchEdge(j->getTargetId())){
                differenceGraph->insertEdge(nodeId, j->getTargetId(), j->getWeight());
                cout << "Aresta não existe no outro grafo. Insere no diferença" << endl;
            }
        }
        cout << "Fim do nó " << nodeId << endl << endl;
    }

    cout << "Fim do grafo" << endl << endl;

    return differenceGraph;
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
            if((node->searchEdge(i->id) == false) || (i->searchEdge(node->id) == false)){
                if(i->id == node->id)
                    continue;
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

void Graph::pert(string path_out)
{
    //verificando condições de existência
    if(!this->directed || !this->weighted_edge)
    {
        cout<<"Rede PERT: não é possível fazer algoritmo PERT: \n Grafo não direcionado ou não ponderado nas arestas"<<endl;
        return; 
    }

    if(this->hasCircuit())
    {
        cout<<"Rede PERT: não é possível fazer algoritmo PERT: \n Grafo possui circuito"<<endl; 
        return;
    }

    vector<pertTask> sol; //esse vetor, tem que em algum momento, ter todos os nós. 
    vector<int> criticPath; 
    int auxDegree = 0; 
    int min, max; 
    int pesoAresta; 
    int maxAlpha = 0;
    int custo = 0;

    //Pega todos os nós com grau de entrada igual a 0
    for(Node* aux = this->first_node; aux!=nullptr; aux = aux->next_node)
    {
        if(aux->getInDegree() == 0 )
        {
            int j = isIn(sol, aux->getId());
            if(j==-1)
            {
                //adiciona novo nó na solução
                pertTask nTarefa;
                nTarefa.id = aux->getId();
                nTarefa.a = 0;
                sol.push_back(nTarefa);
            }else
            {
                //atualiza a informação da tarefa j da solução
                sol[j].a = 0; 
            }
        }
    }

    
    for(Node *aux = this->first_node; aux!=nullptr; aux = aux->next_node)
    {
        auxDegree = aux->getInDegree();
        max = 0; 
        for(int i = 0; i<sol.size(); i++)
        {
            if(getNode(sol[i].id)->searchEdge(aux->getId()))
            {
                auxDegree--;
                pesoAresta = getNode(sol[i].id)->hasEdgeBetween(aux->getId())->getWeight();
                if(sol[i].a + pesoAresta > max)
                {
                    max = sol[i].a + pesoAresta; 
                }
            }
        }

        //Todos os antecedentes estão na solução
        if(auxDegree == 0 && isIn(sol, aux->getId()) == -1)
        {
            cout<<"Adicionando "<<aux->getId()<<" na solução"<<endl;
            pertTask nTarefa;
            nTarefa.id = aux->getId();
            nTarefa.a = max;
            sol.push_back(nTarefa);

            //Reinicia o loop para garantir que todos os nós estão na solução
            aux =  this->first_node; 
        }
    }
    
    
    //!Define nós terminais do grafo
    for(Node* aux = this->first_node; aux!=nullptr; aux = aux->next_node)
    {
        if(aux->getOutDegree() == 0 )
        {
            int j = isIn(sol, aux->getId());
            //atualiza a informação da tarefa j da solução
            sol[j].b = sol[j].a; 
            if(sol[j].a > maxAlpha)
            {
                maxAlpha = sol[j].a;
            }
        }
    }
    
    //parte do betha
    Node *aux;
    for(int j=sol.size() - 1; j>-1; j--)
    {
        aux = this->getNode(sol[j].id);
        auxDegree = aux->getOutDegree();
        min = maxAlpha; 
        for(int i = 0; i<sol.size(); i++)
        {
            if(aux->searchEdge(sol[i].id))
            {
                auxDegree--;
                pesoAresta = aux->hasEdgeBetween(sol[i].id)->getWeight();
                if(sol[i].b - pesoAresta < min )
                {
                    min = sol[i].b - pesoAresta; 
                }
            }
        }
                            
        //Todos os precedentes estão na solução
        if(auxDegree == 0)
        {
            int j = isIn(sol, aux->getId());
            //atualiza a informação da tarefa j da solução
            sol[j].b = min; 
        }
    }    

    for(int i =0; i<sol.size(); i++)
    {
        if(sol[i].b - sol[i].a == 0)
        {
            criticPath.push_back(sol[i].id);
        }
    }

    for(int i =0; i<criticPath.size()-1; i++)
    {
        custo += this->getNode(criticPath[i])->hasEdgeBetween(criticPath[i+1])->getWeight(); 
    }

    //impressão da ordem de execução e de alpha e beta
    ofstream output;
    output.open(path_out, ios::out | ios::trunc);
    if(!output.is_open())
    {
        cout<<"REDE PERT: erro ao abrir o arquivo de saída"<<endl;
        return; 
    }

    output<<"Id: ";
    for(int i =0; i<sol.size(); i++)
    {
        output<<sol[i].id<<" ";
    }

    output<<endl<<"a:  ";
    for(int i =0; i<sol.size(); i++)
    {
        output<<sol[i].a<<" ";
    }

    output<<endl<<"b:  ";
    for(int i =0; i<sol.size(); i++)
    {
        output<<sol[i].b<<" ";
    }

    output<<endl<<endl<<"Caminho crítico: ";
    for(int i =0; i<criticPath.size(); i++)
    {
        output<<criticPath[i]<<" ";
    }

    output<<endl<<"Custo total: "<<custo;
    output.close();
}

int Graph::isIn(vector<pertTask> sol, int id)
{
    for(int i =0; i<sol.size(); i++)
        if(sol[i].id == id)
            return i;

    return -1; 
}