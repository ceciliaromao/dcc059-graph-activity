#include<iostream>
#include "../Graph.h"
#include "../config.h"
#include <set>
#include <string>
#include <string.h>
#include <sstream>

using namespace std; 

// int input_file[] = {6,0, 2, 3,0, 4, 1, 0, 5, 2, 1, 5, 4, 2, 3, 6, 2, 4, 3, 4, 5, 7};

Graph* leituraInstancia(ifstream&input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int weightNodeSource;
    int weightNodeTarget;
    int order;
    int numEdges;

    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    if(weightedNode){
        
        while(input_file >> idNodeSource >> weightNodeSource >> idNodeTarget >> weightNodeTarget) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(weightNodeSource);
            graph->getNode(idNodeTarget)->setWeight(weightNodeTarget);
        }
    } else {
        while(input_file >> idNodeSource >> idNodeTarget) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);

        }
    }
    return graph;
}

void printEdges(Graph *graph,ofstream&op)
{
    Node *aux = graph->getFirstNode();

    while(aux != nullptr)
    {
        for(Edge *i = aux->getFirstEdge(); i != nullptr; i = i->getNextEdge())
        {
            op<<i->getTargetId()<<endl;
        }
        aux = aux->getNextNode();
    }
}

void printNodesGreedy(set<pair<int,int>> solucao,ofstream&op, int instance)
{
    int sumOfWeights = 0;
    for(auto i : solucao)
    {
        sumOfWeights += i.second;
        op<<"No:" <<i.first<<" "<<"Peso: " <<i.second<<endl; 
    }
    cout << "Soma dos pesos: " << sumOfWeights << endl;
    if (instance < sumOfWeights)
        cout << "A solucao e " << (sumOfWeights / (double)instance) - 1 << " pior" << endl;
    else if (instance > sumOfWeights)
        cout << "A solucao e melhor! A literatura e " << (instance / (double)sumOfWeights) - 1 << " pior" << endl;
    else
        cout << "A soulcao e igual a literatura." << endl;
}

void printGraph(ofstream&op, Graph *graph){
    int order = graph->getOrder();

    for(int i = 1; i < order; i++){
        Node *aux = graph->getNode(i);
        for(Edge* j = aux->getFirstEdge(); j != nullptr; j = j->getNextEdge()){
            op<< i << " " << j->getTargetId() << endl;
        }
    }
}

Graph* readAdjacencyMatrix(ifstream&input_file,int directed, int weightedEdge, int weightedNode){
    int order;
    input_file.seekg(sizeof("NumberOfNodes:"),ios::beg);

    input_file >> order;
    order= order+1;
    // input_file.seekg(sizeof("Positions"),ios::cur);

    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);
    string line, aux;

    int skip_positions = order+2;
    int i = 0;

    //skips the positions on the input file

    while(i < skip_positions){
        
        getline(input_file,line);
        i++;
        
    }
    line ="";
    i = 0;
    
    while(i < order-1){
        i++;
        getline(input_file, line);
        stringstream stst(line);
        getline(stst,aux);
        graph->insertNode(i);
        graph->getNode(i)->setWeight(stoi(aux));
    }

    getline(input_file, line);

    int currentNode = 1;
    int targetNode = 0;
    line = "";
    cout << "Ordem de inserção: "<< endl;
    while(!input_file.eof()){
        getline(input_file, line);
        stringstream stst(line);
        while(getline(stst,aux,' ')){
            
            targetNode++;
            
            if(aux == "1"){
                if(currentNode == targetNode) continue;
                // cout<<currentNode<<" " << targetNode<<endl;
                graph->insertEdge(currentNode,targetNode,0);
                
            }
        }
        currentNode++;
        targetNode = 0;
    }
    cout << "Numero de arestas: " << graph->getNumberEdges() << endl;
    return graph;
}
enum {
    SMALL_1 = 552,
    SMALL_2 = 192,
    SMALL_3 = 672,
    MEDIUM_1 = 432,
    MEDIUM_2 = 1608,
    MEDIUM_3 = 1640,
    MEDIUM_4 = 2723,
    LARGE_1 = 1533,
    LARGE_2 = 1825,
    LARGE_3 = 1824 
};

int main()
{
    Graph* graph;
    ifstream input;
    
    string path = USER_DIR;
    string path_in = path;

    path_in+="input/dominating_set/Problem.dat_500_500_3";

    string path_out =path+ "output/output.txt";
    input.open(path_in, ios::in);

    ofstream output;
    graph = readAdjacencyMatrix(input, 0, 0, 1);
    
    output.open(path_out, ios::out | ios::trunc);
    // graph = leituraInstancia(input, 0, 0, 1);
    // printGraph(output,graph);
    set<pair<int,int>> solucao = graph->GreedyConstructive();


    // graph->writeDotFile(path_out);

    printNodesGreedy(solucao,output,SMALL_1);
   
}