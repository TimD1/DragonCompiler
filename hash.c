#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"
#include "tree.h"
#include "y.tab.h"

table_t* head_table;
// didn't use strdup in many places, dependent on syntax tree staying

/* Peter J. Weinberger's hash function, from Red Dragon Book */
int hashpjw( char *s )
{
	char *p; 
	unsigned h = 0, g; 

	for ( p = s; *p != '\0'; p++ ) 
	{ 
		h = (h << 4) + (*p); 
		if ( g = h & 0xf0000000 ) 
		{ 
			h = h ^ ( g >> 24 ); 
			h = h ^ g; 
		} 
	} 
	return h % TABLE_SIZE; 
}


/* Push new table onto the stack */
table_t* push_table()
{
	// create and initialize new table
	table_t* ptr = (table_t*)malloc(sizeof(table_t));
	for(int i = 0; i < TABLE_SIZE; i++)
		ptr->hash_table[i] = NULL;

	// set new table's pointers
	ptr->next = head_table;
	ptr->prev = head_table->prev;
	ptr->id = ptr->prev->id + 1;

	// adjust surrounding pointers
	head_table->prev->next = ptr;
	head_table->prev = ptr;
}


/* Pop top table from the stack */
void pop_table()
{
	// ensure we don't pop the head of our list
	table_t* top_table = head_table->prev;
	if(top_table == head_table)
	{
		fprintf(stderr, "Cannot pop head table!");
		return;
	}

	// remove linked lists of entries
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		while(1)
		{
			entry_t* cur_entry = top_table->hash_table[i];
			
			// no entries, we're done
			if(cur_entry == NULL)
				break;
			// one entry, remove it and exit
			else if(cur_entry->next == NULL)
			{
				free(cur_entry->entry_name);
				free(cur_entry->arg_types);
				free(cur_entry);
				cur_entry = NULL;
				break;
			}
			// two or more entries, delete last
			else
			{
				// get second to last entry
				while(cur_entry->next->next != NULL)
					cur_entry = cur_entry->next;
				
				// remove last entry
				free(cur_entry->next->entry_name);
				free(cur_entry->next->arg_types);
				free(cur_entry->next);
				cur_entry->next = NULL;
			}

		}
	}


	// adjust surrounding pointers to avoid table
	top_table->prev->next = head_table;
	head_table->prev = top_table->prev;

	// remove top table
	top_table->next = NULL;
	top_table->prev = NULL;
	free(top_table);
}


/* Return top table */
table_t* top_table()
{
	return head_table->prev;
}	


/* Prints the specified table */
// add different printing for functions later
void print_table(table_t* table)
{
	fprintf(stderr, "\n\n\t\tHASH TABLE %d\n____________________________________________\n", table->id);
	fprintf(stderr, "\nname\tclass\ttype\tvalue\t\targs\n\n");
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		entry_t* entry = table->hash_table[i];
		while(entry != NULL)
		{
			
			switch(entry->entry_class)
			{
			case FUNCTION:
				switch(entry->return_type)
				{
				case INUM:
					fprintf(stderr, "%s\t%s\t%s\t%s\t\t%d\n", 
						entry->entry_name, class_string(entry->entry_class), 
						type_string(entry->return_type), "none", entry->arg_num);
					break;
				case RNUM:
					fprintf(stderr, "%s\t%s\t%s\t%s\t\t%d\n", 
						entry->entry_name, class_string(entry->entry_class), 
						type_string(entry->return_type), "none", entry->arg_num);
					break;
				default:
					fprintf(stderr, "\nERROR: function of unknown type.\n");
				}
				break;

			case PROCEDURE:
				fprintf(stderr, "%s\t%s\t%s\t%s\t\t%d\n", 
					entry->entry_name, class_string(entry->entry_class), 
					"none", "none", entry->arg_num);
				break;

			case ARRAY:
				switch(entry->return_type)
				{
				case INUM:
					fprintf(stderr, "%s\t%s\t%s\t%p\t%d\n", 
						entry->entry_name, class_string(entry->entry_class), 
						type_string(entry->return_type), entry->entry_value.aival, 
						entry->arg_num);
					break;
				case RNUM:
					fprintf(stderr, "%s\t%s\t%s\t%p\t%d\n", 
						entry->entry_name, class_string(entry->entry_class), 
						type_string(entry->return_type), entry->entry_value.afval, 
						entry->arg_num);
					break;
				default:
					fprintf(stderr, "\nERROR: array of unknown type.\n");
				}
				break;

			case VAR:
				switch(entry->return_type)
				{
				case INUM:
					fprintf(stderr, "%s\t%s\t%s\t%d\t\t%d\n", 
						entry->entry_name, class_string(entry->entry_class), 
						type_string(entry->return_type), entry->entry_value.ival, 
						entry->arg_num);
					break;
				case RNUM:
					fprintf(stderr, "%s\t%s\t%s\t%f\t%d\n", 
						entry->entry_name, class_string(entry->entry_class), 
						type_string(entry->return_type), entry->entry_value.fval, 
						entry->arg_num);
					break;
				case STRING:
					fprintf(stderr, "%s\t%s\t%s\t%s\t\t%d\n", 
						entry->entry_name, class_string(entry->entry_class), 
						type_string(entry->return_type), entry->entry_value.sval, 
						entry->arg_num);
					break;
				case PROGRAM:
					fprintf(stderr, "%s\t%s\t%s\t%d\t\t%d\n", 
						entry->entry_name, class_string(entry->entry_class), 
						type_string(entry->return_type), entry->entry_value.ival, 
						entry->arg_num);
					break;
				default:
					fprintf(stderr, "\nERROR: variable of unknown type.\n");
					break;
				}
				break;
			default:
				fprintf(stderr, "\nERROR: object of unknown class.\n");
				break;

			}
			entry = entry->next;
		}
	}
	fprintf(stderr, "\n\n");
}


/* Given a token, returns matching type string for printing */
char* type_string(int token)
{
	switch(token)
	{
		case RNUM:
			return "real";
		case INUM:
			return "int";
		case STRING:
			return "string";
		case PROGRAM:
			return "i/o";
		case EMPTY:
			return "empty";
		case INTEGER:
			return "integer keyword"; // BAD!
		case REAL:
			return "real keyword"; // BAD!
		default:
			return "other";
	}
}


/* Given a token, returns matching class string for printing */
char* class_string(int token)
{
	switch(token)
	{
		case VAR:
			return "var";
		case ARRAY:
			return "array";
		case FUNCTION:
			return "fcn";
		case PROCEDURE:
			return "proc";
		default:
			return "other";
	}
}


/* Allocate memory for new hash table entry, and return pointer */
entry_t* make_entry(char* name, int entry_class, value* val_ptr, int return_type, int arg_num, int* arg_types, int start_idx, int stop_idx)
{
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->entry_name = strdup(name);
	ptr->entry_class = entry_class;
	ptr->return_type = return_type;
	
	if(entry_class == VAR) // or VAR_PARAM
	{
		switch(return_type)
		{
		case INUM:
			ptr->entry_value.ival = val_ptr->ival;
			break;
		case RNUM:
			ptr->entry_value.fval = val_ptr->fval;
			break;
		case STRING:
			ptr->entry_value.sval = val_ptr->sval;
			break;
		default:
			break;
		}
	}
	
	ptr->arg_num = arg_num;
	
	if(arg_num > 0)
	{
		ptr->arg_types = (int*)malloc(arg_num*sizeof(int));
		memcpy(ptr->arg_types, arg_types, arg_num*sizeof(int));
	}
	else
	{
		ptr->arg_types = NULL;
	}
	
	ptr->start_idx = start_idx;
	ptr->stop_idx = stop_idx;
	ptr->next = NULL;
	return ptr;
}


/* Given an identifier name and hash table,
   finds entry in hash table if it exists */
entry_t* get_entry(table_t* table, char* name)
{
	entry_t* cur_entry = table->hash_table[ hashpjw(name) ];
	while(cur_entry != NULL)
	{
		if( !strcmp(name, cur_entry->entry_name) )
			return cur_entry;
		else
			cur_entry = cur_entry->next;
	}
	return NULL;
}


/* Given an identifier name and hash table,
   finds entry in hash table AND ALL OTHER
   HASH TABLES OF GREATER SCOPE if it exists */
entry_t* find_entry(table_t* table, char* name)
{
	// search all tables up to head table
	while(table != head_table)
	{
		entry_t* cur_entry = table->hash_table[ hashpjw(name) ];
		while(cur_entry != NULL)
		{
			if( !strcmp(name, cur_entry->entry_name) )
				return cur_entry;
			else
				cur_entry = cur_entry->next;
		}
		table = table->prev;
	}
	return NULL;
}


/* Given previously created entry and hash table,
   insert entry into hash table if not already there */
int insert_entry(entry_t* entry_ptr, table_t* table)
{
	// entry already exists in table
	if(get_entry(table, entry_ptr->entry_name) != NULL)
	{
		fprintf(stderr, "\nERROR: entry '%s' already exists within current scope.\n", entry_ptr->entry_name);
		exit(0);
	}

	else // create new entry
	{
		entry_t* last_entry = table->hash_table[ hashpjw(entry_ptr->entry_name) ];
		// if list non-empty, append to end
		if(last_entry != NULL)
		{
			while(last_entry->next != NULL)
				last_entry = last_entry->next;
			last_entry->next = entry_ptr;
		}
		else // create start of list
		{
			table->hash_table[hashpjw(entry_ptr->entry_name)] = entry_ptr;
		}
		return 1;
	}
}


/* Converts keywords INTEGER and REAL to INUM and RNUM */
int num_keyword_to_type(int token)
{
	switch(token)
	{
	case INTEGER:
		return INUM;
		break;
	case REAL:
		return RNUM;
		break;
	default:
		return EMPTY;
		break;
	}
}


/* Given pointer to FUNCTION in syntax tree, add function to symbol table */
void make_function(tree_t* fn_ptr, tree_t* type_ptr)
{

	int fn_class = FUNCTION;
	char* fn_name;
	int fn_arg_num = 0;
	int* fn_args;
	int fn_type = num_keyword_to_type(type_ptr->type);
	
	if(fn_ptr->type != PARENOP) // function takes no args
	{
		fn_name = fn_ptr->attribute.sval;
		entry_t* entry_ptr = 
			make_entry(fn_name, fn_class, NULL, fn_type, 0, NULL, 0, 0);
		insert_entry(entry_ptr, top_table()->prev); // function can be called
		
		// need two copies as hash table prevents memory leaks
		entry_t* entry_ptr2 = 
			make_entry(fn_name, fn_class, NULL, fn_type, 0, NULL, 0, 0);
		insert_entry(entry_ptr2, top_table()); // allows recursion and returning
	}
	else // function does take args
	{
		fn_name = fn_ptr->left->attribute.sval;
		fn_arg_num = count_args(fn_ptr->right);
		fn_args = get_args(fn_ptr->right, fn_arg_num);
		entry_t* entry_ptr = make_entry(fn_name, 
			fn_class, NULL, fn_type, fn_arg_num, fn_args, 0, 0);
		insert_entry(entry_ptr, top_table()->prev); // function can be called
		
		// need two copies as hash table prevents memory leaks
		entry_t* entry_ptr2 = 
			make_entry(fn_name, fn_class, NULL, fn_type, 0, NULL, 0, 0);
		insert_entry(entry_ptr2, top_table()); // allows recursion and returning
	}
}

/* Given pointer to PROCEDURE in syntax tree, add procedure to symbol table */
void make_procedure(tree_t* proc_ptr)
{
	int proc_class = PROCEDURE;
	char* proc_name;
	int proc_arg_num = 0;
	int* proc_args;
	int proc_type = EMPTY;
	
	if(proc_ptr->type != PARENOP) // procedure takes no args
	{
		proc_name = proc_ptr->attribute.sval;
		entry_t* entry_ptr = 
			make_entry(proc_name, proc_class, NULL, proc_type, 0, NULL, 0, 0);
		insert_entry(entry_ptr, top_table()->prev);
	}
	else // procedure does take args
	{
		proc_name = proc_ptr->left->attribute.sval;
		proc_arg_num = count_args(proc_ptr->right);
		proc_args = get_args(proc_ptr->right, proc_arg_num);
		entry_t* entry_ptr = make_entry(proc_name, 
			proc_class, NULL, proc_type, proc_arg_num, proc_args, 0, 0);
		insert_entry(entry_ptr, top_table()->prev);
	}
}


/* If a function, check that return statement exists and is of correct type */
void check_function(tree_t* head_ptr, tree_t* body_ptr)
{
	if(head_ptr->type == FUNCTION)
	{
		int fn_type = num_keyword_to_type(head_ptr->right->type);
		head_ptr = head_ptr->left;
		if(head_ptr->type == PARENOP)
			head_ptr = head_ptr->left;
		char* fn_name = head_ptr->attribute.sval;
		
		// functions must have return statements
		tree_t* return_stmt = find_return_stmt(fn_name, body_ptr);
		if(return_stmt == NULL)
		{
			fprintf(stderr, "\nERROR: return statement missing from function '%s'.\n", 
				fn_name);
			exit(0);
		}

		// return statement must be of correct type
		if(type(return_stmt->right) != fn_type)
		{
			fprintf(stderr, "\nERROR: '%s's return statement of type '%s' given argument of type '%s'.\n", fn_name, type_string(fn_type), type_string(type(return_stmt->right)));
			exit(0);
		}
	}
}


/* Recursively search for return statement, given name and tree. */
tree_t* find_return_stmt(char* fn_name, tree_t* t)
{
	// give up at leaves
	if(t == NULL || t->type == EMPTY)
		return NULL;

	// return statement found!
	if(t->type == ASSOP && !strcmp(t->left->attribute.sval, fn_name))
		return t;

	// recursively explore left and right children
	else
	{
		tree_t* left = find_return_stmt(fn_name, t->left);
		tree_t* right = find_return_stmt(fn_name, t->right);
		if(right != NULL)
			return right;
		if(left != NULL)
			return left;
	}

	// return statement not in subtree
	return NULL;
}


/* Counts arguments to function, assuming one identifier listed at a time
   (no identifier lists 'a,b:integer' are given) */
int count_args(tree_t* arg_ptr)
{
	int count = 0;
	while(arg_ptr->type == LISTOP)
	{
		count++;
		arg_ptr = arg_ptr->left;
	}
	return count;
}

/* Returns a list of types, given a pointer to a function declaration 
   in the syntax tree. Also assumes no identifier lists are used. */
int* get_args(tree_t* arg_ptr, int arg_num)
{
	int* type_list_ptr = (int*)malloc(sizeof(int)*arg_num);
	for(int i = 0; i < arg_num; i++)
	{
		type_list_ptr[i] = num_keyword_to_type(arg_ptr->right->right->type);
		arg_ptr = arg_ptr->left;
	}
	return type_list_ptr;
}


/* Given pointer to "VAR" node in syntax tree, traverse and insert all 
   following variables into the current scope.
*/
void make_vars(tree_t* var_ptr, tree_t* type_ptr)
{
	int var_type = 0;
	int var_class = 0;
	int start_idx = 0;
	int stop_idx = 0;

	// determine and store type info
	if(type_ptr->type == ARRAY) // handle arrays
	{
		var_class = ARRAY;
		var_type = type_ptr->right->type;
		
		type_ptr = type_ptr->left;
		if(type_ptr->type != DOTDOT)
		{
			fprintf(stderr, "\nERROR: array index missing DOTDOT.\n");
			exit(0);
		}

		if(type_ptr->left->type != INUM || type_ptr->right->type != INUM)
		{
			fprintf(stderr, "\nERROR: array indices must be integral.\n");
			exit(0);
		}

		start_idx = type_ptr->left->attribute.ival;
		stop_idx = type_ptr->right->attribute.ival;
	}
	else // handle basic types
	{
		var_class = VAR;
		var_type = type_ptr->type;
	}

	//while there are more identifiers of this type
	while(var_ptr->type == LISTOP)
	{
		char* var_name = var_ptr->right->attribute.sval;
		switch(var_class)
		{
		case VAR:
			switch(var_type)
			{
			case INTEGER:
				make_var_inum(var_name);
				break;
			case REAL:
				make_var_rnum(var_name);
				break;
			default:
				fprintf(stderr, "\nERROR: invalid type, must be integer or real.\n");
				exit(0);
				break;
			}
			break;

		case ARRAY:
			switch(var_type)
			{
			case INTEGER:
				make_arr_inum(var_name, start_idx, stop_idx);
				break;
			case REAL:
				make_arr_rnum(var_name, start_idx, stop_idx);
				break;
			default:
				fprintf(stderr, "\nERROR: invalid type, must be integer or real.\n");
				exit(0);
				break;
			}
			break;
		default:
				fprintf(stderr, "\nERROR: invalid class declaration, must be variable or array.\n");
				exit(0);
		}
		var_ptr = var_ptr->left;
	}
}


/* Inserts "input" and/or "output" into top table
   if specified as a parameter to the main function */
void add_io(tree_t* ident_list_ptr)
{
	while(ident_list_ptr->type == LISTOP)
	{
		char* io_option = ident_list_ptr->right->attribute.sval;
		make_var_io(io_option);

		ident_list_ptr = ident_list_ptr->left;
	}
}


/* Given pointer to "param_list" in syntax tree, traverse and insert all 
   following variables into the current scope.
*/
void add_params(tree_t* param_ptr)
{
	// while there are more types of variables
	while(param_ptr->type != EMPTY)
	{
		int var_type = 0;
		int var_class = 0;
		int start_idx = 0;
		int stop_idx = 0;

		// determine and store type info
		tree_t* type_ptr = param_ptr->right->right;
		if(type_ptr->type == ARRAY) // handle arrays
		{
			var_class = ARRAY;
			var_type = type_ptr->right->type;
			
			type_ptr = type_ptr->left;
			if(type_ptr->type != DOTDOT)
			{
				fprintf(stderr, "\nERROR: parameter array index missing DOTDOT\n.");
				exit(0);
			}

			if(type_ptr->left->type != INUM || type_ptr->right->type != INUM)
			{
				fprintf(stderr, "\nERROR: parameter array indices must be integral.\n");
				exit(0);
			}
			start_idx = type_ptr->left->attribute.ival;
			stop_idx = type_ptr->right->attribute.ival;
		}
		else // handle basic types
		{
			var_class = VAR; // PARAM_VAR?
			var_type = type_ptr->type;
		}


		//while there are more identifiers of this type
		tree_t* var_ptr = param_ptr->right->left;
		value default_val;
		while(var_ptr->type == LISTOP)
		{
			char* var_name = var_ptr->right->attribute.sval;
			switch(var_class)
			{
			case VAR:
				switch(var_type)
				{
				case INTEGER:
					make_var_inum(var_name);
					//default_val.ival = 0;
					//make_entry(var_name, VAR, &default_val, INUM, 0, NULL, 0, 0);
					break;
				case REAL:
					make_var_rnum(var_name);
					break;
				default:
					fprintf(stderr, "\nERROR: invalid parameter type, must be integer or real.\n");
					exit(0);
				}
				break;

			case ARRAY:
				switch(var_type)
				{
				case INTEGER:
					make_arr_inum(var_name, start_idx, stop_idx);
					break;
				case REAL:
					make_arr_rnum(var_name, start_idx, stop_idx);
					break;
				default:
					fprintf(stderr, "\nERROR: invalid parameter type, must be integer or real.\n");
					exit(0);
				}
				break;
			default:
				fprintf(stderr, "\nERROR: invalid parameter class, must be variable or array.\n");
				exit(0);
			}
			var_ptr = var_ptr->left;
		}

		param_ptr = param_ptr->left;
	}
}


void make_var_inum(char* name)
{
	int default_ival = 0;

	// set applicable values
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->entry_name = strdup(name);
	ptr->entry_class = VAR;
	ptr->return_type = INUM;
	ptr->entry_value.ival = default_ival;
	ptr->next = NULL;

	// not applicable
	ptr->arg_num = 0;
	ptr->arg_types = NULL;
	ptr->start_idx = 0;
	ptr->stop_idx = 0;

	insert_entry(ptr, top_table());
}


void make_var_io(char* name)
{
	int default_ioval = 0;

	// set applicable values
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->entry_name = strdup(name);
	ptr->entry_class = VAR;
	ptr->return_type = PROGRAM;
	ptr->entry_value.ival = default_ioval;
	ptr->next = NULL;

	// not applicable
	ptr->arg_num = 0;
	ptr->arg_types = NULL;
	ptr->start_idx = 0;
	ptr->stop_idx = 0;

	insert_entry(ptr, top_table());
}


void make_var_rnum(char* name)
{
	float default_fval = 0.0;

	// set applicable values
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->entry_name = strdup(name);
	ptr->entry_class = VAR;
	ptr->return_type = RNUM;
	ptr->entry_value.fval = default_fval;
	ptr->next = NULL;

	// not applicable
	ptr->arg_num = 0;
	ptr->arg_types = NULL;
	ptr->start_idx = 0;
	ptr->stop_idx = 0;

	insert_entry(ptr, top_table());
}
	
void make_arr_inum(char* name, int start_idx, int stop_idx)
{
	int default_ival = 0;

	// set applicable values`
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->entry_name = strdup(name);
	ptr->entry_class = ARRAY;
	ptr->return_type = INUM;

	int* arr_ptr = (int*)malloc(sizeof(int)*(stop_idx-start_idx+1));
	for(int i = start_idx; i <= stop_idx; i++)
		arr_ptr[i-start_idx] = default_ival;
	
	ptr->entry_value.aival = arr_ptr;
	ptr->next = NULL;
	ptr->start_idx = start_idx;
	ptr->stop_idx = stop_idx;

	// not applicable
	ptr->arg_num = 0;
	ptr->arg_types = NULL;

	insert_entry(ptr, top_table());
}

void make_arr_rnum(char* name, int start_idx, int stop_idx)
{
	float default_fval = 0;

	// set applicable values`
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->entry_name = strdup(name);
	ptr->entry_class = ARRAY;
	ptr->return_type = RNUM;

	float* arr_ptr = (float*)malloc(sizeof(float)*(stop_idx-start_idx+1));
	for(int i = start_idx; i <= stop_idx; i++)
		arr_ptr[i-start_idx] = default_fval;
	
	ptr->entry_value.afval = arr_ptr;
	ptr->next = NULL;
	ptr->start_idx = start_idx;
	ptr->stop_idx = stop_idx;

	// not applicable
	ptr->arg_num = 0;
	ptr->arg_types = NULL;

	insert_entry(ptr, top_table());
}
