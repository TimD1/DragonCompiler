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

	/* // remove linked lists of entries */
	/* for(int i = 0; i < TABLE_SIZE; i++) */

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
			fprintf(stderr, "%d:\t%s = %d\n", i, entry->name, entry->value);
			
			entry = entry->next;
		}
	}
	fprintf(stderr, "\n\n");
}


/* Allocate memory for new hash table entry,
 	and return pointer to it */
entry_t* create_entry(char* name)
{
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->name = strdup(name);
	ptr->next = NULL;
	ptr->value = 0;
	return ptr;
}


/* Given an identifier name and hash table,
   finds entry in hash table if it exists */
entry_t* get_entry(table_t* table, char* name)
{
	entry_t* cur_entry = table->hash_table[ hashpjw(name) ];
	while(cur_entry != NULL)
	{
		if( !strcmp(name, cur_entry->name) )
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
			if( !strcmp(name, cur_entry->name) )
				return cur_entry;
			else
				cur_entry = cur_entry->next;
		}
		table = table->prev;
	}
	return NULL;
}


/* Given identifier name and hash table,
   create entry for identifier if it doesn't exist */
int insert_entry(char* name, table_t* table)
{
	// entry already exists in table
	if(get_entry(table, name) != NULL)
		return 0;

	else // create new entry
	{
		entry_t* ptr = create_entry(name);
		entry_t* last_entry = table->hash_table[hashpjw(name)];
		// if list non-empty, append to end
		if(last_entry != NULL)
		{
			while(last_entry->next != NULL)
				last_entry = last_entry->next;
			last_entry->next = ptr;
		}
		else // create start of list
		{
			table->hash_table[hashpjw(name)] = ptr;
		}
		return 1;
	}
}
