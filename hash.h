#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 211

/* STRUCTURES */
/* Single entry structure */
typedef struct entry_s {
	char *name;
	int value;
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

entry_t* create_entry(char* name);
entry_t* get_entry(table_t* table, char* name);
entry_t* find_entry(table_t* table, char* name); 
int insert_entry(char* name, table_t* table);

table_t* create_table();
table_t* push_table();
table_t* top_table();
void print_table(table_t* table);
void pop_table();

/* GLOBALS */
extern table_t* head_table;

#endif
