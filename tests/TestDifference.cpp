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

void printDegrees(Graph *graph,ofstream&op)
{
    Node *aux = graph->getFirstNode();

    while(aux != nullptr)
    {
        op<< "No:"<<aux->getId()<<endl<<"Grau de Entrada:" <<aux->getInDegree()<<"; Grau de Saida:"<<aux->getOutDegree()<<endl;
        aux = aux->getNextNode();
    }
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

int main()
{
    Graph* graph1, * graph2;
    ofstream output;
    ifstream input;
    
    string path = USER_DIR;
    string path_out =path+ "output/output.txt";

    string path_in1 = path;
    string path_in2 = path;

    path_in1+="input/grafo_4_1.txt";
    path_in2+="input/grafo_4_2.txt";

    input.open(path_in1, ios::in);
    graph1 = leituraInstancia(input, 0, 1, 0); 
    input.close();

    input.open(path_in2, ios::in);
    graph2 = leituraInstancia(input, 0, 1, 0);
    input.close();

    output.open(path_out, ios::out | ios::trunc);
    output<<"Grafo 1:"<<endl;
    printGraph(output, graph1);

    output<<"Grafo 2:"<<endl;
    printGraph(output, graph2);

    output<< "Grafo Diferença:"<<endl;
    Graph *differenceG2 = graph1->getDifference(graph2);
    if (differenceG2 != nullptr) {
        printGraph(output, differenceG2);

        graph1->writeDotFile(path_out);
        graph2->writeDotFile(path_out);
        differenceG2->writeDotFile(path_out);

        cout << "edges: " << graph1->getNumberEdges() << " | order: " << graph1->getOrder() - 1 << endl;
        cout << "edges: " << graph2->getNumberEdges() << " | order: " << graph2->getOrder() - 1 << endl;
        cout << "edges: " << differenceG2->getNumberEdges() << " | order: " << differenceG2->getOrder() - 1 << endl;
    } else {
        cout << "Erro ao calcular a diferença! Insira grafos válidos!" << endl;
    }
}