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


int main()
{
    Graph* graph;
    ifstream input;
    
    string path = USER_DIR;
    string path_in = path;

  
    path_in +="input/grafo_10.txt";

    input.open(path_in, ios::in);

   
    graph = leituraInstancia(input, 0, 0, 0);


    cout << "Grafo conexo? " << graph->connectedGraph() << endl;
}