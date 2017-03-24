#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"
#include "tree.h"
#include "y.tab.h"

table_t* head_table;

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
			switch(entry->return_type)
			{
			case INUM:
				fprintf(stderr, "%s\t%s\t%s\t%d\t\t%d\n", entry->entry_name, class_string(entry->entry_class), type_string(entry->return_type), entry->entry_value.ival, entry->arg_num);
				break;
			case RNUM:
				fprintf(stderr, "%s\t%s\t%s\t%f\t%d\n", entry->entry_name, class_string(entry->entry_class), type_string(entry->return_type), entry->entry_value.fval, entry->arg_num);
				break;
			case STRING:
				fprintf(stderr, "%s\t%s\t%s\t%s\t\t%d\n", entry->entry_name, class_string(entry->entry_class), type_string(entry->return_type), entry->entry_value.sval, entry->arg_num);
				break;
			default:
				fprintf(stderr, "%s\t%s\t%s\t%s\t%d\n", entry->entry_name, "unknown", 
					"none", "unknown", entry->arg_num);
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
		default:
			return "other";
	}
}


/* Given a token, returns matching class string for printing */
char* class_string(int token)
{
	switch(token)
	{
		case FUNCTION:
			return "fcn";
		case ARRAY:
			return "array";
		case VAR:
			return "var";
		default:
			return "other";
	}
}


/* Allocate memory for new hash table entry, and return pointer */
entry_t* create_entry(char* name, int entry_class, int return_type, int entry_ival, float entry_fval, char* entry_sval, int arg_num, int* arg_types)
{
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->entry_name = strdup(name);
	ptr->entry_class = entry_class;
	switch(entry_class)
	{
	case INUM:
		ptr-> entry_value.ival = entry_ival;
		break;
	case RNUM:
		ptr-> entry_value.fval = entry_fval;
		break;
	case STRING:
		ptr-> entry_value.sval = strdup(entry_sval);
		break;
	default:
		break;
	}
	ptr->arg_num = arg_num;
	ptr->arg_types = (int*)malloc(arg_num*sizeof(int));
	memcpy(ptr->arg_types, arg_types, arg_num*sizeof(int));
	ptr->return_type = return_type;
	ptr->next = NULL;
	return ptr;
}


/* Given an identifier name and hash table,
   finds entry in hash table if it exists */
// shouldn't just test name
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
// shouldn't just test name
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
		return 0; // return some error here later

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

/* Given pointer to "VAR" node in syntax tree, traverse and insert all 
   following variables into the current scope.
*/
void make_vars(tree_t* decl_ptr)
{
	// while there are more types of variables
	while(decl_ptr->type != EMPTY)
	{
		int var_type = 0;
		int var_class = 0;
		int start_idx = 0;
		int stop_idx = 0;

		// determine and store type info
		tree_t* type_ptr = decl_ptr->right;
		if(type_ptr->type == ARRAY) // handle arrays
		{
			var_class = ARRAY;
			var_type = type_ptr->right->type;
			
			type_ptr = type_ptr->left;
			if(type_ptr->type != DOTDOT)
			{
				fprintf(stderr, "Array index missing DOTDOT");
				exit(0);
			}

			if(type_ptr->left->type != INUM || type_ptr->right->type != INUM)
			{
				fprintf(stderr, "Array indices must be integer values.");
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

		tree_t* var_ptr = decl_ptr->left;
		if(var_ptr->type != VAR)
		{
			fprintf(stderr, "'var' keyword not in correct tree location.");
			exit(0);
		}
		var_ptr = var_ptr->right;

		
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
					fprintf(stderr, "Invalid type, must be integer or real.");
					exit(0);
				}
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
					fprintf(stderr, "Invalid type, must be integer or real.");
					exit(0);
				}
			}
			var_ptr = var_ptr->left;
		}

		decl_ptr = decl_ptr->left->left;
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
