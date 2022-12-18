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

    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    if(weightedEdge){
        float weight;
        while(input_file >> idNodeSource >> idNodeTarget >> weight) {

            graph->insertEdge(idNodeSource, idNodeTarget, weight);

        }
    } else {
        while(input_file >> idNodeSource >> idNodeTarget) {

            graph->insertEdge(idNodeSource, idNodeTarget, 1);

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

void printDegrees(Graph *graph,ofstream&op)
{
    Node *aux = graph->getFirstNode();

    while(aux != nullptr)
    {
        op<< "No:"<<aux->getId()<<endl<<"Grau de Entrada:" <<aux->getInDegree()<<"; Grau de Saida:"<<aux->getOutDegree()<<endl;
        aux = aux->getNextNode();
    }
}

void executeWarshall(ofstream&output, Graph* graph){
    int source =1;

    float** array= graph->floydWarshall();
    int iterative=graph->getOrder()-1;
    for(int i=0;i<iterative;i++){
        for(int j=0;j<iterative;j++){
            output<<array[i][j]<<" ";
        }
        output<<endl;
    }
}

int main()
{
    Graph* graph;
    ofstream output;
    ifstream input;
    
    string path = USER_DIR;
    string path_in = path;

    cout << "É ponderado? 1 - Sim, 0 - Não" << endl;
    int ponderado = 0;
    cin >> ponderado;
    
    ponderado ? path_in +="input/grafo_3.txt": path_in+="input/grafo_10.txt";

    string path_out =path+ "output/output.txt";

    output.open(path_out, ios::out | ios::trunc);
    input.open(path_in, ios::in);

    if(ponderado)
        graph = leituraInstancia(input, 1, 1, 0);
    else
        graph = leituraInstancia(input, 0, 0, 0);

    output<< "Array Warshall:"<<endl;

    executeWarshall(output, graph);

}