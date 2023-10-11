#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <queue>
#include <stack>
#include <typeinfo>
#include <ctime>

enum class TipoDeGrafo {
    Matriz,
    Lista
};

using namespace std;

// Classe para representar um nó de uma lista de adjacência
struct Node {
    int valor;
    Node* proximo;

    Node(int v) : valor(v), proximo(nullptr) {}
};

// Função para inserir um vértice em ordem crescente em uma lista de adjacência
void orderedInsertion(Node*& lista, int vertice) {
    Node* novoNo = new Node(vertice);
    if (!lista || lista->valor >= vertice) {
        novoNo->proximo = lista;
        lista = novoNo;
    } else {
        Node* atual = lista;
        while (atual->proximo && atual->proximo->valor < vertice) {
            atual = atual->proximo;
        }
        novoNo->proximo = atual->proximo;
        atual->proximo = novoNo;
    }
}

// Função para verificar se a aresta já existe na lista de adjacência
bool existeArestaLista(Node* lista, int vertice2) {
    Node* atual = lista;
    while (atual != nullptr) {
        if (atual->valor == vertice2) {
            return true;
        }
        atual = atual->proximo;
    }
    return false;
}

class Graph {
public:
    // Atributos
    int numeroDeVertices;
    vector<vector<bool>> matrizAdj;
    vector<Node*> listaAdj;
    TipoDeGrafo tipoDeGrafo;

    // Construtor
    Graph(string filename, TipoDeGrafo graphType) {
        tipoDeGrafo = graphType;

        ifstream arquivo(filename);
        // Variável para armazenar cada linha do arquivo
        string linha;
        // Lê o numero de vertices na primeira linha do arquivo
        getline(arquivo, linha);
        numeroDeVertices = stoi(linha);

        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            
            // Inicializa a matriz de adjacência como uma matriz de bool
            matrizAdj = vector<vector<bool>>(numeroDeVertices, vector<bool>(numeroDeVertices, false));

            // Le o arquivo e constrói a matriz de adjacência
            while (getline(arquivo, linha)) {
                istringstream iss(linha);
            
                int numero1, numero2;
                // Extrai os dois números da linha
                if (iss >> numero1 >> numero2) {
                    // Verifica se a aresta não é de um vértice para si mesmo
                    if (numero1 != numero2) {
                        // Verifica se a aresta não foi adicionada anteriormente
                        if (!matrizAdj[numero1-1][numero2-1]) {
                            matrizAdj[numero1 - 1][numero2 - 1] = true;
                            matrizAdj[numero2 - 1][numero1 - 1] = true;
                        }
                    }
                }
            }
            arquivo.close();
        }
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            listaAdj = vector<Node*>(numeroDeVertices, nullptr);

            // Le o arquivo e constrói a lista de adjacência
            while (getline(arquivo, linha)) {
                istringstream iss(linha);

                int numero1, numero2;
                if (iss >> numero1 >> numero2) {
                    // Verifica se a aresta não é de um vértice para si mesmo
                    if (numero1 != numero2) {
                        // Verifica se a aresta não foi adicionada anteriormente
                        if (!existeArestaLista(listaAdj[numero1-1], numero2-1)) {
                            // Insere os vértices em ordem crescente na lista de adjacência
                            orderedInsertion(listaAdj[numero1 - 1], numero2 - 1);
                            orderedInsertion(listaAdj[numero2 - 1], numero1 - 1);
                        }
                    }    
                }
            }
            arquivo.close();
        }
        cout << "Grafo criado com sucesso." << endl;
    }

    int numVertices() {
        return numeroDeVertices;
    }

    int numArestas() {
        int numArestas = 0;
        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            // Itera sobre o triangulo superior da matriz de adjacencia
            for (int i = 0; i < numeroDeVertices; i++) {
                for (int j = i+1; j < numeroDeVertices; j++) {
                    if (matrizAdj[i][j] == 1){
                        numArestas += 1;
                    }
                }
            }
            return numArestas;
        }
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            // Itera sobre a as listas encadeadas da lista de adjacencia
            for (int i = 0; i < numeroDeVertices; i++) {
                Node* currentNode = listaAdj[i];
                while (currentNode != nullptr) {
                    numArestas++;
                    currentNode = currentNode->proximo;
                }
            }
            // É preciso dividir por dois pois cada aresta foi contada duas vezes
            return numArestas/2;
        }
        cout << "Flag4" << endl;
    }

    int grauMinimo() {
        //
        int grauMin = numeroDeVertices;
        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            for (int i = 0; i < numeroDeVertices; i++) {
                int grau = 0;
                for (int j = 0; j < numeroDeVertices; j++) {
                    if (matrizAdj[i][j] == 1) {
                        grau++;
                    }
                }
                if (grau < grauMin) {
                    grauMin = grau;
                }
            }
            return grauMin;
        }
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            for (int i = 0; i < numeroDeVertices; i++) {
                int grau = 0;
                Node* currentNode = listaAdj[i];
                while (currentNode != nullptr) {
                    grau++;
                    currentNode = currentNode->proximo;
                }
                if (grau < grauMin) {
                    grauMin = grau;
                }
            }
            return grauMin;
        }
    }

    int grauMaximo() {
        int grauMax = 0;
        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            for (int i = 0; i < numeroDeVertices; i++) {
                int grau = 0;
                for (int j = 0; j < numeroDeVertices; j++) {
                    if (matrizAdj[i][j] == 1) {
                        grau++;
                    }
                }
                if (grau > grauMax) {
                    grauMax = grau;
                }
            }
            return grauMax;
        }
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            for (int i = 0; i < numeroDeVertices; i++) {
                int grau = 0;
                Node* currentNode = listaAdj[i];
                while (currentNode != nullptr) {
                    grau++;
                    currentNode = currentNode->proximo;
                }
                if (grau > grauMax) {
                    grauMax = grau;
                }
            }
            return grauMax;
        }
    }

    int grauMedio() {
        // Retorna o dobro do número de arestas dividido pelo número de vértices
        return 2*numArestas()/numVertices();
    }

    int medianaDeGrau() {
        vector<int> graus; // Vetor para armazenar os graus dos vértices

        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            // Calcula os graus no caso de matriz de adjacência
            for (int i = 0; i < numeroDeVertices; i++) {
                int grau = 0;
                for (int j = 0; j < numeroDeVertices; j++) {
                    if (matrizAdj[i][j] == 1) {
                        grau++;
                    }
                }
                graus.push_back(grau);
            }
        }
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            // Calcula os graus no caso de lista de adjacência
            for (int i = 0; i < numeroDeVertices; i++) {
                int grau = 0;
                Node* currentNode = listaAdj[i];
                while (currentNode != nullptr) {
                    grau++;
                    currentNode = currentNode->proximo;
                }
                graus.push_back(grau);
            }
        }

        // Ordena os graus em ordem crescente
        sort(graus.begin(), graus.end());

        int tamanho = graus.size();
        if (tamanho % 2 == 1) {
            // Se o número de vértices for ímpar, pegamos a mediana como o valor central
            return graus[tamanho / 2];
        } else {
            // Se o número de vértices for par, pegamos a mediana como a média dos dois valores centrais
            int meio1 = graus[(tamanho - 1) / 2];
            int meio2 = graus[tamanho / 2];
            return (meio1 + meio2) / 2;
        }
    }

    void BFS(int verticeInicial) {
        // Inicializa o vetor de marcação, vetor de pais e vetor de níveis
        vector<int> vetorDeMarcacao(numeroDeVertices, 0);
        vector<int> vetorDePais(numeroDeVertices, -1);
        vector<int> vetorDeNiveis(numeroDeVertices, 0);
        queue<int> fila;

        vetorDeNiveis[verticeInicial-1] = 0;
        vetorDePais[verticeInicial-1] = -2;
        vetorDeMarcacao[verticeInicial-1] = 1;
        fila.push(verticeInicial-1);

        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            // Enquanto a fila não estiver vazia
            while(!fila.empty()){
                // Retira o primeiro elemento da fila
                int v = fila.front();
                fila.pop();
                // Percorre os vértices adjacentes ao vértice retirado da fila
                for(int i = 0; i < numeroDeVertices; i++){
                    if (matrizAdj[v][i] == 1 && vetorDeMarcacao[i] == 0){
                        // Marca o vértice como visitado e o adiciona à fila
                        vetorDeMarcacao[i] = 1;
                        fila.push(i);
                        vetorDePais[i] = v;
                        vetorDeNiveis[i] = vetorDeNiveis[v]+1;
                    }
                }
            }
            // Após a execução do BFS, imprime os resultados em um arquivo de texto
            ofstream arquivoResultado("resultado_bfs_matriz.txt");
            for (int i = 0; i < numeroDeVertices; i++) {
                if (vetorDeMarcacao[i] == 1) {
                    arquivoResultado << i + 1 << " " << vetorDeNiveis[i] << " " << vetorDePais[i] + 1 << endl;
                }
            }
            arquivoResultado.close();
        }
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            while(!fila.empty()){
                // Retira o primeiro elemento da fila
                int v = fila.front();
                fila.pop();
                Node* currentNode = listaAdj[v];
                while (currentNode != nullptr) {
                    if (vetorDeMarcacao[currentNode->valor] == 0) {
                        // Marca o vértice como visitado e o adiciona à fila
                        vetorDeMarcacao[currentNode->valor] = 1;
                        fila.push(currentNode->valor);
                        vetorDePais[currentNode->valor] = v;
                        vetorDeNiveis[currentNode->valor] = vetorDeNiveis[v]+1;
                    }
                    currentNode = currentNode->proximo;  
                }
            }
            cout << vetorDePais[10-1]+1 << " " << vetorDePais[20-1]+1 << " " << vetorDePais[30-1]+1 << endl;
            // Após a execução do BFS, imprime os resultados em um arquivo de texto
            ofstream arquivoResultado("resultado_bfs_lista.txt");
            for (int i = 0; i < numeroDeVertices; i++) {
                if (vetorDeMarcacao[i] == 1) {
                    arquivoResultado << i + 1 << " " << vetorDeNiveis[i] << " " << vetorDePais[i] + 1 << endl;
                }
            }
            arquivoResultado.close();
        }
    }

    void DFS(int verticeInicial) {
        // Inicializa o vetor de marcação, vetor de pais e vetor de níveis
        vector<int> vetorDeMarcacao(numeroDeVertices, 0);
        vector<int> vetorDePais(numeroDeVertices, -1);
        vector<int> vetorDeNiveis(numeroDeVertices, 0);
        stack<int> pilha;

        vetorDeNiveis[verticeInicial-1] = 0;
        vetorDePais[verticeInicial-1] = -2;
        vetorDeMarcacao[verticeInicial-1] = 1;
        pilha.push(verticeInicial-1);

        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            // Enquanto a pilha não estiver vazia
            while(!pilha.empty()){
                // Retira o elemento do topo da pilha
                int v = pilha.top();
                pilha.pop();
                if (vetorDeMarcacao[v] == 0){
                    vetorDeMarcacao[v] = 1;
                    for(int i = 0; i < numeroDeVertices; i++){
                        if (matrizAdj[v][i] == 1) {
                            pilha.push(i);
                            vetorDePais[i] = v;
                            vetorDeNiveis[i] = vetorDeNiveis[v]+1;
                        }
                    }
                }
            }
            // Após a execução do BFS, imprime os resultados em um arquivo de texto
            ofstream arquivoResultado("resultado_dfs_matriz.txt");
            for (int i = 0; i < numeroDeVertices; i++) {
                if (vetorDeMarcacao[i] == 1) {
                    arquivoResultado << i + 1 << " " << vetorDeNiveis[i] << " " << vetorDePais[i] + 1 << endl;
                }
            }
            arquivoResultado.close();
        }
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            while(!pilha.empty()){
                // Retira o primeiro elemento da fila
                int v = pilha.top();
                pilha.pop();
                if (vetorDeMarcacao[v] == 0){
                    vetorDeMarcacao[v] = 1;
                    Node* currentNode = listaAdj[v];
                    while (currentNode != nullptr) {
                        pilha.push(currentNode->valor);
                        vetorDePais[currentNode->valor] = v;
                        vetorDeNiveis[currentNode->valor] = vetorDeNiveis[v]+1;
                        currentNode = currentNode->proximo;  
                    }
                }
            }
            cout << vetorDePais[10-1]+1 << " " << vetorDePais[20-1]+1 << " " << vetorDePais[30-1]+1 << endl;
            // Após a execução do BFS, imprime os resultados em um arquivo de texto
            ofstream arquivoResultado("resultado_dfs_lista.txt");
            for (int i = 0; i < numeroDeVertices; i++) {
                if (vetorDeMarcacao[i] == 1) {
                    arquivoResultado << i + 1 << " " << vetorDeNiveis[i] << " " << vetorDePais[i] + 1 << endl;
                }
            }
            arquivoResultado.close();
        }
    }

    int distancia(int verticeInicial, int verticeFinal){
        // Inicializa o vetor de marcação, vetor de pais e vetor de níveis
        vector<int> vetorDeMarcacao(numeroDeVertices, 0);
        vector<int> vetorDeNiveis(numeroDeVertices, 0);
        queue<int> fila;

        vetorDeNiveis[verticeInicial-1] = 0;
        vetorDeMarcacao[verticeInicial-1] = 1;
        fila.push(verticeInicial-1);

        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            // Enquanto a fila não estiver vazia
            while(!fila.empty()){
                // Retira o primeiro elemento da fila
                int v = fila.front();
                if (v == verticeFinal-1){
                    return vetorDeNiveis[v];
                }
                fila.pop();
                // Percorre os vértices adjacentes ao vértice retirado da fila
                for(int i = 0; i < numeroDeVertices; i++){
                    if (matrizAdj[v][i] == 1 && vetorDeMarcacao[i] == 0){
                        // Marca o vértice como visitado e o adiciona à fila
                        vetorDeMarcacao[i] = 1;
                        fila.push(i);
                        vetorDeNiveis[i] = vetorDeNiveis[v]+1;
                    }
                }
            }
        }
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            while(!fila.empty()){
                // Retira o primeiro elemento da fila
                int v = fila.front();
                if (v == verticeFinal-1){
                    return vetorDeNiveis[v];
                }
                fila.pop();
                Node* currentNode = listaAdj[v];
                while (currentNode != nullptr) {
                    if (vetorDeMarcacao[currentNode->valor] == 0) {
                        // Marca o vértice como visitado e o adiciona à fila
                        vetorDeMarcacao[currentNode->valor] = 1;
                        fila.push(currentNode->valor);
                        vetorDeNiveis[currentNode->valor] = vetorDeNiveis[v]+1;
                    }
                    currentNode = currentNode->proximo;  
                }
            }
        }
        return -1;
    }

    int diametro() {
        int diametro = 0;
        for (int verticeInicial = 1; verticeInicial <= numeroDeVertices; verticeInicial++) {
            // Inicializa o vetor de marcação, vetor de pais e vetor de níveis
            vector<int> vetorDeMarcacao(numeroDeVertices, 0);
            vector<int> vetorDeNiveis(numeroDeVertices, 0);
            queue<int> fila;

            vetorDeNiveis[verticeInicial-1] = 0;
            vetorDeMarcacao[verticeInicial-1] = 1;
            fila.push(verticeInicial-1);

            if (tipoDeGrafo == TipoDeGrafo::Matriz) {
                // Enquanto a fila não estiver vazia
                while(!fila.empty()){
                    // Retira o primeiro elemento da fila
                    int v = fila.front();
                    fila.pop();
                    // Percorre os vértices adjacentes ao vértice retirado da fila
                    for(int i = 0; i < numeroDeVertices; i++){
                        if (matrizAdj[v][i] == 1 && vetorDeMarcacao[i] == 0){
                            // Marca o vértice como visitado e o adiciona à fila
                            vetorDeMarcacao[i] = 1;
                            fila.push(i);
                            vetorDeNiveis[i] = vetorDeNiveis[v]+1;
                        }
                    }
                }
            }
            else if (tipoDeGrafo == TipoDeGrafo::Lista){
                while(!fila.empty()){
                    // Retira o primeiro elemento da fila
                    int v = fila.front();
                    fila.pop();
                    Node* currentNode = listaAdj[v];
                    while (currentNode != nullptr) {
                        if (vetorDeMarcacao[currentNode->valor] == 0) {
                            // Marca o vértice como visitado e o adiciona à fila
                            vetorDeMarcacao[currentNode->valor] = 1;
                            fila.push(currentNode->valor);
                            vetorDeNiveis[currentNode->valor] = vetorDeNiveis[v]+1;
                        }
                        currentNode = currentNode->proximo;  
                    }
                }
            }
            // Encontrar o maior valor em vetorDeNiveis
            for (int i = 0; i < numeroDeVertices; i++) {
                diametro = max(diametro, vetorDeNiveis[i]);
            }
            // cout << verticeInicial << " " << diametro << endl;
        }
    
    return diametro;
    }

    int diametroAprox(int numTestes){
        int diametro = 0;
        for (int i = 0; i < numTestes; i++) {
            int verticeAleatorio = rand() % numeroDeVertices + 1;
            // Inicializa o vetor de marcação, vetor de pais e vetor de níveis
            vector<int> vetorDeMarcacao(numeroDeVertices, 0);
            vector<int> vetorDeNiveis(numeroDeVertices, 0);
            queue<int> fila;

            vetorDeNiveis[verticeAleatorio-1] = 0;
            vetorDeMarcacao[verticeAleatorio-1] = 1;
            fila.push(verticeAleatorio-1);

            if (tipoDeGrafo == TipoDeGrafo::Matriz) {
                // Enquanto a fila não estiver vazia
                while(!fila.empty()){
                    // Retira o primeiro elemento da fila
                    int v = fila.front();
                    fila.pop();
                    // Percorre os vértices adjacentes ao vértice retirado da fila
                    for(int i = 0; i < numeroDeVertices; i++){
                        if (matrizAdj[v][i] == 1 && vetorDeMarcacao[i] == 0){
                            // Marca o vértice como visitado e o adiciona à fila
                            vetorDeMarcacao[i] = 1;
                            fila.push(i);
                            vetorDeNiveis[i] = vetorDeNiveis[v]+1;
                        }
                    }
                }
            }
            else if (tipoDeGrafo == TipoDeGrafo::Lista){
                while(!fila.empty()){
                    // Retira o primeiro elemento da fila
                    int v = fila.front();
                    fila.pop();
                    Node* currentNode = listaAdj[v];
                    while (currentNode != nullptr) {
                        if (vetorDeMarcacao[currentNode->valor] == 0) {
                            // Marca o vértice como visitado e o adiciona à fila
                            vetorDeMarcacao[currentNode->valor] = 1;
                            fila.push(currentNode->valor);
                            vetorDeNiveis[currentNode->valor] = vetorDeNiveis[v]+1;
                        }
                        currentNode = currentNode->proximo;  
                    }
                }
            }
            // Encontrar o maior valor em vetorDeNiveis
            for (int i = 0; i < numeroDeVertices; i++) {
                diametro = max(diametro, vetorDeNiveis[i]);
            }
            // cout << verticeInicial << " " << diametro << endl;
        }
    
    return diametro;
    }

    void compConexas(){
        vector<vector<int>> componentes;
        vector<int> componenteAtual;
        vector<int> vetorDeMarcacao(numeroDeVertices, 0);
        queue<int> fila;
        
        int verticeInicial = -1;
        for (int i = 0; i < numeroDeVertices; i++){
            if (vetorDeMarcacao[i] == 0){
                verticeInicial = i;
                break;
            }
        }

        while (verticeInicial != -1){
            vetorDeMarcacao[verticeInicial] = 1;
            fila.push(verticeInicial);
            componenteAtual.push_back(verticeInicial+1);
            if (tipoDeGrafo == TipoDeGrafo::Matriz) {
                // Enquanto a fila não estiver vazia
                while(!fila.empty()){
                    // Retira o primeiro elemento da fila
                    int v = fila.front();
                    fila.pop();
                    // Percorre os vértices adjacentes ao vértice retirado da fila
                    for(int i = 0; i < numeroDeVertices; i++){
                        if (matrizAdj[v][i] == 1 && vetorDeMarcacao[i] == 0){
                            // Marca o vértice como visitado e o adiciona à fila
                            vetorDeMarcacao[i] = 1;
                            fila.push(i);
                            componenteAtual.push_back(i+1);
                        }
                    }
                }
                componentes.push_back(componenteAtual);
                componenteAtual.clear();
            }
            if (tipoDeGrafo == TipoDeGrafo::Lista){
                while(!fila.empty()){
                    // Retira o primeiro elemento da fila
                    int v = fila.front();
                    fila.pop();
                    Node* currentNode = listaAdj[v];
                    while (currentNode != nullptr) {
                        if (vetorDeMarcacao[currentNode->valor] == 0) {
                            // Marca o vértice como visitado e o adiciona à fila
                            vetorDeMarcacao[currentNode->valor] = 1;
                            fila.push(currentNode->valor);
                            componenteAtual.push_back(currentNode->valor+1);
                        }
                        currentNode = currentNode->proximo;  
                    }
                }
                componentes.push_back(componenteAtual);
                componenteAtual.clear();
            }

            verticeInicial = -1;
            for (int i = 0; i < numeroDeVertices; i++){
                if (vetorDeMarcacao[i] == 0){
                    verticeInicial = i;
                    break;
                }
            }
        }

        sort(componentes.begin(), componentes.end(), [](const vector<int>& a, const vector<int>& b) {return a.size() > b.size();});
        
        ofstream arquivoResultado("Arquivo_resultado.txt", ios::app);
        arquivoResultado << "Numero de componentes: " << componentes.size() << endl;
        
        for (int i = 0; i < componentes.size(); i++) {
            arquivoResultado << "Tamanho das componentes: " << componentes[i].size() << " ->";
            for (int j = 0; j < componentes[i].size(); j++){
                arquivoResultado << " " << componentes[i][j];
            }
            arquivoResultado << endl << endl;
        }
        arquivoResultado.close();
    }

    void saida(){
        ofstream arquivoResultado("Arquivo_resultado.txt");
        arquivoResultado << "Numero de vertices: " << numeroDeVertices << endl << endl;
        arquivoResultado << "Numero de arestas: " << numArestas() << endl << endl;
        arquivoResultado << "Grau minimo: " << grauMinimo() << endl << endl;
        arquivoResultado << "Grau maximo: " << grauMaximo() << endl << endl;
        arquivoResultado << "Grau medio: " << grauMedio() << endl << endl;
        arquivoResultado << "Mediana de grau: " << medianaDeGrau() << endl << endl; 
        
        compConexas();

    }
};

using namespace std;

int main() {
    const int numGraphs = 1; // Número total de arquivos de grafo
    int limiteIteracoes = 600; // Limite de iterações para o diâmetro aproximado

    ofstream arquivoResultados("diametrosAprox.txt", ios::app);

    for (int graphIndex = 1; graphIndex <= numGraphs; graphIndex++) {
        string nomeArquivo = "grafo_6.txt";
        Graph grafoLista(nomeArquivo, TipoDeGrafo::Lista);

        // Calcule o diâmetro aproximado do grafo com o limite de iterações
        int diametroAproximado = grafoLista.diametroAprox(limiteIteracoes);

        // Calcule o diâmetro exato do grafo
        // int diametroExato = grafoLista.diametro();

        // Escreva os resultados no arquivo à medida que são calculados
        arquivoResultados << "Grafo " << "6" << ":" << endl;
        arquivoResultados << "Diâmetro Aproximado (com limite de " << limiteIteracoes << " iterações): " << diametroAproximado << endl;
        // arquivoResultados << "Diâmetro Exato: " << diametroExato << endl;
        arquivoResultados << "----------------------------------------" << endl;
    }

    arquivoResultados.close();

    return 0;
}


// Calculo do tempo medio de execucao do BFS e DFS para cada grafo
/*
int main() {
    const int numGraphs = 6;
    const int numTests = 100;

    ofstream arquivoBFS("resultado_bfs.txt");
    ofstream arquivoDFS("resultado_dfs.txt");

    for (int graphIndex = 1; graphIndex <= numGraphs; graphIndex++) {
        string nomeArquivo = "grafo_" + to_string(graphIndex) + ".txt";
        Graph grafoLista(nomeArquivo, TipoDeGrafo::Lista);

        double elapsed_time = 0;
        srand(static_cast<unsigned int>(time(nullptr)));

        for (int i = 0; i < numTests; i++) {
            int numVertices = grafoLista.numVertices();
            int verticeAleatorio = rand() % numVertices + 1;

            clock_t start_time = clock();
            grafoLista.BFS(verticeAleatorio);
            clock_t end_time = clock();
            elapsed_time += static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
        }

        arquivoBFS << "Tempo médio para BFS " << graphIndex << ": " << elapsed_time / numTests << " segundos" << endl;

        elapsed_time = 0;

        for (int i = 0; i < numTests; i++) {
            int numVertices = grafoLista.numVertices();
            int verticeAleatorio = rand() % numVertices + 1;

            clock_t start_time = clock();
            grafoLista.DFS(verticeAleatorio);
            clock_t end_time = clock();
            elapsed_time += static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
        }

        arquivoDFS << "Tempo médio para DFS " << graphIndex << ": " << elapsed_time / numTests << " segundos" << endl;
    }

    arquivoBFS.close();
    arquivoDFS.close();

    return 0;
}
*/