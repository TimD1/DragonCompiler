#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"

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
void print_table(table_t* table)
{
	fprintf(stderr, "\n\nHASH TABLE %d\n_____________\n\n", table->id);
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		entry_t* entry = table->hash_table[i];
		while(entry != NULL)
		{
			switch(entry->entry_type)
			{
		case INT:
			fprintf(stderr, "%d:\t%p\t%s = %d\n", i, entry, entry->entry_name, entry->entry_value.ival);
			break;
		case FLT:
			fprintf(stderr, "%d:\t%p\t%s = %f\n", i, entry, entry->entry_name, entry->entry_value.fval);
			break;
		case STR:
			fprintf(stderr, "%d:\t%p\t%s = %s\n", i, entry, entry->entry_name, entry->entry_value.sval);
			break;
		case IDK:
			fprintf(stderr, "%d:\t%p\t%s = %s\n", i, entry, entry->entry_name, "type unknown");
			break;
		default:
			fprintf(stderr, "%d:\t%p\t%s = %s\n", i, entry, entry->entry_name, "none");
			break;
			}
			entry = entry->next;
		}
	}
	fprintf(stderr, "\n\n");
}


/* Allocate memory for new hash table entry, and return pointer */
entry_t* create_entry(char* name, type entry_type, int entry_ival, float entry_fval, char* entry_sval, int arg_num, type* arg_types, type return_type)
{
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->entry_name = strdup(name);
	ptr->entry_type = entry_type;
	switch(entry_type)
	{
	case INT:
		ptr-> entry_value.ival = entry_ival;
		break;
	case FLT:
		ptr-> entry_value.fval = entry_fval;
		break;
	case STR:
		ptr-> entry_value.sval = strdup(entry_sval);
		break;
	default:
		break;
	}
	ptr->arg_num = arg_num;
	ptr->arg_types = (type*)malloc(arg_num*sizeof(type));
	memcpy(ptr->arg_types, arg_types, arg_num*sizeof(type));
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
		return 0;

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
