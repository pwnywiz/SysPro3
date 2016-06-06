#include <stdio.h>
#include <stdlib.h>

#include "HashTable.h"

//  Create a hash array of n tables
//  as well as allocate the required memory
Hashtable *hash_create(int n) {
  int i;

  Hashtable *hash = malloc(sizeof(Hashtable));
  hash->tables = n;
  hash->hashtable = malloc(n*sizeof(node));
  for (i = 0; i < n; i++) {
    hash->hashtable[i] = NULL;
  }

  return hash;
}

//  Check if the id has already been inserted
//  Then insert the new id into the hashtable
void hash_insert(Hashtable *ht, int init_amount, char *id) {
  int n = ht->tables;
  int bucket = hash_fillBucket(n, id);

  add_node(&(ht->hashtable[bucket]), init_amount, id);
}

//  Transfer an amount from account y to x
//  and then delete the same amount from account y
void hash_transfer(Hashtable *ht, int amount, char *x, char *y) {
  int n = ht->tables;
  int bucket1 = hash_fillBucket(n, x);
  int bucket2 = hash_fillBucket(n, y);

  add_amount(ht->hashtable[bucket1], amount, x, y);
  decrease_amount(ht->hashtable[bucket2], amount, y);
}

//  Get the balance of the account with name id
int hash_getBalance(Hashtable *ht, char *id) {
  int n = ht->tables;
  int bucket = hash_fillBucket(n, id);

  return get_balance(ht->hashtable[bucket], id);
}

//  Clear a specific account from the hashtable
void hash_delete(Hashtable *ht, char *id) {
  int n = ht->tables;
  int bucket = hash_fillBucket(n, id);

  delete_node(&(ht->hashtable[bucket]), id);
}

//  Clear all the hashtable nodes
void hash_destroy(Hashtable *ht) {
  int i;
  int n = ht->tables;
  for (i = 0; i < n; i++) {
    destroy_list(&ht->hashtable[i]);
  }
  for (i = 0; i < n; i++) {
    ht->hashtable[i] = NULL;
  }
  //  Uncomment only if user input gets removed
  // free(ht->hashtable);
  // free(ht);
}

//  Get a tnode with the specified id
tnode *hash_getBucket(Hashtable *ht, char *id) {
  int n = ht->tables;
  int bucket = hash_fillBucket(n, id);

  return get_node(ht->hashtable[bucket], id);
}

//  Print the whole hashtable
void hash_printall(Hashtable *ht) {
  int i;
  int size = ht->tables;
  for (i = 0; i < size; i++) {
    if (ht->hashtable[i] != NULL) {
      print_list(ht->hashtable[i]);
    }
  }
}

//  Search if if an account with id is inside the table
int hash_search(Hashtable *ht, char *id) {
  int n = ht->tables;
  int bucket = hash_fillBucket(n, id);

  return find_node(ht->hashtable[bucket], id);
}

//  Return the bucket in which the new item has to be added
int hash_fillBucket(int table_len, char *id) {
  unsigned long hash = 5381;
  int c;

  while (c = *id++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash%table_len;
}
