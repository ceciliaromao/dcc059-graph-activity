#include<iostream>
#include "../Graph.h"
#include "../config.h"
#include <set>
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

void printNodesGreedy(set<pair<int,int>> solucao,ofstream&op)
{
    for(auto i = solucao.begin(); i != solucao.end(); i++)
    {
        op<<"Nó:" <<i->first<<" "<<"Peso: " <<i->second<<endl;
    }
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

int main()
{
    Graph* graph;
    ifstream input;
    
    string path = USER_DIR;
    string path_in = path;

    path_in+="input/grafo_8_weighted.txt";

    string path_out =path+ "output/output.txt";
    input.open(path_in, ios::in);

    ofstream output;

    output.open(path_out, ios::out | ios::trunc);
    graph = leituraInstancia(input, 0, 0, 1);

    printNodesGreedy(graph->GreedyConstructive(),output);
}