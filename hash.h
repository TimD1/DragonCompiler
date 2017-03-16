#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 211

/* ENUMERATIONS */
typedef enum {IDK, FLT, INT, STR} type;

/* UNIONS */
typedef union {
	int ival;
	float fval;
	char *sval;
} value;

/* STRUCTURES */
/* Single entry structure */
typedef struct entry_s {
	char *entry_name;	// identifier name			numItems, i
	type entry_type;	// type 					function, array, var, param
	value entry_value;  // value					13, 3.1415, 'Sam'
	int arg_num;		// number of arguments		0, 5
	type *arg_types;	// list of argument types	[REAL, INT], [STRING]
	type return_type;	// return type				REAL
	
	struct entry_s *next; // linked list for collision
} entry_t;

/* Hash table structure */
typedef struct table_s {
	entry_t *hash_table[ TABLE_SIZE ];
	int id;

	// use a double linked list as a stack
	struct table_s *prev;
	struct table_s *next;
} table_t;


/* FUNCTIONS */
int hashpjw(char *s);

entry_t* create_entry(char* name, type entry_type, int entry_ival, float entry_fval, char* entry_sval, int arg_num, type* arg_types, type return_type);
entry_t* get_entry(table_t* table, char* name);
entry_t* find_entry(table_t* table, char* name); 
int insert_entry(entry_t* entry_ptr, table_t* table);

table_t* create_table();
table_t* push_table();
table_t* top_table();
void print_table(table_t* table);
void pop_table();

/* GLOBALS */
extern table_t* head_table;

#endif
