#ifndef ACCOUNTLIST_H_
#define ACCOUNTLIST_H_

struct node {
  char *account_id;
  int account_balance;
  struct node *next;
  struct tnode *transactions;
};

struct tnode {
  char *account_id;
  int transfered_amount;
  struct tnode *next;
};

typedef struct node node;
typedef struct tnode tnode;

//  node functions
void add_node(node **n, int init_amount, char *x);
int add_amount(node *n, int amount, char *x, char *y);
int decrease_amount(node *n, int amount, char *x);
void delete_node(node **n, char *x);
void destroy_list(node **n);
int find_node(node *n, char *x);
int get_balance(node *n, char *x);
tnode *get_node(node *n, char *x);
void print_list(node *n);

//  tnode functions
void add_transaction(tnode **n, int amount, char *x);
void delete_tnode(tnode **n, char *x);
void destroy_tlist(tnode **n);
int find_transaction(tnode *n, char *x);

#endif /* ACCOUNTLIST_H_ */
