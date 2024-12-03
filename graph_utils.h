#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#define INF INT_MAX

// Structure for an adjacency list node
struct Node {
    int destination;
    int *weights;       // Array of weights
    int weightCount;    // Number of weights
    struct Node *next;
};

// Structure for an adjacency list
struct AdjacencyList {
    struct Node *head;
};

// Structure for a graph
struct Graph {
    int totalVertices;
    struct AdjacencyList *adjLists;
};

// Structure for a MinHeap node
struct HeapNode {
    int vertex;
    int distance;
};

// Structure for a MinHeap
struct MinHeap {
    struct HeapNode *nodes;
    int size;
    int capacity;
    int *positions; // Array to track positions of nodes
};

// Function to create a new adjacency list node
struct Node *createNode(int destination, int *weights, int weightCount) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed for Node\n");
        exit(EXIT_FAILURE);
    }
    newNode->destination = destination;
    newNode->weights = (int *)malloc(weightCount * sizeof(int));
    if (!newNode->weights) {
        fprintf(stderr, "Memory allocation failed for weights array\n");
        exit(EXIT_FAILURE);
    }
    memcpy(newNode->weights, weights, weightCount * sizeof(int));
    newNode->weightCount = weightCount;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph
struct Graph *createGraph(int vertices) {
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    if (!graph) {
        perror("Failed to allocate memory for graph");
        exit(EXIT_FAILURE);
    }
    graph->totalVertices = vertices;
    graph->adjLists = (struct AdjacencyList *)malloc(vertices * sizeof(struct AdjacencyList));
    if (!graph->adjLists) {
        perror("Failed to allocate memory for adjacency lists");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i].head = NULL;
    }
    return graph;
}

// Function to add an edge to the graph
void addEdge(struct Graph *graph, int source, int destination, int *weights, int weightCount) {
    struct Node *newNode = createNode(destination, weights, weightCount);
    newNode->next = graph->adjLists[source].head;
    graph->adjLists[source].head = newNode;
}

// Function to print the graph
void printGraph(struct Graph *graph) {
    for (int i = 0; i < graph->totalVertices; i++) {
        struct Node *current = graph->adjLists[i].head;
        printf("Vertex %d:\n", i);
        while (current) {
            printf(" -> %d [weights:", current->destination);
            for (int j = 0; j < current->weightCount; j++) {
                printf(" %d", current->weights[j]);
            }
            printf("]\n");
            current = current->next;
        }
        printf("\n");
    }
}

// Function to free the graph
void freeGraph(struct Graph *graph) {
    for (int i = 0; i < graph->totalVertices; i++) {
        struct Node *current = graph->adjLists[i].head;
        while (current) {
            struct Node *temp = current;
            current = current->next;
            free(temp->weights);
            free(temp);
        }
    }
    free(graph->adjLists);
    free(graph);
}

// Function to create a MinHeap
struct MinHeap *createMinHeap(int capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->nodes = (struct HeapNode *)malloc(capacity * sizeof(struct HeapNode));
    minHeap->positions = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

// Function to free a MinHeap
void freeMinHeap(struct MinHeap *minHeap) {
    free(minHeap->nodes);
    free(minHeap->positions);
    free(minHeap);
}

// Dijkstra's algorithm adapted for periodic weights
void dijkstra(struct Graph *graph, int source, int destination, int period) {
    int totalNodes = graph->totalVertices * period;
    int *distances = (int *)malloc(totalNodes * sizeof(int));
    int *predecessors = (int *)malloc(totalNodes * sizeof(int));
    bool *visited = (bool *)malloc(totalNodes * sizeof(bool));

    for (int i = 0; i < totalNodes; i++) {
        distances[i] = INF;
        predecessors[i] = -1;
        visited[i] = false;
    }

    distances[source * period] = 0;
    struct MinHeap *minHeap = createMinHeap(totalNodes);
    minHeap->nodes[0] = (struct HeapNode){source * period, 0};

    while (minHeap->size > 0) {
        struct HeapNode current = minHeap->nodes[0];
        if (visited[current.vertex]) {
            break;
        }
        visited[current.vertex] = true;

        struct Node *neighbor = graph->adjLists[current.vertex / period].head;
        while (neighbor) {
            int newDist = distances[current.vertex] + neighbor->weights[current.vertex % period];
            int neighborIndex = neighbor->destination * period + (current.vertex + 1) % period;

            if (newDist < distances[neighborIndex]) {
                distances[neighborIndex] = newDist;
                predecessors[neighborIndex] = current.vertex;
            }
            neighbor = neighbor->next;
        }
    }

    if (distances[destination * period] == INF) {
        printf("No path found from %d to %d\n", source, destination);
    } else {
        printf("Shortest path distance: %d\n", distances[destination * period]);
    }

    free(distances);
    free(predecessors);
    free(visited);
    freeMinHeap(minHeap);
}

#endif 
