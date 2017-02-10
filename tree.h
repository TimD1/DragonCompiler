#ifndef TREE_H
#define TREE_H


/* Semantic analyzer's channel with parser */
typedef struct tree_s {
	/* token type */
	int type;
	
	/* actual value of token */
	union {
		int ival;
		float fval; 
		char *opval;	
		char *sval;
	} attribute;

	/* children nodes */
	struct tree_s *left, *right;

} tree_t;

tree_t* make_tree(int type, tree_t *left, tree_t *right);
void print_tree(tree_t *t, int spaces);
int eval_tree(tree_t *t);


#endif
