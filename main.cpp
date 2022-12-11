#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;

Graph* leitura(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    if(!graph->getWeightedEdge() && !graph->getWeightedNode()){

        while(input_file >> idNodeSource >> idNodeTarget) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);

        }

    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() ){

        float edgeWeight;

        while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);

        }

    }else if(graph->getWeightedNode() && !graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }else if(graph->getWeightedNode() && graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }

    return graph;
}

Graph* leituraInstancia(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    while(input_file >> idNodeSource >> idNodeTarget) {

        graph->insertEdge(idNodeSource, idNodeTarget, 0);

    }

    return graph;
}

int menu(){

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Complementar do grafo" << endl;
    cout << "[2] Imprimir caminhamento em largura" << endl;
    cout << "[3] Busca em profundidade" << endl;
    cout << "[4] Imprimir componentes conexas" << endl;
    cout << "[5] Imprimir componentes fortemente conexas" << endl;
    cout << "[6] Imprimir ordenacao topológica" << endl;
    cout << "[8] Caminho Mínimo Dijkstra" << endl;
    cout << "[9] Caminho Mínimo Floyd" << endl;
    cout << "[10] Grafo União(Com grafo complementar)" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void printGraph(ofstream&op, Graph *graph){
    int order = graph->getOrder();

    for(int i = 1; i < order; i++){
        Node *aux = graph->getNode(i);
        for(Edge* j = aux->getFirstEdge(); j != nullptr; j = j->getNextEdge()){
            op<< i << " " << j->getTargetId() << " " << j->getWeight() << endl;
        }
    }
}

void executeDijkstra(ofstream&output, Graph* graph){
    int source =14;

    float* array= graph->dijkstra(source);
    int iterative=graph->getOrder();
    for(int i=1;i<iterative;i++){
        output<<array[i]<<endl;
    }
}


void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {

        //Complementar
        case 1:{
            Graph* complementar = graph->getComplement();
            printGraph(output_file,complementar);
            break;
        }

        //BFS
        case 2:{
           
            break;
        }

        //DFS
        case 3:{
            
            break;
        }

        //Componentes Conexas
        case 4:{

            break;
        }

        //Componentes Fortementes Conexas
        case 5:{
            
            break;
        }

        //Ordenação Topológica
        case 6:{
            
            break;
        }


        //Algoritmo de Prim
        case 7:
        {
            
            break;
        }

        //Algoritmo de Dijkstra
        case 8:
        {
            executeDijkstra(output_file, graph);
            break;
        }

        //Algoritmo de Floyd
        case 9:
        {
            
            break;

        }

        // Grafo União
        case 10:
        {
            Graph * complement = graph->getComplement();
            Graph* unionG = graph->getUnion(complement);
            printGraph(output_file,unionG);
        }

  }
}

int mainMenu(ofstream& output_file, Graph* graph){

    int selecao = 1;

    while(selecao != 0){
        system("clear");
        selecao = menu();

        if(output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;

    }

    return 0;
}




int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 6) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    

    Graph* graph;

    if(input_file.is_open()){

        graph = leituraInstancia(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    }else
        cout << "Unable to open " << argv[1];

    
    mainMenu(output_file, graph);

    

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}
