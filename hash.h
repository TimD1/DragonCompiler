#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 211

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


typedef struct node_s {
	char *name;
	int value;
	struct node_s *next; // linked list for collision
}
node_t;

typedef struct table_s {
	node_t *hash_table[ TABLE_SIZE ];
	struct table_s *next; // effectively a stack
}
table_t;

#endif
