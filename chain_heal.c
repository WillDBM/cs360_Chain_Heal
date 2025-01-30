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

int init_range, jump_range, num_jumps, init_power;
double power_reduction;

typedef struct node{

	int x, y;
	int current_PP, max_PP;
	struct node *previous;

} Node;

int main(int argc, char *argv[]){

	if (argc != 6) {
		printf("Wrong Number of arguments");
		return 0;
	}


	return 0;
}
