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

	char visited;
	int healing;

} Node;

typedef struct global{

	// from command line
	int init_range, jump_range, num_jumps, init_power;
	double power_reduction;
	// Best path
	int best_healing, best_path_length;
	Node **best_path;
	int *healing;
} Global;


// helper for calculating distance between two nodes
float distance(Node *source, Node *destination){
	return sqrt(((destination->x - source->x) * (destination->x -source->x)) +((destination->y - source->y) * (destination->y - source->y)));
}

/* Perforsm DFS Recursivly
 *
 * source:		source node
 * previous:		last source node
 * global:		global struct
 * jumps:		number of jumps made so far
 * healing_total:	amount healed thus far
 * */
void DFS(Node *source, Node *previous, Global *global, int jumps, int healing_total) {
	//printf("DFS has been called\n");

	//exit cases
	if (source->visited || jumps > global->num_jumps){
		return;
	}

	//printf("made it pased\n");

	source->previous = previous;
	source->visited = 1;

	// calculates the largest possible amount of healing that can be done.
	source->healing = rint((global->init_power) * pow(1 - global->power_reduction, jumps - 1));
	// set healing lower if healing and current exceeds max
	if (source->healing + source->current_PP > source->max_PP) {
		source->healing = source->max_PP - source->current_PP;
		//printf("healing was adjusted to %d\n", source->healing);
	}
	
	//printf("healing is %d\n", source->healing);
	// add healing to total
	healing_total += source->healing;

	// update best healing if needed
	if(healing_total > global->best_healing) {
		global->best_healing = healing_total;
		global->best_path_length = jumps;

		Node *current = source;
		for(int i = 0; i < global->num_jumps && current != NULL; i++) {
			global->best_path[i] = current;
			global->healing[i] = current->healing;
			current = current->previous;
		}
	}

	// recursive call
	for(int i = 0; i < source->adj_size; i++) {
		DFS(source->adj[i], source, global, jumps + 1, healing_total);
	}

	// reset node when path is exhausted
	source->visited = 0;
	source->previous = NULL;
}



int main(int argc, char **argv){

	// command line managment
	if (argc != 6) {
		printf("Wrong Number of arguments\n");
		return 0;
	}

	Global global;
	global.init_range = atoi(argv[1]);
	global.jump_range = atoi(argv[2]);
	global.num_jumps = atoi(argv[3]);
	global.init_power = atoi(argv[4]);
	global.power_reduction = atoi(argv[5]);


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
	//printf("Nodes were properly created\n");
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
			if (distance(current, nodes[j]) > global.jump_range)
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
			if (i == j || distance(current, nodes[j]) > global.jump_range)
				continue;

			// add to list and increment index
			current->adj[adj_index++] = nodes[j];
		}

		if (strcmp(current->name, "Urgosa_the_Healing_Shaman") == 0)
			urgosa = current;
	}
	//printf("adj lists were properly created\n");

	// Allocation for path
	global.best_healing = 0;
	global.best_path_length = 0;
	global.best_path = (Node **)malloc(sizeof(Node *) * global.num_jumps);
	global.healing = (int *)malloc(sizeof(int *) * global.num_jumps);
	//printf("paths were properly allocated\n");


	// init ranges around urgosa
	for(int i = 0; i < num_nodes; i++) {
		// Set nodes to unvisited
		for(int j = 0; j < num_nodes; j++) {
			nodes[j]->visited = 0;
			nodes[j]->healing = 0;
			nodes[j]->previous = NULL;
		}

		if (distance(urgosa, nodes[i]) <= global.init_range) {
			DFS(nodes[i], NULL, &global, 1, 0);
		}
	}
	//printf("DFS properly completed\n");

	// output paths with amounts
	for(int i = global.best_path_length - 1; i >= 0; i--) {
		printf("%s %d\n", global.best_path[i]->name, global.healing[i]);
	}
	printf("Total_Healing %d\n", global.best_healing);

	// free up memory
	for (int i = 0; i < num_nodes; i++) {
		free(nodes[i]->adj);
		free(nodes[i]->name);
		free(nodes[i]);
	}

	free(global.best_path);
	free(global.healing);

	return 0;
}
