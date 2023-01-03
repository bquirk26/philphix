#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h> //delete later TODO
/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  newTable->size = size;
  newTable->data = malloc(sizeof(struct HashBucket *) * size);
  for (i = 0; i < size; i++) {
    newTable->data[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/*
 * This inserts a key/data pair into a hash table.  To use this
 * to store strings, simply cast the char * to a void * (e.g., to store
 * the string referred to by the declaration char *string, you would
 * call insertData(someHashTable, (void *) string, (void *) string).
 */
void insertData(HashTable *table, void *key, void *data) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  // 2. Allocate a new hash bucket struct.
  // 3. Append to the linked list or create it if it does not yet exist. 
	unsigned int loc = (*(table->hashFunction))( key);
	HashBucket* new = (HashBucket *) malloc(sizeof(HashBucket));
	new->key = key;
	new->data =  data;
	new->next = (table->data)[loc];
	(table->data)[loc] = new;
}

/*
 * This returns the corresponding data for a given key.
 * It returns NULL if the key is not found. 
 */
void *findData(HashTable *table, void *key) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  // 2. Walk the linked list and check for equality with table->equalFunction.
	unsigned int loc = (*(table->hashFunction))(key); 
	if (loc > 96) {fprintf(stderr, "loc %d out of bounds", loc); return;}
	HashBucket *curr = ((table->data)[loc]); // invalid read
	while (curr) {
		if ((*table->equalFunction)(key, curr->key)) {
			return (void*) curr->data;
		}
		curr = curr->next;
	}
	return NULL;
}
void freeBucket(HashBucket *bucket) {
	if (bucket == NULL) return;
	if (bucket->data) free(bucket->data);
	if(bucket-> data) free(bucket->key);
	freeBucket(bucket->next);
	free(bucket);
}


void freeTable(HashTable *table) {
	for (int i = 0; i < table->size; i++) {
		freeBucket(table->data[i]);
	}
	free(table->data);
	free(table);
}		
