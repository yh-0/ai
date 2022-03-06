#include <stdio.h>
#include <stdlib.h>
#define N 5

// Traveling Salesman Problem

struct Graph
{
	int** adjList;
};

struct Graph* createGraph()
{
	// allocating graph and adjList
	struct Graph* graph = malloc(sizeof(struct Graph));
	graph->adjList = malloc(N * sizeof(int*));
	
	int i, j;
	
	// allocating every array in adjList
	for(i = 0; i < N; i++)
		graph->adjList[i] = malloc(N * sizeof(int));
	
	// filling adjList with 0s
	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			graph->adjList[i][j] = 0;
	
	return graph;
}

// adding edges to a non-oriented graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
	graph->adjList[src][dest] = weight;
	graph->adjList[dest][src] = weight;
}

void printGraph(struct Graph* graph)
{
	int i, j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
			printf("%d ", graph->adjList[i][j]);
		printf("\n");
	}
}

// finding if array contains given value
int arrayContains(int* array, int value)
{
	int i;
	for(i = 0; i < N; i++)
		if(array[i] == value) 
			return 1;
		
	return 0;
}

int arrayMinIndex(int* array, int* visited)
{
	// setting minimum to 1000
	int i, min = 1000, minInd = -1;
	int* cpArray = malloc(N * sizeof(int*));
	
	// copying argument array
	for(i = 0; i < N; i++) cpArray[i] = array[i];
	
	// setting values of visited nodes to 1000
	for(i = 0; i < N; i++)
		if(arrayContains(visited, i)) 
			cpArray[i] = 1000;
	
	// finding index of minimal value in array
	for(i = 0; i < N; i++)
		if(cpArray[i] > 0 && cpArray[i] < min)
		{
			minInd = i;
			min = cpArray[i];
		}
	
	free(cpArray);
	return minInd;
}

void tsp(struct Graph* graph, int start)
{
	printf("Path starting from %d: ", start);
	
	int i, curr, tmp, length = 0;
	int* visited = malloc(N * sizeof(int));
	for(i = 0; i < N; i++) visited[i] = -1;
	
	curr = start;
	
	for(i = 0; i < N; i++)
	{
		// adding current node to visited (path)
		visited[i] = curr;
		
		// setting tmp to index of minimal value of edge from current node neighbours (closest neighbour)
		tmp = arrayMinIndex(graph->adjList[curr], visited);
		if(i < N - 1)
		{
			// increasing length by length of the edge between current node and tmp node
			length += graph->adjList[curr][tmp];
			
			// changing current node to tmp (closest neighbour)
			curr = tmp;
		}
	}
	
	// increasing length by length of edge between last node and starting node
	length += graph->adjList[curr][start];

	for(i = 0; i < N; i++) printf("%d ", visited[i]);
	printf("%d ; length = %d\n", start, length);
	
	free(visited);
}

int main()
{
	struct Graph* graph = createGraph();
	
	addEdge(graph,0,1,15);
	addEdge(graph,0,2,12);
	addEdge(graph,0,3,6);
	addEdge(graph,0,4,21);
	addEdge(graph,1,2,3);
	addEdge(graph,1,3,10);
	addEdge(graph,1,4,4);
	addEdge(graph,2,3,11);
	addEdge(graph,2,4,26);
	addEdge(graph,3,4,17);
		
	printf("Graph:\n");
	printGraph(graph);

	int i;
	for(i = 0; i < N; i++) tsp(graph, i);
}
