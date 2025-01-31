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

	return 0;
}
