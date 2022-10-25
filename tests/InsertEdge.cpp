#include<iostream>
#include "Graph.h"

using namespace std; 

int input_file[] = {6,0, 2, 3,0, 4, 1, 0, 5, 2, 1, 4, 2, 1, 5, 4, 2, 3, 6, 2, 4, 3, 4, 5, 7};

Graph* leituraInstancia(int *input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order = input_file[0];
    int numEdges;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    for(int i = 1; i < 25 ; i+=3)
    {
        graph->insertNode(input_file[i]);
        graph->insertNode(input_file[i+1]);
        graph->insertEdge(input_file[i], input_file[i+1], input_file[i+2]);
    }

    return graph;
}

void printEdges(Graph *graph)
{
    Node *aux = graph->getFirstNode();

    while(aux != nullptr)
    {
        for(Edge *i = aux->getFirstEdge(); i != nullptr; i = i->getNextEdge())
        {
            cout<<i->getTargetId()<<endl;
        }
        aux = aux->getNextNode();
    }
}
int main()
{
    Graph* graph;

    graph = leituraInstancia(input_file, 0, 0, 0);
    
    cout<<"O programa foi executado"<<endl;
    printEdges(graph);
}