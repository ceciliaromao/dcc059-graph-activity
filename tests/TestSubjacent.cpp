#include<iostream>
#include "../Graph.h"
#include "../config.h"

using namespace std; 

// int input_file[] = {6,0, 2, 3,0, 4, 1, 0, 5, 2, 1, 5, 4, 2, 3, 6, 2, 4, 3, 4, 5, 7};


Graph* leituraInstancia(ifstream&input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    input_file >> order >> numEdges;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    while(input_file >> idNodeSource >> idNodeTarget) {

        graph->insertEdge(idNodeSource, idNodeTarget, 0);

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

void printDegrees(Graph *graph,ofstream&op)
{   
    int order = graph->getOrder();
    for(int i= 1; i < order; i++)
    {
        Node *aux = graph->getNode(i);
        op<<"Nó: "<< aux->getId()<<endl << "Grau de Entrada: "<<aux->getInDegree()<< " Grau de Saída:"<<aux->getOutDegree()<<endl<<endl;
    }
}

int main()
{
    Graph* graph;
    ofstream output;
    ifstream input;
    
    string path = USER_DIR;
    string path_out =path+ "output/output.txt";
    string path_in = path+"input/grafo_1000_1.txt";

    output.open(path_out, ios::out | ios::trunc);
    input.open(path_in, ios::in);

    graph = leituraInstancia(input, 1, 0, 0);

    output<< "Grafo Subjacente:"<<endl;

    printDegrees(graph->getSubjacent(),output);

    output<<"Grafo Original "<<endl;

    printDegrees(graph,output);
}