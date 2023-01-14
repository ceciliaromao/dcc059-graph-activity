#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include <string.h>
#include <set>
#include <sstream>
#include "Graph.h"
#include "Node.h"

using namespace std;

// Resultados da literatura
#define SMALL_1 552;
#define SMALL_2 192;
#define SMALL_3 672;
#define MEDIUM_1 432;
#define MEDIUM_2 1608;
#define MEDIUM_3 1640;
#define MEDIUM_4 2723;
#define LARGE_1 1533;
#define LARGE_2 1825;
#define LARGE_3 1824;

// Funções
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

int getLiteratureSolution(string input_file_name) 
{
    int instance;

    if (strcmp(input_file_name.c_str(), "./input/Problem.dat_50_50_3") == 0) {
        instance = SMALL_1;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_50_250_3") == 0) {
        instance = SMALL_2;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_100_250_3") == 0) {
        instance = SMALL_3;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_100_500_3") == 0) {
        instance = MEDIUM_1;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_150_150_3") == 0) {
        instance = MEDIUM_2;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_250_750_3") == 0) {
        instance = MEDIUM_3;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_300_500_3") == 0) {
        instance = MEDIUM_4;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_300_2000_3") == 0) {
        instance = LARGE_1;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_500_500_0") == 0) {
        instance = LARGE_2;
    } else if (strcmp(input_file_name.c_str(), "./input/Problem.dat_500_500_3") == 0) {
        instance = LARGE_3;
    } else  {
        instance = 0;
    }

    return instance;
}

// Faz 10 soluções diferentes e imprime a média dos pesos encontrados 
void printNodesRandomReactiveGreedy(Graph* graph, ofstream&op, string input_file_name, vector<double> alphas, int numIterations, int block_size) {
    int instance = getLiteratureSolution(input_file_name);

    double avgWeights = 0;      // Média dos pesos de todas as soluções
    int sumOfWeights = 0;       // Soma dos pesos de uma solução
    double avgTime = 0;         // Média do tempo de execução de todas as soluções
    
    for (int i = 0; i < 10; i++) {
        auto t1 = chrono::high_resolution_clock::now();
        set<pair<int,int>> solucao = graph->GreedyRandomizedReactive(alphas, numIterations, block_size);
        auto t2 = chrono::high_resolution_clock::now();
        auto elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
        avgTime += elapsed_seconds;

        op << "Solucao " << i+1 << endl;

        // soma os pesos de cada nó da solução
        for(auto i : solucao){
            sumOfWeights += i.second;
            op<<"No:" << i.first << " | " << "Peso: " << i.second << endl; 
        }   

        op << "Soma dos pesos da solução: " << sumOfWeights << endl;
        op << "--------" << endl;

        avgWeights += sumOfWeights;
        sumOfWeights = 0;
    }

    avgWeights = avgWeights / 10.0;
    avgTime = avgTime / 10.0;

    op << "Média dos valores das soluções: " << avgWeights << endl;
    cout << "Média dos valores das soluções: " << avgWeights << endl;
    cout << "Soma dos pesos da melhor solução da literatura: " << instance << endl;
    cout << "--------" << endl;

    if (instance < avgWeights) {
        double qualidade = (avgWeights - instance) / (double)instance;
        op << "A solucao tem média de desempenho " << qualidade << " pior que o da literatura." << endl;
        cout << "A solucao tem média de desempenho " << qualidade << " pior que o da literatura." << endl;
    } else if (instance > avgWeights) {
        double qualidade = (instance - avgWeights) / avgWeights;
        op << "A média da solucao é melhor que a da literatura! O desempenho da literatura foi " << qualidade << " pior." << endl;
        cout << "A média da solucao e melhor que a da literatura! O desempenho da literatura foi " << (instance - sumOfWeights) / (double)sumOfWeights << " pior." << endl;
    }
    else {
        op << "A média das soluções tem desempenho igual ao da literatura." << endl;
        cout << "A média das soluções tem desempenho igual ao da literatura." << endl;
    }

    cout << "--------" << endl;

    op << "Média do tempo de execução: " << avgTime << "s." << endl;
    cout << "Média do tempo de execução: " << avgTime << "s." << endl;

    cout << "--------" << endl;
}

// Faz 10 soluções diferentes e imprime a média dos pesos encontrados 
void printNodesRandomGreedy(Graph* graph, ofstream&op, string input_file_name, double alpha, int numIterations) {
    int instance = getLiteratureSolution(input_file_name);

    double avgWeights = 0;      // Média dos pesos de todas as soluções
    int sumOfWeights = 0;       // Soma dos pesos de uma solução
    double avgTime = 0;         // Média do tempo de execução de todas as soluções
    
    for (int i = 0; i < 10; i++) {
        auto t1 = chrono::high_resolution_clock::now();
        set<pair<int,int>> solucao = graph->GreedyRandomizedAdaptive(alpha, numIterations);
        auto t2 = chrono::high_resolution_clock::now();
        auto elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
        avgTime += elapsed_seconds;

        op << "Solucao " << i+1 << endl;

        // soma os pesos de cada nó da solução
        for(auto i : solucao){
            sumOfWeights += i.second;
            op<<"No:" << i.first << " | " << "Peso: " << i.second << endl; 
        }   

        op << "Soma dos pesos da solução: " << sumOfWeights << endl;
        op << "--------" << endl;

        avgWeights += sumOfWeights;
        sumOfWeights = 0;
    }

    avgWeights = avgWeights / 10.0;
    avgTime = avgTime / 10.0;

    op << "Média dos valores das soluções: " << avgWeights << endl;
    cout << "Média dos valores das soluções: " << avgWeights << endl;
    cout << "Soma dos pesos da melhor solução da literatura: " << instance << endl;
    cout << "--------" << endl;

    if (instance < avgWeights) {
        double qualidade = (avgWeights - instance) / (double)instance;
        op << "A solucao tem média de desempenho " << qualidade << " pior que o da literatura." << endl;
        cout << "A solucao tem média de desempenho " << qualidade << " pior que o da literatura." << endl;
    } else if (instance > avgWeights) {
        double qualidade = (instance - avgWeights) / avgWeights;
        op << "A média da solucao é melhor que a da literatura! O desempenho da literatura foi " << qualidade << " pior." << endl;
        cout << "A média da solucao e melhor que a da literatura! O desempenho da literatura foi " << (instance - sumOfWeights) / (double)sumOfWeights << " pior." << endl;
    }
    else {
        op << "A média das soluções tem desempenho igual ao da literatura." << endl;
        cout << "A média das soluções tem desempenho igual ao da literatura." << endl;
    }

    cout << "--------" << endl;

    op << "Média do tempo de execução: " << avgTime << "s." << endl;
    cout << "Média do tempo de execução: " << avgTime << "s." << endl;

    cout << "--------" << endl;
}

void printNodesGreedy(set<pair<int,int>> solucao,ofstream&op, string input_file_name, double elapsed_secs)
{   
    int instance = getLiteratureSolution(input_file_name);

    int sumOfWeights = 0;

    for(auto i : solucao)
    {
        sumOfWeights += i.second;
        op<<"No:" << i.first << " | " << "Peso: " << i.second << endl; 
    }

    op << "Soma dos pesos da solução: " << sumOfWeights << endl;

    cout << "Soma dos pesos da solução: " << sumOfWeights << endl;
    cout << "Soma dos pesos da melhor solução da literatura: " << instance << endl;
    cout << "--------" << endl;

    if (instance < sumOfWeights) {
        double qualidade = (sumOfWeights - instance) / (double)instance;
        op << "A solucao tem desempenho " << qualidade << " pior que o da literatura." << endl;
        cout << "A solucao tem desempenho " << qualidade << " pior que o da literatura." << endl;
    } else if (instance > sumOfWeights) {
        double qualidade = (instance - sumOfWeights) / (double)sumOfWeights;
        op << "A solucao é melhor que a da literatura! O desempenho da literatura foi " << qualidade << " pior." << endl;
        cout << "A solucao e melhor que a da literatura! O desempenho da literatura foi " << (instance - sumOfWeights) / (double)sumOfWeights << " pior." << endl;
    }
    else {
        op << "A solucao tem desempenho igual ao da literatura." << endl;
        cout << "A solucao tem desempenho igual ao da literatura." << endl;
    }

    cout << "--------" << endl;

    cout  << "Tempo de execução: " << elapsed_secs << "s." << endl;
    op << "Tempo de execução: " << elapsed_secs << "s." << endl;

    cout << "--------" << endl;
}

// leitura da instância
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

int menu(){

    int selecao;

    cout << "PROBLEMA DO SUBCONJUNTO DOMINANTE PONDERADO" << endl;
    cout << "----" << endl;
    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Solução por algoritmo guloso" << endl;
    cout << "[2] Solução por algoritmo guloso randomizado e adaptativo" << endl;
    cout << "[3] Solução por algoritmo guloso randomizado reativo" << endl;
    cout << "[0] Sair" << endl;
    cout << "----" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph* graph, ofstream& output_file, string input_file_name){

    switch (selecao) {

        // Guloso
        case 1:{
            cout << "-----------" << endl;
            cout << "Solução por algoritmo guloso" << endl;
            cout << "-----------" << endl;

            auto t1 = chrono::high_resolution_clock::now();
            set<pair<int,int>> solucao = graph->GreedyConstructive();
            auto t2 = chrono::high_resolution_clock::now();
            auto elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
            
            printNodesGreedy(solucao, output_file, input_file_name, elapsed_seconds);

            cout << "Solução gerada com sucesso no arquivo de saída!" << endl;
            cout << "-----------" << endl;
            cout << "Voltar para menu? (s/n) ";
            char resposta;
            cin >> resposta;
            cout << "-----------" << endl;
            if (resposta == 'n')
                exit(0);

            break;
        }

        // Guloso Randomizado Adaptativo
        case 2:{
            cout << "-----------" << endl;
            cout << "Solução por algoritmo guloso randomizado adaptativo" << endl;
            cout << "-----------" << endl;

            cout << "Insira o valor de alpha: ";
            double alpha;
            cin >> alpha;
            cout << endl << "Insira o número de iterações: ";
            int iteracoes;
            cin >> iteracoes;
            cout << "-----------" << endl;

            cout << "O algoritmo será executado 10 vezes e a média das soluções será salva no arquivo de saída." << endl;

            cout << "-----------" << endl;
            
            cout << "Alpha = " << alpha << endl;
            printNodesRandomGreedy(graph, output_file, input_file_name, alpha, iteracoes);

            cout << "Solução gerada com sucesso no arquivo de saída!" << endl;
            cout << "-----------" << endl;
            cout << "Voltar para menu? (s/n) ";
            char resposta;
            cin >> resposta;
            cout << "-----------" << endl;
            if (resposta == 'n')
                exit(0);

            break;
        }

        // Guloso Randomizado Reativo
        case 3:{
            cout << "-----------" << endl;
            cout << "Solução por algoritmo guloso randomizado reativo" << endl;
            cout << "-----------" << endl;

            cout << "Insira a quantidade de alphas: ";
            int n;
            cin >> n;

            vector<double> alphas;
            
            for (int i = 0; i < n; i++) {
                cout << "Insira o " << i+1 << "o valor de alpha: ";
                double alpha;
                cin >> alpha;
                alphas.push_back(alpha);
            }
            
            cout << "Insira o número de iterações: ";
            int iteracoes;
            cin >> iteracoes;

            cout << endl << "Insira o tamanho do bloco: ";
            int bloco;
            cin >> bloco;
            cout << "-----------" << endl;

            cout << "O algoritmo será executado 10 vezes e a média das soluções será salva no arquivo de saída." << endl;

            cout << "-----------" << endl;
            
            printNodesRandomReactiveGreedy(graph, output_file, input_file_name, alphas, iteracoes, bloco);

            cout << "Solução gerada com sucesso no arquivo de saída!" << endl;
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

int mainMenu(string input_file_name, ofstream& output_file, Graph* graph){

    int selecao = 1;

    while(selecao != 0){
        system("clear");
        selecao = menu();

        if(output_file.is_open())
            selecionar(selecao, graph, output_file, input_file_name);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;
    }

    return 0;
}


int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 3) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> " << endl;
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
        graph = readAdjacencyMatrix(input_file, 0, 0, 1);
    else
        cout << "Unable to open " << argv[1];

    mainMenu(input_file_name, output_file, graph);
    
    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}
