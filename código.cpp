#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <queue>
#include<stack>

enum class TipoDeGrafo {
    Matriz,
    Lista
};

using namespace std;

struct Node {
    int valor;
    Node* proximo;

    Node(int v) : valor(v), proximo(nullptr) {}
};

// Função para verificar e remover repetições
void removeRepeticoes(string nomeArquivoEntrada, string nomeArquivoSaida) {
    ifstream arquivoEntrada(nomeArquivoEntrada);
    if (!arquivoEntrada.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return;
    }

    ofstream arquivoSaida(nomeArquivoSaida);
    if (!arquivoSaida.is_open()) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return;
    }

    set<pair<int, int>> conjuntoArestas;  // Conjunto para armazenar as arestas únicas
    vector<pair<int, int>> repeticoes;  // Vetor para armazenar as repetições

    int numVertices;
    arquivoEntrada >> numVertices;
    arquivoSaida << numVertices << endl;

    int vertice1, vertice2;
    while (arquivoEntrada >> vertice1 >> vertice2) {
        if (vertice1 == vertice2) {
            repeticoes.push_back({vertice1, vertice2});  // Laço (vértice para si mesmo)
        } else if (conjuntoArestas.count({vertice2, vertice1}) || conjuntoArestas.count({vertice1, vertice2})) {
            repeticoes.push_back({vertice1, vertice2});  // Aresta duplicada ou na ordem inversa
        } else {
            conjuntoArestas.insert({vertice1, vertice2});
            conjuntoArestas.insert({vertice2, vertice1});
            arquivoSaida << vertice1 << " " << vertice2 << endl;
        }
    }

    arquivoEntrada.close();
    arquivoSaida.close();

    // Exibir repetições encontradas
    if (!repeticoes.empty()) {
        // cout << "Repetiçoes encontradas e removidas:" << endl;
        for (const auto& repeticao : repeticoes) {
            // cout << "Repetiçao: " << repeticao.first << " " << repeticao.second << endl;
        }
        cout << "Repeticoes encontradas e removidas." << endl;
    } else {
        cout << "Nenhuma repetiçao encontrada." << endl;
    }
}

class Graph {
public:
    // Atributos
    int numeroDeVertices;
    vector<vector<int>> matrizAdj;
    vector<Node*> listaAdj;
    TipoDeGrafo tipoDeGrafo;

    // Construtor
    Graph(string filename, TipoDeGrafo graphType) {
        tipoDeGrafo = graphType;
        // Abra o arquivo para leitura
        ifstream arquivo(filename);

        // Verifique se o arquivo foi aberto com sucesso
        if (!arquivo.is_open()) {
            cerr << "Erro ao abrir o arquivo." << endl;
        }

        // Variável para armazenar cada linha do arquivo
        string linha;
        // Ler a primeira linha do arquivo e salvar o número de vértices
        getline(arquivo, linha);
        numeroDeVertices = stoi(linha);

        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            // Inicializa a matriz de adjacência com 0
            matrizAdj = vector<vector<int>>(numeroDeVertices, vector<int>(numeroDeVertices, 0));

            // Ler e preencher a matriz de adjacência
            while (getline(arquivo, linha)) {
                istringstream iss(linha);
            
                int numero1, numero2;
                // Tente extrair os dois números da linha
                if (iss >> numero1 >> numero2) {
                    // Faça o que quiser com os números
                    matrizAdj[numero1-1][numero2-1] = 1;
                    matrizAdj[numero2-1][numero1-1] = 1;

                }
            }

            arquivo.close();
        }
        
        else if (tipoDeGrafo == TipoDeGrafo::Lista){
            listaAdj = vector<Node*>(numeroDeVertices, nullptr);

            // Ler e construir a lista de adjacência
            while (getline(arquivo, linha)) {
                istringstream iss(linha);

                int numero1, numero2;
                // Tente extrair os dois números da linha
                if (iss >> numero1 >> numero2) {
                    // Adiciona a aresta aos vértices correspondentes na lista de adjacência
                    // Crie um novo nó para o vértice de destino e insira-o no início da lista
                    Node* novoNo1 = new Node(numero2-1);
                    novoNo1->proximo = listaAdj[numero1-1];
                    listaAdj[numero1-1] = novoNo1;

                    Node* novoNo2 = new Node(numero1-1);
                    novoNo2->proximo = listaAdj[numero2-1];
                    listaAdj[numero2-1] = novoNo2;

                }
            }

            arquivo.close();
        }
    }

    int numVertices() {
        return numeroDeVertices;
    }

    int numArestas() {
        int numArestas = 0;
        if (tipoDeGrafo == TipoDeGrafo::Matriz) {
            // Verifique a matriz de adjacencia para contar as arestas
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
            // Para uma lista de adjacencia
            for (int i = 0; i < numeroDeVertices; i++) {
                Node* currentNode = listaAdj[i];
                while (currentNode != nullptr) {
                    numArestas++;
                    currentNode = currentNode->proximo;
                }
            }
            // Como sao grafos nao direcionados, cada aresta e contada duas vezes.
            return numArestas/2;
        }
    }

    int grauMinimo() {
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
        int grauMid = 2*numArestas()/numVertices(); 
        return grauMid;
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
            // Se o número de vértices for ímpar, a mediana é o valor central
            return graus[tamanho / 2];
        } else {
            // Se o número de vértices for par, a mediana é a média dos dois valores centrais
            int meio1 = graus[(tamanho - 1) / 2];
            int meio2 = graus[tamanho / 2];
            return (meio1 + meio2) / 2;
        }
    }

    void BFS(int verticeInicial) {
        // Inicializa o vetor de marcação, vetor de pais e vetor de níveis
        vector<int> vetorDeMarcacao(numeroDeVertices, 0);
        vector<int> vetorDePais(numeroDeVertices, 0);
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
        vector<int> vetorDePais(numeroDeVertices, 0);
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
                // Percorre os vértices adjacentes ao vértice retirado da fila
                for(int i = 0; i < numeroDeVertices; i++){
                    if (matrizAdj[v][i] == 1 && vetorDeMarcacao[i] == 0){
                        // Marca o vértice como visitado e o adiciona à fila
                        vetorDeMarcacao[i] = 1;
                        pilha.push(i);
                        vetorDePais[i] = v;
                        vetorDeNiveis[i] = vetorDeNiveis[v]+1;
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
                Node* currentNode = listaAdj[v];
                while (currentNode != nullptr) {
                    if (vetorDeMarcacao[currentNode->valor] == 0) {
                        // Marca o vértice como visitado e o adiciona à fila
                        vetorDeMarcacao[currentNode->valor] = 1;
                        pilha.push(currentNode->valor);
                        vetorDePais[currentNode->valor] = v;
                        vetorDeNiveis[currentNode->valor] = vetorDeNiveis[v]+1;
                    }
                    currentNode = currentNode->proximo;  
                }
            }
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
            cout << verticeInicial << " " << diametro << endl;
        }
    
    return diametro;
    }

    void compConexas(){
        vector<vector<int>> componentes;
        vector<int> componenteAtual;
        vector<int> vetorDeMarcacao(numeroDeVertices, 0);
        queue<int> fila;

        // 
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

        // Sort components by size in descending order
        sort(componentes.begin(), componentes.end(), [](const vector<int>& a, const vector<int>& b) {return a.size() > b.size();});
        
        // Após a execução do BFS, imprime os resultados em um arquivo de texto
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

int main(){
    //removeRepeticoes("grafo_2.txt", "grafo_sem_repeticao.txt");
    // Graph grafoLista("grafo_sem_repeticao.txt", TipoDeGrafo::Lista);
    Graph grafoMatriz("grafo_sem_repeticao.txt", TipoDeGrafo::Matriz);

    grafoMatriz.saida();
    //grafoLista.saida();
}
