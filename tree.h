#ifndef TREE_H
#define TREE_H


typedef struct tree_s {
	/* Semantic analyzer's channel with parser */
	int type; /* NUM ADDOP RELOP MULOP */
	
	union {
		int ival;	/* INUM */
		float fval; /* RNUM */
		char opval;	/* ADDOP, MULOP */
		char *sval;
	} attribute;

	struct tree_s *left, *right;

} tree_t;

tree_t* make_tree(int type, tree_t *left, tree_t *right);
void print_tree(tree_t *t, int spaces);
int eval_tree(tree_t *t);


#endif
