import random

# FUNCIONES DE GENERACIÓN DE GRAFOS DE DISTINTOS TIPOS
def random_connected_graph(n, m):
    if m < n - 1:
        raise ValueError("The graph must have at least n-1 edges to be connected.")

    tree_edges = [(i, i + 1) for i in range(1, n)]
    remaining_edges = m - (n - 1)

    available_vertices = set(range(1, n + 1))
    added_edges = set(tree_edges)

    while remaining_edges > 0:
        u = random.choice(list(available_vertices))
        v = random.choice(list(available_vertices - {u}))

        if (u, v) not in added_edges and (v, u) not in added_edges:
            added_edges.add((u, v))
            remaining_edges -= 1

    return list(added_edges)


def random_connected_or_not_graph(n, m):
    if m < 0 or m > n * (n - 1) // 2:
        raise ValueError("Invalid number of edges for the given number of vertices.")

    added_edges = set()

    while len(added_edges) < m:
        u = random.randint(1, n)
        v = random.randint(1, n)
        if u != v and (u, v) not in added_edges and (v, u) not in added_edges:
            added_edges.add((u, v))
    return list(added_edges)


def cycle_graph_edges(n):
    if n < 3:
        raise ValueError("Number of vertices must be at least 3 for a cycle graph.")

    graph_edges = [(i, i + 1) for i in range(1, n)] + [(n, 1)]
    return graph_edges


def star_graph_edges(n):
    if n < 3:
        raise ValueError("Number of vertices must be at least 3 for a star graph.")

    central_vertex = 1
    graph_edges = [(central_vertex, i) for i in range(2, n + 1)]
    return graph_edges



# FUNCIONES AUXILIARES PARA LA CREACIÓN DE LOS GRAFOS
def remove_duplicate_edges(edges):
    unique_edges = set()
    result = []

    for edge in edges:
        reverse_edge = (edge[1], edge[0])

        if edge not in unique_edges and reverse_edge not in unique_edges:
            result.append(edge)
            unique_edges.add(edge)

    return result


def sort_tuples(lst):
    sorted_list = [tuple(sorted(t)) for t in lst]
    return sorted(sorted_list)


def get_unique_vertices(graph):
    vertices = set()
    for edge in graph:
        vertices.update(edge)
    return list(vertices)


def count_vertex_occurrences(graph):
    vertex_counts = {}

    for pair in graph:
        for vertex in pair:
            if vertex in vertex_counts:
                vertex_counts[vertex] += 1
            else:
                vertex_counts[vertex] = 1

    max_vertex = max(max(pair) for pair in graph)
    occurrences = [vertex_counts.get(i, 0) for i in range(1, max_vertex + 1)]

    return occurrences



# FUNCIONES PARA LA GESTIÓN DE PROBABILIDADES DE LAS ARISTAS DEL GRAFO
def calculate_vertex_probabilities(vertices, occurrences, graph):
    vertex_probabilities = {}

    for v, occurrence in zip(vertices, occurrences):
        edges_with_vertex = [edge for edge in graph if v in edge]
        total_edges = sum(occurrences[edge[0] - 1] for edge in edges_with_vertex)

        probabilities = [round(random.uniform(0, 1), 2) for _ in edges_with_vertex]
        total_prob = sum(probabilities)
        normalized_probabilities = [prob / total_prob for prob in probabilities]

        vertex_probabilities[v] = list(zip(edges_with_vertex, normalized_probabilities))
    return vertex_probabilities


def extract_edge_probabilities(input_dict):
    result_dict = {}

    for key, value in input_dict.items():
        for entry in value:
            a, b = entry[0]
            prob = entry[1]

            if (a, b) not in result_dict:
                result_dict[(a, b)] = []

            result_dict[(a, b)].append(prob)

    result_list = [(a, b, *probs) for (a, b), probs in result_dict.items()]
    return result_list


def get_probabilities_for_vertex(graph, vertex):
    probabilities = []
    for edge in graph:
        if edge[0] == vertex:
            probabilities.append(edge[2])
        elif edge[1] == vertex:
            probabilities.append(edge[3])
    return probabilities



# FUNCIONES PARA LA GESTIÓN DE PROBABILIDADES CON REDONDEO (no utilizadas)
def adjust_probabilities(edge_probabilities, n_vertices):
    for i in range(1, n_vertices + 1):
        probabilities_of_vertex = get_probabilities_for_vertex(edge_probabilities, i)
        difference = round(calculate_difference_from_1(probabilities_of_vertex), 2)
        if difference != 0.0 and len(probabilities_of_vertex) > 0:
            election = random.randint(0, len(probabilities_of_vertex) - 1)
            probabilities_of_vertex[election] = round(probabilities_of_vertex[election] - difference, 2)
            count = 0
            for edge in edge_probabilities:
                if i in edge[:2]:
                    count += 1
                    if count == election + 1:
                        if i == edge[0]:
                            new_edge = (edge[0], edge[1], probabilities_of_vertex[election], edge[3])
                            edge_probabilities.remove(edge)
                            edge_probabilities.append(new_edge)
                        elif i == edge[1]:
                            new_edge = (edge[0], edge[1], edge[2], probabilities_of_vertex[election])
                            edge_probabilities.remove(edge)
                            edge_probabilities.append(new_edge)

        new_difference = calculate_difference_from_1(probabilities_of_vertex)
    
    return edge_probabilities


def calculate_difference_from_1(probabilities):
    sum_of_probabilities = sum(probabilities)
    difference = sum_of_probabilities - 1
    return difference



# FUNCIONES PARA ESCRIBIR EL GRAFO EN EL FICHERO EN EL FORMATO SOLICITADO
def has_path(graph, start, end):
    def dfs(node, visited):
        visited.add(node)
        for neighbor in graph:
            if node in neighbor:
                next_node = neighbor[0] if neighbor[1] == node else neighbor[1]
                if next_node == end:
                    return True
                if next_node not in visited:
                    if dfs(next_node, visited):
                        return True
        return False

    visited_set = set()
    return dfs(start, visited_set)

def insert_random_number(lst):
    result = []
    for tup in lst:
        random_number = random.randint(1, 20)
        new_tup = tup[:2] + (random_number,) + tup[2:]
        result.append(new_tup)
    return result


def write_to_file(output_filename, lst):
    with open(output_filename, 'a') as file:
        for tup in lst:
            line = ' '.join(map(str, tup))
            file.write(line + '\n')


def generate_line_connected_or_not(output_filename):

    # CAMBIAR LOS VALORES QUÍ PARA LAS PRUEBAS
    N = 150
    M = 5625

    C = random.randint(1, N)
    A = random.randint(1, N)
    while A == C:
        A = random.randint(1, N)
    B = random.randint(1, N)
    while B == A or B == C:
        B = random.randint(1, N)
    
    line = f"{N} {M} {C} {A} {B}"
    with open(output_filename, 'w') as file:
        file.write(line + '\n')

    return [N, M, C, A, B]

output_filename = 'generated_graph.txt'


#-----------------CREACIÓN DE LOS GRAFOS PARA LAS PRUEBAS-------------------
# GENERACIÓN DE LA PRIMERA LÍNEA DEL FICHERO
first_line = generate_line_connected_or_not(output_filename)
N = first_line[0]   
M = first_line[1]
C = first_line[2]
A = first_line[3]
B = first_line[4]


# GRAFOS CONEXOS (y, por tanto, también completos)
raw_graph = random_connected_graph(N, M)

# GRAFOS INCONEXOS
# raw_graph = random_connected_or_not_graph(N, M)

# GRAFOS CÍCLICOS
# raw_graph = cycle_graph_edges(10)

# GRAFOS EN ESTRELLA
# raw_graph = star_graph_edges(10)

# GARANTIZA QUE C, A Y B SE ENCUENTRAN EN EL MISMO SUBGRAFO (en caso de que el grafo sea conexo)
while not (has_path(raw_graph, C, A) and has_path(raw_graph, C, B)):
           raw_graph = random_connected_or_not_graph(N, M)
           
new_raw_graph = sort_tuples(raw_graph)
vertices = list(range(1, N + 1))
occurrences = count_vertex_occurrences(new_raw_graph)
vertices_probabilities = calculate_vertex_probabilities(vertices, occurrences, new_raw_graph)
edges_probabilities = extract_edge_probabilities(vertices_probabilities)
graph_with_time = insert_random_number(edges_probabilities)
write_to_file(output_filename, graph_with_time)
