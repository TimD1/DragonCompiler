#ifndef CODE_H
#define CODE_H

#include "tree.h"

/* STRUCTURES */

typedef struct record_s {

	struct record_s *parent;
} record_t;


/* GLOBALS */
extern FILE* outfile;


/* FUNCTIONS */
void file_header(char* filename);
void file_footer();
char* ia64(char* opval);
char* string_value(tree_t* n);
void gencode(tree_t* n);

#endif
