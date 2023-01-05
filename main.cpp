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
    cout << "[1] Grafo Interseção" << endl;
    cout << "[2] Grafo União" << endl;
    cout << "[3] Grafo Diferença" << endl;
    cout << "[4] Rede PERT" << endl;
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

string leGrafo2(Graph* graph){
    string nomeArquivo;
    cout << "O Grafo 1: ";
    string texto = (graph->getDirected()) ? "é direcionado, " : "não é direcionado, ";
    texto += (graph->getWeightedEdge()) ? "possui arestas ponderadas, " : "não possui arestas ponderadas, ";
    texto += (graph->getWeightedNode()) ? "e possui nós ponderados." : "e não possui nós ponderados.";
    cout << texto << endl;
    cout << "Informe o nome do arquivo do Grafo 2: " << endl;
    cin >> nomeArquivo;
    return nomeArquivo;
}

void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {

        // Interseção
        case 1:{
            cout << "-----------" << endl;
            cout << "Grafo Interseção" << endl;
            cout << "-----------" << endl;

            string nomeArquivo = leGrafo2(graph);

            ifstream input_file;
            input_file.open(nomeArquivo, ios::in);

            Graph* graph2 = leituraInstancia(input_file, graph->getDirected(), graph->getWeightedEdge(), graph->getWeightedNode());
            Graph* interG = graph->getIntersection(graph2);
            printGraph(output_file,interG);
            
            interG->writeDotFile("grafoIntersecao.txt");

            cout << "Grafo Interseção gerado com sucesso no arquivo de saída!" << endl;
            cout << "-----------" << endl;
            cout << "Voltar para menu? (s/n) ";
            char resposta;
            cin >> resposta;
            cout << "-----------" << endl;
            if (resposta == 'n')
                exit(0);

            break;
        }

        // União
        case 2:{
            cout << "-----------" << endl;
            cout << "Grafo União" << endl;
            cout << "-----------" << endl;

            string nomeArquivo = leGrafo2(graph);

            ifstream input_file;
            input_file.open(nomeArquivo, ios::in);

            Graph* graph2 = leituraInstancia(input_file, graph->getDirected(), graph->getWeightedEdge(), graph->getWeightedNode());
            Graph* unionG = graph->getUnion(graph2);
            printGraph(output_file,unionG);

            unionG->writeDotFile("grafoUniao.txt");

            cout << "Grafo União gerado com sucesso no arquivo de saída!" << endl;
            cout << "-----------" << endl;
            cout << "Voltar para menu? (s/n) ";
            char resposta;
            cin >> resposta;
            cout << "-----------" << endl;
            if (resposta == 'n')
                exit(0);

            break;
        }

        // Diferença
        case 3:{
            cout << "-----------" << endl;
            cout << "Grafo Diferença" << endl;
            cout << "-----------" << endl;

            string nomeArquivo = leGrafo2(graph);

            ifstream input_file;
            input_file.open(nomeArquivo, ios::in);

            Graph* graph2 = leituraInstancia(input_file, graph->getDirected(), graph->getWeightedEdge(), graph->getWeightedNode());
            Graph* differenceG = graph->getDifference(graph2);
            printGraph(output_file,differenceG);

            differenceG->writeDotFile("grafoDiferenca.txt");

            cout << "Grafo Diferença gerado com sucesso no arquivo de saída!" << endl;
            cout << "-----------" << endl;
            cout << "Voltar para menu? (s/n) ";
            char resposta;
            cin >> resposta;
            cout << "-----------" << endl;
            if (resposta == 'n')
                exit(0);

            break;
        }

        // Rede PERT
        case 4:{
            cout << "-----------" << endl;
            cout << "Rede PERT" << endl;
            cout << "-----------" << endl;

            graph->pert(output_file);
            cout << "Rede PERT gerada com sucesso no arquivo de saída!" << endl;
            cout << "-----------" << endl;
            cout << "Voltar para menu? (s/n) ";
            char resposta;
            cin >> resposta;
            cout << "-----------" << endl;

            if (resposta == 'n')
                exit(0);

            break;
        }

        // Sair do programa
        case 0:{
            "Fim do programa";
            break;
        }

        default:{
            cout << "Opção inválida" << endl;
            break;
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

    if(input_file.is_open())
        graph = leituraInstancia(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    else
        cout << "Unable to open " << argv[1];

    mainMenu(output_file, graph);
    
    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}
