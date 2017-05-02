#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 211

typedef struct tree_s tree_t;

/* UNIONS */
typedef union {
	int ival;
	float fval;
	char *sval;

	// arrays only
	int* aival;
	float* afval;
} value;

/* STRUCTURES */
/* Single entry structure */
typedef struct entry_s {
	char *entry_name;	// identifier name			numItems, i
	int entry_class;	// type 					function, array, var  ...param?
	value entry_value;  // value					13, 3.1415, 'Sam'
	int return_type;	// return type				RNUM, INUM
	int id;
	
	// functions only
	int arg_num;		// number of arguments		0, 5
	int *arg_types;		// list of argument types	[INUM RNUM], [STRING]
	
	// arrays only
	int start_idx;
	int stop_idx;

	struct entry_s *next; // linked list for collision
} entry_t;

/* Hash table structure */
typedef struct table_s {
	entry_t *hash_table[ TABLE_SIZE ];
	int id;
	int table_size;
	int num_params;
	int num_locals;
	char* table_name;
	int table_class;
	struct table_s *parent;

	// use a double linked list as a stack
	struct table_s *prev;
	struct table_s *next;
} table_t;


/* FUNCTIONS */
int hashpjw(char *s);

char* type_string(int token);
char* class_string(int token);
char* arg_string(int argnum, int* args);
int num_keyword_to_type(int token);

void make_vars(tree_t* var_ptr, tree_t* type_ptr);
void add_io(tree_t* ident_list_ptr);
void add_params(tree_t* param_ptr);
void make_function(tree_t* fn_ptr, tree_t* type_ptr);
void make_procedure(tree_t* proc_ptr);
int count_args(tree_t* arg_ptr);
int* get_args(tree_t* arg_ptr, int arg_num);

entry_t* make_entry( char* entry_name, int entry_class, value* val_ptr, int return_type, int arg_num, int* arg_types, int start_idx, int stop_idx);

void make_var_io(char* name);
void make_var_inum(char* name);
void make_var_rnum(char* name);
void make_arr_inum(char* name, int start_idx, int stop_idx);
void make_arr_rnum(char* name, int start_idx, int stop_idx);

entry_t* get_entry(table_t* table, char* name);
int get_entry_id(table_t* table, char* name);
entry_t* find_entry(table_t* table, char* name); 
int insert_entry(entry_t* entry_ptr, table_t* table);

table_t* create_table();
table_t* push_table(char* name, int class_);
table_t* top_table();
void print_table(table_t* table);
void pop_table();

/* GLOBALS */
extern table_t* head_table;
extern table_t* caller;

#endif
