def check_duplicate_self_reverse_edges(filename):
    edge_set = set()  # Usamos um conjunto para armazenar as arestas únicas
    duplicates = []
    self_edges = []
    reverse_edges = []

    with open(filename, 'r') as file:
        # Lê o número de vértices na primeira linha
        num_vertices = int(file.readline().strip())

        for line in file:
            vertices = tuple(map(int, line.split()))
            
            if len(vertices) != 2:
                print(f"Aresta inválida: {vertices}. Ignorada.")
                continue
            
            if vertices[0] == vertices[1]:
                self_edges.append(vertices)
            elif vertices in edge_set:
                duplicates.append(vertices)
            elif (vertices[1], vertices[0]) in edge_set:
                reverse_edges.append(vertices)
            else:
                edge_set.add(vertices)

    return duplicates, self_edges, reverse_edges

filename = "grafo_1.txt"  # Substitua com o nome do seu arquivo
duplicates, self_edges, reverse_edges = check_duplicate_self_reverse_edges(filename)

total_anomalies = len(duplicates) + len(self_edges) + len(reverse_edges)

if total_anomalies > 0:
    print(f"Total de anomalias encontradas no grafo: {total_anomalies}")
    
    if duplicates:
        print("Arestas duplicadas encontradas:")
        for edge in duplicates:
            print(f"Aresta {edge[0]}-{edge[1]} é repetida.")

    if self_edges:
        print("Laços encontrados:")
        for edge in self_edges:
            print(f"Laço no vértice {edge[0]}.")

    if reverse_edges:
        print("Arestas na ordem inversa encontradas:")
        for edge in reverse_edges:
            print(f"Aresta na ordem inversa: {edge[0]}-{edge[1]}.")

else:
    print("Não foram encontradas anomalias no grafo.")

