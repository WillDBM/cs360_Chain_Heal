/*  William Armentrout
    warmentr
    chain_heal.c
    Description:
*/ 

// Only what we are allowed to use
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct node{

	char *name;
	int x, y;
	int current_PP, max_PP;
	struct node *previous;

	int adj_size;
	struct node **adj;

} Node;

// from command line
int init_range, jump_range, num_jumps, init_power;
double power_reduction;
// Best path
int best_healing, best_path_length;
Node **best_path;
int *healing;


// helper for calculating distance between two nodes
float distance(Node *source, Node *destination){
	return sqrt(((destination->x - source->x) * (destination->x -source->x)) +((destination->y - source->y) * (destination->y - source->y)));
}



int main(int argc, char **argv){

	// command line managment
	if (argc != 6) {
		printf("Wrong Number of arguments");
		return 0;
	}

	init_range = atoi(argv[1]);
	jump_range = atoi(argv[2]);
	num_jumps = atoi(argv[3]);
	init_power = atoi(argv[4]);
	power_reduction = atoi(argv[5]);


	// Node creation
	int x, y;
	int current_PP, max_PP;
	char name[100];
	int num_nodes = 0;
	Node *prev = NULL;
	// loops untill all node have been created
	while(scanf("%d %d %d %d %s", &x, &y, &current_PP, &max_PP, name) == 5) {
		Node *node = (Node *)malloc(sizeof(Node));
		node->name = strdup(name);
		node->x = x;
		node->y = y;
		node->current_PP = current_PP;
		node->max_PP = max_PP;
		node->previous = prev;

		prev = node;
		num_nodes++;
	}

	// Creation of node array
	Node *nodes[num_nodes];

	for(int i = 0; i < num_nodes; i++) {
		nodes[i] = prev;
		prev = prev->previous;
	}

	// create a reference node for Urgosa in canse they aren't first
	Node *urgosa;

	// create adjacency list for nodes (creates graph)
	for(int i = 0; i < num_nodes; i++) {
		Node *current = nodes[i];
		current->adj_size = 0;

		//size of adj list
		for(int j = 0; j < num_nodes; j++) {
			// a node can't be in it's own adj list
			if (i == j) 
				continue;

			// must be reachable
			if (distance(current, nodes[j]) > jump_range)
				continue;

			current->adj_size++;
		}

		// allocate memory by multiplying size of node by list size
		current->adj = (Node **)malloc(sizeof(Node *) * current->adj_size);

		// keep track of adj list index
		int adj_index = 0;

		// finally add nodes to the index (I'm missing vectors)
		for(int j = 0; j < num_nodes; j++) {
			// same checks as the last loop
			if (i == j || distance(current, nodes[j]) > jump_range)
				continue;

			// add to list and increment index
			current->adj[adj_index] = nodes[j];
			adj_index++;
		}

		if (strcmp(current->name, "Urgosa_the_Healing_Shaman") == 0)
			urgosa = current;
	}

	// Allocation for path
	best_healing = 0;
	best_path_length = 0;
	best_path = (Node **)malloc(sizeof(Node *) * num_jumps);
	healing = (int *)malloc(sizeof(int *) * num_jumps);

	

	return 0;
}
