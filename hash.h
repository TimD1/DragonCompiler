#ifndef HASH_H
#define HASH_H

#include "string.h"

#define TABLE_SIZE 211

// C create global table variable GLOBAL_TABLE

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


/* Single entry structure */
typedef struct entry_s {
	char *name;
	int value;
	struct entry_s *next; // linked list for collision
} entry_t;


/* Hash table structure */
typedef struct table_s {
	entry_t *hash_table[ TABLE_SIZE ];
	struct table_s *next; // effectively a stack
} table_t;


/* Allocate memory for new hash table entry,
 	and return pointer to it */
entry_t* create_entry(char* name)
{
	entry_t* ptr = (entry_t*)malloc(sizeof(entry_t));
	ptr->name = strdup(name);
	return ptr;
}


/* Given an identifier name and hash table,
   finds entry in hash table if it exists */
entry_t* find_entry(table_t* table, char* name)
{
	entry_t* cur_entry = table->hash_table[ hashpjw(name) ];
	while(cur_entry != NULL)
	{
		if( strcmp(name, cur_entry->name) )
			return cur_entry;
		else
			cur_entry = cur_entry->next;
	}
	return NULL;
}


/* Given identifier name and hash table,
   create entry for identifier if it doesn't exist */
int insert_entry(char* name, table_t* table)
{
	// entry already exists in table
	if(find_entry(table, name) != NULL)
		return 0;

	else // create new entry
	{
		entry_t* ptr = create_entry(name);
		entry_t* next_entry = table->hash_table[hashpjw(name)];
		while(next_entry != NULL) next_entry = next_entry->next;
		next_entry = ptr;
		return 1;
	}
}


/* Create new hash table */
table_t* create_table()
{
	table_t* ptr = (table_t*)malloc(sizeof(table_t));
	for(int i = 0; i < TABLE_SIZE; i++)
		ptr->hash_table[i] = NULL;
	ptr->next = NULL;
	return ptr;
}


/* Push new table onto the stack */
table_t* push_table()
{
	table_t* ptr = create_table();
	table_t* top_table = GLOBAL_TABLE;
	while(top_table->next != NULL)
		top_table = top_table->next;
	top_table->next = ptr;
}


/* Pop top table from the stack */
void pop_table()
{
	table_t* top_table = GLOBAL_TABLE;
	while(top_table->next != NULL)
		top_table = top_table->next;

	table_t* prev_table = GLOBAL_TABLE;
	while(prev_table->next != top_table)
	{
		if(prev_table->next == NULL)
		{
			fprintf(stderr, "Cannot pop first table!");
			return;
		}
		prev_table = prev_table->next;
	}
	prev_table->next = NULL;
}


#endif
