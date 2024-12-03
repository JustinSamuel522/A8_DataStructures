#include "graph_utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <graph_file> <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *graphFile = fopen(argv[1], "r");
    if (!graphFile) {
        perror("Error opening graph file");
        return EXIT_FAILURE;
    }

    int vertexCount = 0, weightCount = 0;
    if (fscanf(graphFile, "%d %d", &vertexCount, &weightCount) != 2) {
        fprintf(stderr, "Invalid graph file format\n");
        fclose(graphFile);
        return EXIT_FAILURE;
    }

    struct Graph *graph = initializeGraph(vertexCount);
    int start = 0, target = 0;
    int *weights = malloc(weightCount * sizeof(int));
    if (!weights) {
        perror("Memory allocation failed");
        fclose(graphFile);
        return EXIT_FAILURE;
    }

    while (fscanf(graphFile, "%d %d", &start, &target) == 2) {
        for (int i = 0; i < weightCount; i++) {
            fscanf(graphFile, "%d", &weights[i]);
        }
        addConnection(graph, start, target, weights, weightCount);
    }

    fclose(graphFile);

    int sourceNode = 0, destinationNode = 0;
    while (scanf("%d %d", &sourceNode, &destinationNode) != EOF) {
        findShortestPath(graph, sourceNode, destinationNode, weightCount);
    }

    freeGraph(graph);
    free(weights);

    return EXIT_SUCCESS;
}