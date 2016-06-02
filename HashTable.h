#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "AccountList.h"

struct Hashtable {
  int tables;
  node **hashtable;
};

typedef struct Hashtable Hashtable;

Hashtable *hash_create(int n);
void hash_insert(Hashtable *ht, int init_amount, char *id);
void hash_delete(Hashtable *ht, char *id);
void hash_destroy(Hashtable *ht);
tnode *hash_getBucket(Hashtable *ht, char *id);
void hash_printall(Hashtable *ht);
int hash_search(Hashtable *ht, char *id);
int hash_fillBucket(int table_len, char *id);

#endif /* HASHTABLE_H_ */
