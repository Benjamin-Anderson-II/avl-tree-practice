#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include "avl.h"


int FindMinPath(struct AVLTree *tree, TYPE *path);
void printBreadthFirstTree(struct AVLTree *tree);
int getCost(TYPE *path, int len);
int recurse(struct AVLnode * curr, TYPE *currPath, int depth, TYPE *bestPath, int *bestCost, int *bestDepth);
void printCurrentLevel(struct AVLnode* curr, int level);

/* -----------------------
The main function
  param: argv = pointer to the name (and path) of a file that the program reads for adding elements to the AVL tree
*/
int main(int argc, char** argv) {

	FILE *file;
	int len, i;
	TYPE num; /* value to add to the tree from a file */
	struct timeval stop, start; /* variables for measuring execution time */
	int pathArray[100];  /* static array with values of nodes along the min-cost path of the AVL tree. The means that the depth of the AVL tree cannot be greater than 100 which is  sufficient for our purposes*/

	struct AVLTree *tree;
	
	tree = newAVLTree(); /*initialize and return an empty tree */
	
	file = fopen(argv[1], "r"); 	/* filename is passed in argv[1] */
	assert(file != 0);

	/* Read input file and add numbers to the AVL tree */
	while((fscanf(file, "%i", &num)) != EOF)
		addAVLTree(tree, num);		
	
	/* Close the file  */
	fclose(file);

        printf("\nThe AVL tree has %d nodes.\n",tree->cnt);
	
	printf("\nPrinting the AVL tree breadth-first : \n");
	printBreadthFirstTree(tree);

	gettimeofday(&start, NULL);

	/* Find the minimum-cost path in the AVL tree*/
	len = FindMinPath(tree, pathArray);
	
	gettimeofday(&stop, NULL);

	/* Print out all numbers on the minimum-cost path */
	printf("\n\nThe minimum-cost path has %d nodes printed top-down from the root to the leaf: \n", len);
	for(i = 0; i < len; i++)
		printf("%d ", pathArray[i]);
	printf("\n");

	printf("\nYour execution time to find the mincost path is %f microseconds\n", (double) (stop.tv_usec - start.tv_usec));

        /* Free memory allocated to the tree */
	deleteAVLTree(tree); 
	
	return 0;
}


  
/* --------------------
Finds the minimum-cost path in an AVL tree
   Input arguments: 
        tree = pointer to the tree,
        path = pointer to array that stores values of nodes along the min-cost path, 
   Output: return the min-cost path length 

   pre: assume that
       path is already allocated sufficient memory space 
       tree exists and is not NULL
*/
int FindMinPath(struct AVLTree *tree, TYPE *path)
{
	int bestDepth = 99999;
	int bestCost = 99999;
	TYPE currPath[100];
	return recurse(tree->root, currPath, 1, path, &bestCost, &bestDepth);
}

/* --------------------
Recursively walks through the tree and finds the minimum cost path
	Input arguments:
		curr: pointer to current node
		currPath: path to the current node
		depth: How how deep into the tree (root = 1)
		bestPath: The current min-cost Path
		bestCost: The cost of the current best Path
		bestDepth: The depth of the leaf of the current best path
	Output: returns the length of the current bestPath

	pre: assume that when initially called
		currPath and bestPath are different variables
		bestCost is larger than the value of any node
		bestDepth is larger than the depth of the tree
*/
int recurse(struct AVLnode * curr, TYPE *currPath, int depth, TYPE *bestPath, int *bestCost, int *bestDepth)
{
	int cost;
	if(curr == 0) 
		return *bestDepth;

	currPath[depth-1] = curr->val;
	cost = getCost(currPath, depth-1);

	if(cost > *bestCost) 
		return *bestDepth;

	else if(curr->height == 0){
		int i;
		for(i = 0; i < depth; i++)
			bestPath[i] = currPath[i];
		*bestCost = cost;
		*bestDepth = depth;

	} else {
		recurse(curr->left, currPath, depth+1, bestPath, bestCost, bestDepth);
		recurse(curr->right, currPath, depth+1, bestPath, bestCost, bestDepth);
	}
	return *bestDepth;
}

/* --------------------
Gets the cost of the input path
	Input arguments:
		path: path to the current node
		len: How how deep into the tree current node is (root = 1)
	Output: returns the cost of the input path
*/
int getCost(TYPE *path, int len)
{
	int sum = 0;
	int i;
	for(i = 0; i < len; i++){
		sum += abs(path[i]-path[i+1]);
	}
	return sum;
}

/* -----------------------
Printing the contents of an AVL tree in breadth-first fashion
  param: pointer to a tree
  pre: assume that tree was initialized well before calling this function
*/
void printBreadthFirstTree(struct AVLTree *tree)
{
	int height = tree->root->height+1;
	int i;
	for(i = 1; i <= height; i++)
		printCurrentLevel(tree->root, i);
}

void printCurrentLevel(struct AVLnode *curr, int level)
{
	if(curr == 0)
		return;
	if(level == 1)
		printf("%d ", curr->val);
	else if(level > 1){
		printCurrentLevel(curr->left, level - 1);
		printCurrentLevel(curr->right, level - 1);
	}
}


