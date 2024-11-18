#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 1000
#define INF INT_MAX

typedef struct {
    int target;
    int *weights;
} Edge;

typedef struct {
    Edge **edges;
    int edge_count;
} Vertex;

typedef struct {
    int weight;
    int vertex;
    int step;
} Node;

typedef struct {
    Node *nodes;
    int size;
} PriorityQueue;

void init_priority_queue(PriorityQueue *pq, int max_size) {
    pq->nodes = (Node *)malloc(max_size * sizeof(Node));
    pq->size = 0;
}

void push(PriorityQueue *pq, Node node) {
    int i = pq->size++;
    while (i > 0 && pq->nodes[(i - 1) / 2].weight > node.weight) {
        pq->nodes[i] = pq->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    pq->nodes[i] = node;
}

Node pop(PriorityQueue *pq) {
    Node root = pq->nodes[0];
    Node temp = pq->nodes[--pq->size];
    int i = 0, child;
    while (i * 2 + 1 < pq->size) {
        child = i * 2 + 1;
        if (child + 1 < pq->size && pq->nodes[child].weight > pq->nodes[child + 1].weight) {
            child++;
        }
        if (temp.weight <= pq->nodes[child].weight) break;
        pq->nodes[i] = pq->nodes[child];
        i = child;
    }
    pq->nodes[i] = temp;
    return root;
}

int is_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

void add_edge(Vertex *graph, int src, int dest, int *weights, int period) {
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    edge->target = dest;
    edge->weights = (int *)malloc(period * sizeof(int));
    for (int i = 0; i < period; i++) {
        edge->weights[i] = weights[i];
    }
    graph[src].edges[graph[src].edge_count++] = edge;
}

void shortest_path(Vertex *graph, int V, int N, int start, int end) {
    PriorityQueue pq;
    init_priority_queue(&pq, V * N);

    int **distances = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        distances[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            distances[i][j] = INF;
        }
    }

    int *prev = (int *)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        prev[i] = -1;
    }

    distances[start][0] = 0;
    push(&pq, (Node){0, start, 0});

    while (!is_empty(&pq)) {
        Node node = pop(&pq);
        int curr_vertex = node.vertex;
        int curr_weight = node.weight;
        int step = node.step;

        if (curr_vertex == end) {
            break;
        }

        for (int i = 0; i < graph[curr_vertex].edge_count; i++) {
            Edge *edge = graph[curr_vertex].edges[i];
            int next_vertex = edge->target;
            int weight = edge->weights[step % N];
            int new_weight = curr_weight + weight;

            if (new_weight < distances[next_vertex][(step + 1) % N]) {
                distances[next_vertex][(step + 1) % N] = new_weight;
                prev[next_vertex] = curr_vertex;
                push(&pq, (Node){new_weight, next_vertex, step + 1});
            }
        }
    }

    if (distances[end][0] == INF) {
        printf("No path found from %d to %d.\n", start, end);
    } else {
        int path[MAX_VERTICES];
        int count = 0;
        for (int v = end; v != -1; v = prev[v]) {
            path[count++] = v;
        }
        for (int i = count - 1; i >= 0; i--) {
            printf("%d ", path[i]);
        }
        printf("\n");
    }

    for (int i = 0; i < V; i++) {
        free(distances[i]);
    }
    free(distances);
    free(prev);
    free(pq.nodes);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <graph_file> <queries_file>\n", argv[0]);
        return 1;
    }

    FILE *graph_file = fopen(argv[1], "r");
    if (!graph_file) {
        perror("Error opening graph file");
        return 1;
    }

    int V, N;
    fscanf(graph_file, "%d %d", &V, &N);

    Vertex graph[MAX_VERTICES];
    for (int i = 0; i < V; i++) {
        graph[i].edges = (Edge **)malloc(V * sizeof(Edge *));
        graph[i].edge_count = 0;
    }

    int src, dest;
    int weights[MAX_VERTICES];
    while (fscanf(graph_file, "%d %d", &src, &dest) == 2) {
        for (int i = 0; i < N; i++) {
            fscanf(graph_file, "%d", &weights[i]);
        }
        add_edge(graph, src, dest, weights, N);
    }
    fclose(graph_file);

    FILE *queries_file = fopen(argv[2], "r");
    if (!queries_file) {
        perror("Error opening queries file");
        return 1;
    }

    while (fscanf(queries_file, "%d %d", &src, &dest) == 2) {
        shortest_path(graph, V, N, src, dest);
    }
    fclose(queries_file);

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < graph[i].edge_count; j++) {
            free(graph[i].edges[j]->weights);
            free(graph[i].edges[j]);
        }
        free(graph[i].edges);
    }

    return 0;
}
